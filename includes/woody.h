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

#define PAYLOAD_LEN 570
#define MAX_SIZE 2147483648 // 2Gb

// ERROS
#define TOO_BIG "[!!] Origin file too long"
#define NOT_ELF_ERROR "[!!] Not ELF64 file or ELF error"
#define MALLOC_FAIL "[!!] Malloc Fail"
#define WRITE_FAIL "[!!] Write woody file fail"
#define KEYGEN_FAIL "[!!] Can not generate encrypt key"
#define ELF_HEADER_ERROR "[!!] Error, some problem in ELF Header"

// ELF HEADER
#define H_TYPE 16     // header type
#define H_SIZE 52     // header size
#define H_PH_OFF 32   // program header table offset
#define H_SH_OFF 40   // secction header table offset
#define H_PH_SIZE 54  // program header table entry size
#define H_PH_COUNT 56 // program header table entry count
#define H_SH_SIZE 58  // secction header table entry size
#define H_SH_COUNT 60 // secction header table entry count

typedef struct s_woody {
  Elf64_Ehdr *header;
  Elf64_Phdr *p_header;
  Elf64_Shdr *s_header;
  Elf64_Phdr *my_Pheader;
  Elf64_Off my_entry;
  Elf64_Addr origin_entry;
  unsigned int padding;
  void *file;
  size_t file_size;
  long int entry_distance;
  char *key;
  Elf64_Off text_off;
  Elf64_Off test; // test .text offset
  uint64_t text_size;
  Elf64_Addr text_dist;
  uint8_t key_size;

} t_woody;

// check_file.c
void main_checker(uint8_t *origin_file, size_t origin_len);
void check_origin_elf(uint8_t *origin_file, size_t origin_len);
void check_elf_header(uint8_t *origin_file, size_t origin_len);
void check_elf_size(uint8_t *origin_file, size_t origin_len);

// get_data_origin_file.c
void get_elf64_data(t_woody *woody, void *origin_file, ssize_t origin_len);
void get_elf64_header(t_woody *woody, void *origin_file);
void get_elf64_pheader(t_woody *woody, void *origin_file);
void get_elf64_sheader(t_woody *woody, void *origin_file);
void get_text_section(t_woody *woody, void *origin_file);
void get_origin_entry_point(t_woody *woody);
Elf64_Addr get_max_add(t_woody *woody);
void calculate_my_size_file(t_woody *woody, ssize_t origin_len);
void reserve_memory_to_my_file(t_woody *woody, void *origin_file,
                               ssize_t origin_len);

// error.c
void launch_error(char *msg, void *file, size_t file_len);

// prints.c
void print_elf_header(Elf64_Ehdr *header);
void print_elf64_phdrs(const Elf64_Phdr *p_header, int phnum);
void print_key(char *key, uint64_t key_size);

// get_data.c
void init_my_Pheader(t_woody *woody, ssize_t origin_len);
Elf64_Addr get_max_paddr(t_woody *woody);
Elf64_Addr get_max_vaddr(t_woody *woody);
void mod_phdr(t_woody *woody, ssize_t origin_len);
unsigned int calculate_padding(t_woody *woody, ssize_t origin_len);

// set_data.c
void main_set_data(t_woody *woody, void *origin_file, ssize_t origin_len);
void mod_origin_header(t_woody *woody, void *origin_file);
ssize_t put_data_in_buffer(t_woody *woody, void *origin_file,
                           ssize_t origin_len);
void patch_data(t_woody *woody, ssize_t count);
void mod_phdr_text_section(t_woody *woody);

// generate_file.c
void put_file(t_woody *woody, void *origin_file, ssize_t origin_len);

// free.c
void free_origin_file(void *origin_file, ssize_t origin_len);
void free_woody_file(void *file);
void free_pheaders(Elf64_Phdr *p_header);
void clean_up(t_woody *woody, void *origin_file, ssize_t origin_len);

// encrypt.c
void main_encrypt(t_woody *woody);
char *get_random_key(uint64_t key_size);
void encrypt_file(t_woody *woody);

#endif
