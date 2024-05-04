#include "../includes/woody.h"
#include <fcntl.h>
#include <stdio.h>

int check_the_origin_file(char *file) {
  int fd;
  fd = open(file, O_RDONLY);
  if (fd == -1) {
    perror("Can not open the source file");
    return (fd);
  }
  return (fd);
}
