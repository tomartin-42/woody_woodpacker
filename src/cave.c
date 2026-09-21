#include "../includes/woody.h"
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

int generate_cave(size_t origin_len, t_elf_info *elf_info) {
  uint64_t *injection_off = NULL;
  uint64_t *new_segment_vaddr = NULL;

  // Padding hasta payload
  if (!align_up(origin_len, 0x1000, injection_off)) {
    return (0);
  }
  if (!align_up(elf_info->max_load_end, 0x1000, new_segment_vaddr)) {
    return (0);
  }
  return (1);
}
