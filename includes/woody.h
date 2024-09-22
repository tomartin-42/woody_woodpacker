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

#define PAYLOAD_LEN 570
#define MAX_SIZE 2147483648 // 2Gb

// ERROS
#define TOO_BIG "[!!] Origin file too long"
#define NOT_ELF_ERROR "[!!] Not ELF64 file or ELF error"
#define MALLOC_FAIL "[!!] Malloc Fail"
#define WRITE_FAIL "[!!] Write woody file fail"
#define KEYGEN_FAIL "[!!] Can not generate encrypt key"
#define ELF_HEADER_ERROR "[!!] Error, some problem in ELF Header"
#define NOT_TEXT_SECTION "[!!] .text section not found"

// ELF HEADER 64
#define H_TYPE 16     // header type
#define H_SIZE 52     // header size
#define H_PH_OFF 32   // program header table offset
#define H_SH_OFF 40   // secction header table offset
#define H_PH_SIZE 54  // program header table entry size
#define H_PH_COUNT 56 // program header table entry count
#define H_SH_SIZE 58  // secction header table entry size
#define H_SH_COUNT 60 // secction header table entry count

// ELF HEADER 32
#define H_TYPE_32 16     // header type
#define H_SIZE_32 40     // header size
#define H_PH_OFF_32 28   // program header table offset
#define H_SH_OFF_32 32   // secction header table offset
#define H_PH_SIZE_32 42  // program header table entry size
#define H_PH_COUNT_32 44 // program header table entry count
#define H_SH_SIZE_32 46  // secction header table entry size
#define H_SH_COUNT_32 48 // secction header table entry count

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

typedef struct s_woody_32 {
  Elf32_Ehdr *header;
  Elf32_Phdr *p_header;
  Elf32_Shdr *s_header;
  Elf32_Phdr *my_Pheader;
  Elf32_Off my_entry;
  Elf32_Addr origin_entry;
  unsigned int padding;
  void *file;
  size_t file_size;
  long int entry_distance;
  char *key;
  Elf32_Off text_off;
  Elf32_Off test; // test .text offset
  uint32_t text_size;
  Elf32_Addr text_dist;
  uint8_t key_size;
} t_woody_32;

// check_file.c
int main_checker(uint8_t *origin_file, size_t origin_len);
void check_origin_elf(uint8_t *origin_file, size_t origin_len);
void check_elf_header(uint8_t *origin_file, size_t origin_len);
void check_elf_size(uint8_t *origin_file, size_t origin_len);
void check_elf_size_32(uint8_t *origin_file, size_t origin_len);

// get_data_origin_file.c
void get_elf64_data(t_woody *woody, void *origin_file, ssize_t origin_len);
void get_elf64_header(t_woody *woody, void *origin_file, ssize_t origin_len);
void get_elf64_pheader(t_woody *woody, void *origin_file, ssize_t origin_len);
void get_elf64_sheader(t_woody *woody, void *origin_file, ssize_t origin_len);
void get_text_section(t_woody *woody, void *origin_file, ssize_t origin_len);
void get_origin_entry_point(t_woody *woody);
Elf64_Addr get_max_add(t_woody *woody);
void calculate_my_size_file(t_woody *woody, ssize_t origin_len);
void reserve_memory_to_my_file(t_woody *woody, void *origin_file,
                               ssize_t origin_len);
unsigned int calculate_padding(t_woody *woody, ssize_t origin_len);
Elf64_Addr get_max_paddr(t_woody *woody);
Elf64_Addr get_max_vaddr(t_woody *woody);

// get_data_origin_file_32.c
void get_elf32_data(t_woody_32 *woody, void *origin_file, ssize_t origin_len);
void get_elf32_header(t_woody_32 *woody, void *origin_file);
void get_elf32_pheader(t_woody_32 *woody, void *origin_file);
void get_elf32_sheader(t_woody_32 *woody, void *origin_file,
                       ssize_t origin_len);
void get_text_section_32(t_woody_32 *woody, void *origin_file,
                         ssize_t origin_len);
void get_origin_entry_point_32(t_woody_32 *woody);
Elf32_Addr get_max_add_32(t_woody_32 *woody);
void calculate_my_size_file_32(t_woody_32 *woody, ssize_t origin_len);
void reserve_memory_to_my_file_32(t_woody_32 *woody, void *origin_file,
                                  ssize_t origin_len);
unsigned int calculate_padding_32(t_woody_32 *woody, ssize_t origin_len);
Elf32_Addr get_max_paddr_32(t_woody_32 *woody);
Elf32_Addr get_max_vaddr_32(t_woody_32 *woody);

// error.c
void launch_error(char *msg, void *file, size_t file_len);
void launch_headers_error(char *msg, t_woody *woody, void *file,
                          ssize_t file_len);
void launch_headers_error_32(char *msg, t_woody_32 *woody, void *file,
                             ssize_t file_len);

// prints.c
void print_elf_header(Elf64_Ehdr *header);
void print_elf_header_32(Elf32_Ehdr *header);
void print_elf64_phdrs(const Elf64_Phdr *p_header, int phnum);
void print_elf32_phdrs(const Elf32_Phdr *p_header, int phnum);
void print_key(char *key, uint64_t key_size);

// set_data.c
void init_my_Pheader(t_woody *woody, ssize_t origin_len);
void main_set_data(t_woody *woody, void *origin_file, ssize_t origin_len);
void mod_phdr(t_woody *woody, ssize_t origin_len);
void mod_origin_header(t_woody *woody, void *origin_file);
ssize_t put_data_in_buffer(t_woody *woody, void *origin_file,
                           ssize_t origin_len);
void patch_data(t_woody *woody, ssize_t count);
void mod_phdr_text_section(t_woody *woody);

// set_data_32.c
void init_my_Pheader_32(t_woody_32 *woody, ssize_t origin_len);
void main_set_data_32(t_woody_32 *woody, void *origin_file, ssize_t origin_len);
void mod_phdr_32(t_woody_32 *woody, ssize_t origin_len);
void mod_origin_header_32(t_woody_32 *woody, void *origin_file);
ssize_t put_data_in_buffer_32(t_woody_32 *woody, void *origin_file,
                              ssize_t origin_len);
void patch_data_32(t_woody_32 *woody, ssize_t count);
void mod_phdr_text_section_32(t_woody_32 *woody);

// generate_file.c
void put_file(t_woody *woody, void *origin_file, ssize_t origin_len);
void put_file_32(t_woody_32 *woody, void *origin_file, ssize_t origin_len);

// free.c
void free_origin_file(void *origin_file, ssize_t origin_len);
void free_woody_file(void *file);
void free_pheaders(Elf64_Phdr *p_header);
void free_pheaders_32(Elf32_Phdr *p_header);
void clean_up(t_woody *woody, void *origin_file, ssize_t origin_len);
void clean_up_32(t_woody_32 *woody, void *origin_file, ssize_t origin_len);
void free_woody_struct(t_woody *woody);
void free_woody_struct_32(t_woody_32 *woody);

// encrypt.c
void main_encrypt(t_woody *woody);
void main_encrypt_32(t_woody_32 *woody);
char *get_random_key(uint64_t key_size);
char *get_random_key_32(uint8_t key_size);
void encrypt_file(t_woody *woody);
void encrypt_file_32(t_woody_32 *woody);

#endif
