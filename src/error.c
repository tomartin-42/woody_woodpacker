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

void launch_headers_error(char *msg, t_woody *woody, void *file,
                          size_t file_len) {
  perror(msg);
  munmap(file, file_len);
  free_woody_struct(woody);
  exit(42);
}
