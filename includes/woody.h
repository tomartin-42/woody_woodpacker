#ifndef WOODY_H
#define WOODY_H

#include "../libft42/libft.h"
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define PAYLOAD                                                                \
  "\xBF\x01\x00\x00\x00\x48\xBE\x00\x20\x40\x00\x00\x00\x00\x00\xBA\x0D\x00"   \
  "\x00\x00\xB8\x01\x00\x00\x00\x0F\x05\x48\x31\xFF\xB8\x3C\x00\x00\x00\x0F"   \
  "\x05";

// ERROS
#define NOT_ELF_ERROR "Not ELF file"

typedef struct s_woody {
  Elf64_Ehdr *header;
  Elf64_Phdr *p_header;
  Elf64_Phdr *my_Pheader;
  int padding;
  // void *file;
  // size_t file_size;

} t_woody;

// check_file.c
int check_origin_elf(uint8_t *origin_file, size_t origin_len);

// get_data_origin_file.c
void get_elf64_data(t_woody *woody, void *origin_file);
void get_elf64_header(t_woody *woody, void *origin_file);
void get_elf64_pheader(t_woody *woody, void *origin_file);
void get_entry_point(t_woody *woody, void *origin_file);

// error.c
void launch_error(char *msg, void *file, size_t file_len);

// prints.c
void print_elf_header(Elf64_Ehdr *header);
void print_elf64_phdrs(const Elf64_Phdr *p_header, int phnum);

// get_data.c
void init_my_Pheader(t_woody *woody);
Elf64_Addr get_max_add(t_woody *woody);
void mod_phdr(t_woody *woody, ssize_t origin_len);

void main_set_data(t_woody *woody, void *origin_file, ssize_t origin_len);
void mod_origin_header(t_woody *woody, void *origin_file);

// generate_file.c
void put_file(t_woody *woody, void *origin_file, ssize_t origin_len);

#endif
