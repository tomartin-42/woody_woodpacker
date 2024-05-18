#ifndef WOODY_H
#define WOODY_H

#include "../libft42/libft.h"
#include <elf.h>

int check_the_origin_file(char *file);
int check_elf64_origin_file(int fd);
int main_check_input_file(char *file);

Elf64_Ehdr *get_elf64_header(int fd);
Elf64_Phdr *get_target_program_headers(Elf64_Ehdr *header, int fd);
#endif
