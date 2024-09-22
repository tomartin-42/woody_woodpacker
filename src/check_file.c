#include "../includes/woody.h"
#include <elf.h>
#include <stdint.h>
#include <stdlib.h>

static int get_elf_type_bits(uint8_t *origin_file) {
  if (origin_file[EI_CLASS] == ELFCLASS64) {
    return (64);
  }
  return (32);
}

int main_checker(uint8_t *origin_file, size_t origin_len) {
  int type; // 64bits or 32bits
  check_origin_elf(origin_file, origin_len);
  type = get_elf_type_bits(origin_file);
  if (type == 64) {
    check_elf_size(origin_file, origin_len);
  } else if (type == 32) {
    check_elf_size_32(origin_file, origin_len);
  }
  return (type);
}

void check_origin_elf(uint8_t *origin_file, size_t origin_len) {
  if (origin_len >= MAX_SIZE) {
    launch_error(TOO_BIG, origin_file, origin_len);
  }
  if (ft_memcmp(origin_file, ELFMAG, SELFMAG)) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }
  if (origin_file[EI_CLASS] != ELFCLASS64 &&
      origin_file[EI_CLASS] != ELFCLASS32) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }
  if ((int)origin_file[H_TYPE] != ET_EXEC &&
      (int)origin_file[H_TYPE] != ET_DYN) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }
}

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

void check_elf_size_32(uint8_t *origin_file, size_t origin_len) {
  if (origin_len < sizeof(Elf32_Ehdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if (origin_file[H_SIZE_32] != sizeof(Elf32_Ehdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if ((int)origin_file[H_PH_SIZE_32] != sizeof(Elf32_Phdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if ((int)origin_file[H_SH_SIZE_32] != sizeof(Elf32_Shdr)) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if (origin_file[H_SH_OFF] +
          (origin_file[H_SH_COUNT_32] * origin_file[H_SH_SIZE_32]) >
      (int)origin_len) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
  if (origin_file[H_PH_OFF_32] +
          (origin_file[H_PH_COUNT_32] * origin_file[H_PH_SIZE_32]) >
      (int)origin_len) {
    launch_error(ELF_HEADER_ERROR, origin_file, origin_len);
  }
}
