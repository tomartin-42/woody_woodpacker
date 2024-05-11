#include "../includes/woody.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int check;
  Elf64_Ehdr *header = NULL;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }
  check = main_check_input_file(argv[1]);
  if (check < 0) {
    exit(1);
  } else {
    header = get_elf64_header(argv[1]);
    printf("Entry Point -> 0x%lx\n", header->e_entry);
    printf("woody Entry Point -> %p\n", (void *)main);
    //  reopen input file and read
  }
}
