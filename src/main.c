#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  (void) argv;
  if (argc > 3 || argc < 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }
}
