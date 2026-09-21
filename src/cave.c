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
  Elf64_Ehdr *ehdr;
  Elf64_Phdr *new_phdrs;
  Elf64_Phdr *new_segment;

  ehdr = (Elf64_Ehdr *)elf_info->ehdr;

  // Reserva espacio para la tabla original y la nueva entrada PT_LOAD
  new_phdrs = malloc(cave_info->phdr_size);
  if (new_phdrs == NULL)
    return (NULL);

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
  return (new_phdrs);
}

int generate_cave64(size_t origin_len, t_elf_info *elf_info,
                    t_cave_info *cave_info) {
  Elf64_Phdr *new_phdrs;

  if (!load_data(origin_len, elf_info, cave_info)) {
    return (0);
  }

  new_phdrs = generate_new_phdr(elf_info, cave_info);
  if (!new_phdrs) {
    return (0);
  }

  return (1);
}
