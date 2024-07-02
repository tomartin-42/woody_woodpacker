#include "../includes/woody.h"

void put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  int fd;
  // Elf64_Addr highest_vaddr = get_max_add(woody);
  char a = '0';

  fd = open("woody", O_WRONLY | O_CREAT, 0644);
  write(fd, origin_file, origin_len);
  // printf("[ 1] Write: %d\n", tmp);
  for (int i = 0; i < woody->padding; i++) {
    write(fd, &a, 1);
  }
  write(fd, woody->p_header, (sizeof(Elf64_Phdr) * (woody->header->e_phnum)));
  // printf("[ 2] Write: %d\n", tmp);
  write(fd, woody->my_Pheader, sizeof(Elf64_Phdr));
  // printf("[ 3] Write: %d\n", tmp);
  close(fd);
}
