
#include "../includes/woody.h"
#include <elf.h>
#include <stdlib.h>
#include <sys/types.h>

void free_origin_file(void *origin_file, ssize_t origin_len) {
  munmap(origin_file, origin_len);
}

void free_woody_file(void *file) { free(file); }

void free_pheaders(Elf64_Phdr *p_header) { free(p_header); }

void clean_up(t_woody *woody, void *origin_file, ssize_t origin_len) {
  free_origin_file(origin_file, origin_len);
  free_woody_file(woody->file);
  free_pheaders(woody->p_header);
  free_pheaders(woody->my_Pheader);
  free(woody->s_header);
  // free(woody->key);
}
