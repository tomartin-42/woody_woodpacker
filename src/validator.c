#include "../includes/woody.h"

static int validate_ident(const unsigned char *original_file,
                          size_t original_len) {
  return (0);
}
static int validate_elf65(void *original_file, size_t original_len,
                          t_elf_info *elf_info) {
  return (0);
}
static int validate_elf32(void *original_file, size_t original_len,
                          t_elf_info *elf_info) {
  return (0);
}

int validate_elf(void *original_file, size_t original_len,
                 t_elf_info *elf_info) {
  if (!validate_ident(original_file, original_len)) {
    return (-1);
  }
  validate_elf65(original_file, original_len, elf_info);
  validate_elf32(original_file, original_len, elf_info);
  return (0);
}
