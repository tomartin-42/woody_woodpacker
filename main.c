#include <stdio.h>
#include <stdlib.h>

int check_if_file_ELF64(char *file) {
  printf("FILE= %s", file);
  return (0);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }

  check_if_file_ELF64(argv[1]);
}
