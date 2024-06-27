#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main_put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  init_my_Pheade(woody, origin_file);
  mod_origin_header(woody, origin_file, origin_len);
  put_file(woody, origin_file, origin_len);
}

Elf64_Addr get_max_add(t_woody *woody) {
  Elf64_Off max_add = 0;

  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_vaddr > max_add) {
      max_add = woody->p_header[i].p_vaddr;
    }
  }
  return (max_add);
}

void init_my_Pheade(t_woody *woody, void *origin_file) {
  (void)origin_file;
  woody->my_Pheader = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr));
  woody->my_Pheader->p_type = PT_LOAD;
  woody->my_Pheader->p_offset = 0x500000;
  woody->my_Pheader->p_vaddr = 0x500000;
  woody->my_Pheader->p_paddr = 0x500000;
  woody->my_Pheader->p_filesz = 0x60;
  woody->my_Pheader->p_memsz = 0x60;
  woody->my_Pheader->p_flags = 7;
  woody->my_Pheader->p_align = 0x1000;
}

void mod_origin_header(t_woody *woody, void *origin_file, ssize_t origin_len) {
  // TODO
  (void)woody;
  Elf64_Ehdr *tmp_header;

  tmp_header = (Elf64_Ehdr *)origin_file;
  tmp_header->e_phnum++;
  printf("POFF Before: %ld\n", tmp_header->e_phoff);
  printf("origin_len: %ld\n", origin_len);
  tmp_header->e_phoff = origin_len;
  printf("POFF After: %ld\n", tmp_header->e_phoff);
}

void put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  int fd;
  int tmp;

  fd = open("woody", O_WRONLY | O_CREAT, 0644);
  tmp = write(fd, origin_file, origin_len);
  printf("[ 1] Write: %d\n", tmp);
  tmp = write(fd, woody->p_header,
              (sizeof(Elf64_Phdr) * (woody->header->e_phnum)));
  printf("[ 2] Write: %d\n", tmp);
  tmp = write(fd, woody->my_Pheader, sizeof(Elf64_Phdr));
  printf("[ 3] Write: %d\n", tmp);
  close(fd);
}
