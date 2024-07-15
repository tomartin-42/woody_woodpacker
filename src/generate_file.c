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
  size_t enty_off = 0;

  // Origin file
  ft_memcpy(woody->file, origin_file, origin_len);
  count = origin_len;
  // Padding
  ft_memset(woody->file + count, 42, woody->padding);
  count += woody->padding;
  // P_headers
  ft_memcpy(woody->file + count, woody->p_header,
            (sizeof(Elf64_Phdr) * (woody->header->e_phnum)));
  count += (sizeof(Elf64_Phdr) * (woody->header->e_phnum));
  enty_off = count + sizeof(Elf64_Phdr);
  woody->my_entry =
      woody->my_Pheader->p_vaddr + (enty_off - woody->my_Pheader->p_offset);
  // woody->my_Pheader->p_offset = woody->my_entry;
  // My_p_header
  ft_memcpy(woody->file + count, woody->my_Pheader, sizeof(Elf64_Phdr));
  count += sizeof(Elf64_Phdr);

  char code[] =
      "\xbf\x01\x00\x00\x00\x48\x8d\x35\x1c\x00\x00\x00\xba\x0a\x00\x00\x00\xb8"
      "\x01\x00\x00\x00\x0f\x05\xb2\x2a\x31\xc0\xff\xc0\xf6\xe2\x89\xc7\x31\xc0"
      "\xb0\x3c\x0f\x05\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

  // printf("init shellcode: 0x%lx\n", woody->my_entry);
  //
  // char code[] =
  // "\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x12\x00\x00\x00"
  //               "\x0f\x05\xb2\x2a\x31\xc0\xff\xc0\xf6\xe2\x89\xc7\x31\xc0\xb0"
  //               "\x3c\x0f\x05\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

  // char code[] =
  //     "\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x12\x00\x00\x00\x0f\x05"
  //     "\xb2\x2a\x31\xc0\xff\xc0\xf6\xe2\x89\xc7\x31\xc0\xb0\x3c\x0f\x05\x2e\x2e"
  //     "\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

  ft_memcpy(woody->file + count, code, 60);
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
