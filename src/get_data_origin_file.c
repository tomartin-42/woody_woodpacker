#include "../includes/woody.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

Elf64_Ehdr *get_elf64_header(char *file) {
  Elf64_Ehdr *header = (Elf64_Ehdr *)malloc(sizeof(Elf64_Ehdr));
  if (header == NULL) {
    return (header);
  }

  int fd = open(file, O_RDONLY);
  if (fd < 0) {
    free(header);
    return (header = NULL);
  }

  int r = 0;
  r = read(fd, header, sizeof(Elf64_Ehdr));
  if (r != 64) {
    free(header);
    return (header = NULL);
  }

  return (header);
}
