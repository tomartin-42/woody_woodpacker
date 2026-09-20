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
  uint64_t max_load_end; // Fin virtual más alto de los PT_LOAD; base del nuevo
                         // segmento
} t_elf_info;

// Declaraciones para acceso al payload64.asm
extern const unsigned char payload64_start[];
extern const unsigned char payload64_end[];
extern const unsigned char payload64_text_delta[];
extern const unsigned char payload64_entry_delta[];
extern const unsigned char payload64_text_size[];
extern const unsigned char payload64_key_size[];
extern const unsigned char payload64_woody_str[];
extern const unsigned char payload64_key[];
int validate_elf(const unsigned char *file, size_t file_size, t_elf_info *info);
int validate_text_segment(const unsigned char *file, size_t file_size,
                          t_elf_info *elf_info);

#endif
