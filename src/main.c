#include "../includes/woody.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int check;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }
  check = main_check_input_file(argv[1]);
  if (check < 0) {
    exit(1);
  } else {
    get_elf64_header(argv[1]);
    //  reopen input file and read
  }
}
