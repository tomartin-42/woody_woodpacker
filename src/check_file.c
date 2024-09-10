#include "../includes/woody.h"

void check_origin_elf(uint8_t *origin_file, size_t origin_len) {
  if (origin_len >= MAX_SIZE) {
    launch_error(TOO_BIG, origin_file, origin_len);
  }

  if (ft_memcmp(origin_file, ELFMAG, SELFMAG)) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }

  if (origin_file[EI_CLASS] != ELFCLASS64) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }
}
