#include "../includes/woody.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }
  if (check_the_origin_file(argv[1])) {
    exit(1);
  }
}
