#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int check_the_origin_file(char *file) {
  int fd;
  fd = open(file, O_RDONLY);
  if (fd == -1) {
    perror("Can not open the source file");
    return (fd);
  }
  return (fd);
}
int check_elf64_origin_file(int fd) {
  Elf64_Ehdr header = {0};
  ssize_t i;
  i = read(fd, &header, sizeof(header));
  return (i);
}
