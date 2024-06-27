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
  void *file;
  size_t file_size;
} t_woody;

// int check_the_origin_file(char *file);
int check_origin_elf(uint8_t *origin_file, size_t origin_len);
// int main_check_input_file(char *file);

void get_elf64_data(t_woody *woody, void *origin_file);
void get_elf64_header(t_woody *woody, void *origin_file);
void get_elf64_pheader(t_woody *woody, void *origin_file);
// get_data_origin_file
// Elf64_Ehdr *get_elf64_header(int fd);
// Elf64_Phdr *get_target_program_headers(Elf64_Ehdr *header, int fd);
// void *map_file(int fd);

// print.c
// void print_header(Elf64_Ehdr *header);
// void print_p_headers(Elf64_Phdr *p_header, int range);
// void print_info_p_headers(Elf64_Phdr *p_headers, int range);

// error.c
void launch_error(char *msg, void *file, size_t file_len);

// prints.c
void print_elf_header(Elf64_Ehdr *header);
void print_elf64_phdrs(const Elf64_Phdr *p_header, int phnum);

// get_data.c
void init_my_Pheade(t_woody *woody, void *origin_file);
Elf64_Addr get_max_add(t_woody *woody);

void main_put_file(t_woody *woody, void *origin_file, ssize_t origin_len);
void put_file(t_woody *woody, void *origin_file, ssize_t origin_len);
void mod_origin_header(t_woody *woody, void *origin_file, ssize_t origin_len);
#endif
