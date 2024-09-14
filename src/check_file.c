#include "../includes/woody.h"
#include <elf.h>
#include <stdint.h>

void main_checker(uint8_t *origin_file, size_t origin_len) {
  check_origin_elf(origin_file, origin_len);
  check_elf_size(origin_file, origin_len);
}

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
  if ((int)origin_file[H_TYPE] != ET_EXEC &&
      (int)origin_file[H_TYPE] != ET_DYN) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }
}

/* void check_elf_header(uint8_t *origin_file, size_t origin_len) {} */

void check_elf_size(uint8_t *origin_file, size_t origin_len) {
  if (origin_len < sizeof(Elf64_Ehdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if (origin_file[H_SIZE] != sizeof(Elf64_Ehdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if ((int)origin_file[H_PH_SIZE] != sizeof(Elf64_Phdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if ((int)origin_file[H_SH_SIZE] != sizeof(Elf64_Shdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if (origin_file[H_SH_OFF] +
          (origin_file[H_SH_COUNT] * origin_file[H_SH_SIZE]) >
      (int)origin_len) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if (origin_file[H_PH_OFF] +
          (origin_file[H_PH_COUNT] * origin_file[H_PH_SIZE]) >
      (int)origin_len) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
}

/* void check_elf_header(uint8_t *origin_file, size_t origin_len) {} */
