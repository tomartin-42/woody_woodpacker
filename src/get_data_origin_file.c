#include "../includes/woody.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

Elf64_Ehdr *get_elf64_header(char *file) {
  Elf64_Ehdr *header = (Elf64_Ehdr *)malloc(sizeof(Elf64_Ehdr));
  int fd = open(file, O_RDONLY);

  read(fd, header, sizeof(Elf64_Ehdr));

  return (header);
}
