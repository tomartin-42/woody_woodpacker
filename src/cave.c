#include "../includes/woody.h"
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

int generate_cave64(size_t origin_len, t_elf_info *elf_info,
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
