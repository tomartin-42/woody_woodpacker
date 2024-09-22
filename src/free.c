
#include "../includes/woody.h"
#include <elf.h>
#include <stdlib.h>
#include <sys/types.h>

void free_origin_file(void *origin_file, ssize_t origin_len) {
  munmap(origin_file, origin_len);
}

void free_woody_file(void *file) { free(file); }

void free_pheaders(Elf64_Phdr *p_header) { free(p_header); }

void free_pheaders_32(Elf32_Phdr *p_header) { free(p_header); }

void clean_up(t_woody *woody, void *origin_file, ssize_t origin_len) {
  free_origin_file(origin_file, origin_len);
  free_woody_file(woody->file);
  free_pheaders(woody->p_header);
  free(woody->header);
  free(woody->my_Pheader);
  free(woody->s_header);
  free(woody->key);
  free(woody);
}

void clean_up_32(t_woody_32 *woody, void *origin_file, ssize_t origin_len) {
  free_origin_file(origin_file, origin_len);
  free_woody_file(woody->file);
  free_pheaders_32(woody->p_header);
  free(woody->header);
  free(woody->my_Pheader);
  free(woody->s_header);
  free(woody->key);
  free(woody);
}

void free_woody_struct(t_woody *woody) {
  if (woody->header != NULL) {
    free(woody->header);
  }
  if (woody->p_header != NULL) {
    free(woody->p_header);
  }
  if (woody->s_header != NULL) {
    free(woody->s_header);
  }
  if (woody->my_Pheader != NULL) {
    free(woody->my_Pheader);
  }
  if (woody->file != NULL) {
    free(woody->file);
  }
  if (woody->key != NULL) {
    free(woody->key);
  }
  if (woody != NULL) {
    free(woody);
  }
}

void free_woody_struct_32(t_woody_32 *woody) {
  if (woody->header != NULL) {
    free(woody->header);
  }
  if (woody->p_header != NULL) {
    free(woody->p_header);
  }
  if (woody->s_header != NULL) {
    free(woody->s_header);
  }
  if (woody->my_Pheader != NULL) {
    free(woody->my_Pheader);
  }
  if (woody->file != NULL) {
    free(woody->file);
  }
  if (woody->key != NULL) {
    free(woody->key);
  }
  if (woody != NULL) {
    free(woody);
  }
}
