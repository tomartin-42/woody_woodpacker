#include <stddef.h>
#include <stdint.h>
#ifndef WOODY_H
#define WOODY_H

typedef enum e_elf_class { WOODY_ELF32, WOODY_ELF64 } t_elf_class;

typedef struct s_elf_info {
  t_elf_class elf_class;
  void *ehdr;
  void *phdrs;
  void *shdrs;
  void *text_shdr;
  void *text_segment;
  uint64_t old_entry;
  uint64_t text_offset;
  uint64_t text_vaddr;
  uint64_t text_size;
  uint64_t max_load_end;
} t_elf_info;

int validate_elf(const unsigned char *file, size_t file_size, t_elf_info *info);

#endif // DEBUG
