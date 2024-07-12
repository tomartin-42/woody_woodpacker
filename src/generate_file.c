#include "../includes/woody.h"
#include <elf.h>
#include <sys/types.h>

void put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  int fd;
  // Elf64_Addr highest_vaddr = get_max_add(woody);
  char a = '0';

  fd = open("woody", O_WRONLY | O_CREAT, 0777);
  write(fd, origin_file, origin_len);
  // printf("[ 1] Write: %d\n", tmp);
  for (unsigned int i = 0; i < woody->padding; i++) {
    write(fd, &a, 1);
  }
  write(fd, woody->p_header, (sizeof(Elf64_Phdr) * (woody->header->e_phnum)));
  // printf("[ 2] Write: %d\n", tmp);
  write(fd, woody->my_Pheader, sizeof(Elf64_Phdr));
  // printf("[ 3] Write: %d\n", tmp);
  close(fd);
}

void put_data_in_buffer(t_woody *woody, void *origin_file, ssize_t origin_len) {
  size_t count = 0;
  int fd;

  ft_memcpy(woody->file, origin_file, origin_len); // Origin file
  count = origin_len;
  ft_memset(woody->file + count, 42, woody->padding); // Padding
  count += woody->padding;
  printf("[!] File Adders My New Section: 0x%lx\n", count);
  ft_memcpy(woody->file + count, woody->p_header,
            (sizeof(Elf64_Phdr) * (woody->header->e_phnum))); // P_headers
  count += (sizeof(Elf64_Phdr) * (woody->header->e_phnum));
  woody->my_entry = count + sizeof(Elf64_Phdr);
  woody->my_Pheader->p_offset = woody->my_entry;
  ft_memcpy(woody->file + count, woody->my_Pheader,
            sizeof(Elf64_Phdr)); // My_p_header
  count += sizeof(Elf64_Phdr);

  printf("init shellcode: 0x%lx\n", woody->my_entry);

  char code[] =
      "\xbf\x01\x00\x00\x00\x48\x8b\x35\xf4\x0f\x00\xba\x09\x00\x00\x00\xb8\x01"
      "\x00\x00\x00\x0f\x05\x48\x31\xff\xb8\x3c\x00\x00\x00\x0f\x05";

  ft_memcpy(woody->file + count, code, 44);
  // change entry
  Elf64_Ehdr *tmp = (Elf64_Ehdr *)woody->file;
  woody->origin_entry = tmp->e_entry;
  tmp->e_entry = woody->my_entry;

  fd = open("woody2", O_WRONLY | O_CREAT, 0777);
  if (write(fd, woody->file, woody->file_size) != (long int)woody->file_size) {
    launch_error(WRITE_FAIL, origin_file, origin_len);
  }
  close(fd);
  clean_up(woody, origin_file, origin_len);
}
