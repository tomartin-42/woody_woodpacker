#include "../includes/woody.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int fd;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }
  fd = check_the_origin_file(argv[1]);
  if (fd < 0) {
    exit(1);
  }
  printf("%d", check_elf64_origin_file(fd));
}
