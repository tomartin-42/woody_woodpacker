#include "../includes/woody.h"
#include <elf.h>

static int validate_ident(const unsigned char *original_file,
                          size_t original_len) {
  // Comprueba que se pueda leer la identificación completa del ELF
  if (original_file == NULL || original_len < EI_NIDENT) {
    return (0);
  }

  // Comprueba los bytes mágicos que identifican un archivo ELF
  if (original_file[EI_MAG0] != ELFMAG0 || original_file[EI_MAG1] != ELFMAG1 ||
      original_file[EI_MAG2] != ELFMAG2 || original_file[EI_MAG3] != ELFMAG3) {
    return (0);
  }

  // Comprueba que el ejecutable sea de 32 o 64 bits
  if (original_file[EI_CLASS] != ELFCLASS32 &&
      original_file[EI_CLASS] != ELFCLASS64) {
    return (0);
  }

  // Comprueba que use little-endian y una versión ELF válida
  if (original_file[EI_DATA] != ELFDATA2LSB ||
      original_file[EI_VERSION] != EV_CURRENT) {
    return (0);
  }

  // Los ejecutables Linux pueden declarar ABI System V o Linux
  if (original_file[EI_OSABI] != ELFOSABI_SYSV &&
      original_file[EI_OSABI] != ELFOSABI_LINUX) {
    return (0);
  }

  return (1);
}

static int validate_elf64(const unsigned char *original_file,
                          size_t original_len, t_elf_info *elf_info) {
  if (original_len < sizeof(Elf64_Ehdr)) {
    return (0);
  }

  return (1);
}
static int validate_elf32(const unsigned char *original_file,
                          size_t original_len, t_elf_info *elf_info) {
  return (0);
}

int validate_elf(const unsigned char *original_file, size_t original_len,
                 t_elf_info *elf_info) {
  if (!validate_ident(original_file, original_len)) {
    return (-1);
  }
  if (original_file[EI_CLASS] == ELFCLASS64) {
    validate_elf64(original_file, original_len, elf_info);
  } else {
    validate_elf32(original_file, original_len, elf_info);
  }
  return (0);
}
