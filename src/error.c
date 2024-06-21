#include "../includes/woody.h"
#include <stdio.h>
#include <sys/mman.h>

void launch_error(char *msg, void *file, size_t file_len) {
  perror(msg);
  if (file) {
    munmap(file, file_len);
  }
  exit(42);
}
