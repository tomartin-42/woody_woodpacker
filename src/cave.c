#include "../includes/woody.h"
#include "libft.h"
#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static int align_up(uint64_t value, uint64_t alignment, uint64_t *result) {
  uint64_t mask;

  if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
    return (0);
  }

  mask = alignment - 1;
  if (value > UINT64_MAX - mask)
    return (0);

  *result = (value + mask) & ~mask;
  return (1);
}

static int load_data(size_t origin_len, t_elf_info *elf_info,
                     t_cave_info *cave_info) {
  // Sitúa la zona añadida al comienzo de la siguiente página del archivo
  if (!align_up(origin_len, 0x1000, &cave_info->injection_offset))
    return (0);

  // Coloca el nuevo PT_LOAD después de todos los segmentos cargables
  if (!align_up(elf_info->max_load_end, 0x1000, &cave_info->new_segment_vaddr))
    return (0);

  // Reserva una entrada adicional para el nuevo PT_LOAD
  cave_info->new_phnum = ((const Elf64_Ehdr *)elf_info->ehdr)->e_phnum + 1;

  // La nueva tabla PHDR empieza al inicio de la zona añadida
  cave_info->phdr_offset = cave_info->injection_offset;

  // Calcula el tamaño completo de la nueva tabla PHDR
  cave_info->phdr_size = cave_info->new_phnum * sizeof(Elf64_Phdr);

  // Alinea el comienzo del payload después de la tabla PHDR
  if (!align_up(cave_info->phdr_offset + cave_info->phdr_size, 16,
                &cave_info->payload_offset))
    return (0);

  // Obtiene el tamaño del blob delimitado por los símbolos de assembly
  cave_info->payload_size =
      (uint64_t)((uintptr_t)payload64_end - (uintptr_t)payload64_start);

  // Convierte el offset del payload en su dirección virtual dentro del segmento
  cave_info->payload_vaddr =
      cave_info->new_segment_vaddr +
      (cave_info->payload_offset - cave_info->injection_offset);

  return (1);
}

static Elf64_Phdr *generate_new_phdr(t_elf_info *elf_info,
                                     t_cave_info *cave_info) {
  const Elf64_Ehdr *ehdr;
  Elf64_Phdr *new_phdrs;
  Elf64_Phdr *new_segment;
  unsigned int found;

  ehdr = (const Elf64_Ehdr *)elf_info->ehdr;

  // Reserva espacio para la tabla original y la nueva entrada PT_LOAD
  new_phdrs = malloc(cave_info->phdr_size);
  if (new_phdrs == NULL) {
    return (NULL);
  }

  // Copia las entradas originales al comienzo de la nueva tabla
  ft_memcpy(new_phdrs, elf_info->phdrs, ehdr->e_phnum * sizeof(Elf64_Phdr));

  // Añade el nuevo segmento en la última posición de la tabla
  new_segment = &new_phdrs[ehdr->e_phnum];
  ft_bzero(new_segment, sizeof(*new_segment));

  // El nuevo PT_LOAD carga la tabla PHDR, su padding y el payload
  new_segment->p_type = PT_LOAD;
  new_segment->p_flags = PF_R | PF_X;
  new_segment->p_offset = cave_info->injection_offset;
  new_segment->p_vaddr = cave_info->new_segment_vaddr;
  new_segment->p_paddr = cave_info->new_segment_vaddr;

  // El final físico del segmento coincide con el final del payload. Al restar
  // su offset inicial se obtiene el tamaño total de PHDR + padding + payload.
  new_segment->p_filesz = cave_info->payload_offset + cave_info->payload_size -
                          cave_info->injection_offset;

  // No se reserva memoria adicional sin contenido, por eso ambos tamaños son
  // iguales
  new_segment->p_memsz = new_segment->p_filesz;
  new_segment->p_align = 0x1000;

  // Si existe PT_PHDR, actualiza la ubicación de la tabla que describe
  found = 0;
  for (size_t i = 0; i < ehdr->e_phnum; i++) {
    if (new_phdrs[i].p_type == PT_PHDR) {
      found++;
      new_phdrs[i].p_offset = cave_info->phdr_offset;

      // Convierte el offset de la tabla dentro del nuevo PT_LOAD a dirección
      // virtual: base virtual + (offset de la tabla - offset del segmento).
      new_phdrs[i].p_vaddr =
          cave_info->new_segment_vaddr +
          (cave_info->phdr_offset - cave_info->injection_offset);

      // PT_PHDR describe exactamente la nueva tabla en memoria y en archivo
      new_phdrs[i].p_paddr = new_phdrs[i].p_vaddr;
      new_phdrs[i].p_filesz = cave_info->phdr_size;
      new_phdrs[i].p_memsz = cave_info->phdr_size;
      new_phdrs[i].p_flags = PF_R;
      new_phdrs[i].p_align = 8;
    }
  }

  // PT_PHDR es opcional, pero una tabla no debe contener más de uno
  if (found > 1) {
    free(new_phdrs);
    return (NULL);
  }

  return (new_phdrs);
}

static unsigned char *generate_payload(t_elf_info *elf_info,
                                       t_cave_info *cave_info,
                                       const unsigned char *key,
                                       size_t key_size) {
  unsigned char *payload;
  int64_t delta;
  uint64_t patched_size;
  size_t offset;
  size_t payload_size;

  if (elf_info == NULL || cave_info == NULL || key == NULL || key_size == 0 ||
      key_size > 64 || cave_info->payload_size > SIZE_MAX)
    return (NULL);

  payload_size = (size_t)cave_info->payload_size;
  payload = malloc(payload_size);
  if (payload == NULL)
    return (NULL);

  // Crea una copia modificable; la plantilla enlazada permanece intacta.
  ft_memcpy(payload, payload64_start, payload_size);

  // Cada offset se obtiene restando al símbolo parcheable el inicio del blob.
  // Así los cambios en el assembly no obligan a mantener offsets manuales.
  offset =
      (size_t)((uintptr_t)payload64_text_delta - (uintptr_t)payload64_start);

  // Parchea la distancia relativa desde el payload hasta la sección .text.
  delta = (int64_t)elf_info->text_vaddr - (int64_t)cave_info->payload_vaddr;
  ft_memcpy(payload + offset, &delta, sizeof(delta));

  // Parchea la distancia relativa hasta el entry point original.
  offset =
      (size_t)((uintptr_t)payload64_entry_delta - (uintptr_t)payload64_start);
  delta = (int64_t)elf_info->old_entry - (int64_t)cave_info->payload_vaddr;
  ft_memcpy(payload + offset, &delta, sizeof(delta));

  // Parchea el tamaño de .text usando el qword reservado en el payload.
  offset =
      (size_t)((uintptr_t)payload64_text_size - (uintptr_t)payload64_start);
  patched_size = elf_info->text_size;
  ft_memcpy(payload + offset, &patched_size, sizeof(patched_size));

  // Parchea el número de bytes de clave que usará el bucle XOR.
  offset = (size_t)((uintptr_t)payload64_key_size - (uintptr_t)payload64_start);
  patched_size = key_size;
  ft_memcpy(payload + offset, &patched_size, sizeof(patched_size));

  // Sustituye los bytes reservados para la clave dentro de la copia.
  offset = (size_t)((uintptr_t)payload64_key - (uintptr_t)payload64_start);
  ft_memcpy(payload + offset, key, key_size);
  return (payload);
}

int generate_cave64(size_t origin_len, t_elf_info *elf_info,
                    t_cave_info *cave_info, const unsigned char *key,
                    size_t key_size) {
  if (!load_data(origin_len, elf_info, cave_info)) {
    return (0);
  }

  cave_info->new_phdrs = generate_new_phdr(elf_info, cave_info);
  if (!cave_info->new_phdrs) {
    return (0);
  }

  cave_info->payload = generate_payload(elf_info, cave_info, key, key_size);
  if (!cave_info->payload) {
    free(cave_info->new_phdrs);
    cave_info->new_phdrs = NULL;
    return (0);
  }

  return (1);
}
