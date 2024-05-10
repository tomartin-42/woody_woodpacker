#include "../includes/woody.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

char *read_input_file(char *file) {
  char *i_file;
  int fd;

  fd = open(file, O_RDONLY);
  if (fd == -1) {
    perror("Can not open the source file");
    exit(fd);
  }
  return ("HOLA");
}
