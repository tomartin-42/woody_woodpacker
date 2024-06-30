#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main_put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  init_my_Pheader(woody, origin_file);
  mod_origin_header(woody, origin_file, origin_len);
  mod_phdr(woody, origin_file, origin_len);
  put_file(woody, origin_file, origin_len);
}

Elf64_Addr get_max_add(t_woody *woody) {
  Elf64_Addr highest_vaddr = 0;

  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz >
        highest_vaddr) {
      highest_vaddr = woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz;
    }
  }
  return (highest_vaddr);
}

void init_my_Pheader(t_woody *woody, void *origin_file) {
  (void)origin_file;
  Elf64_Addr highest_vaddr = get_max_add(woody);

  woody->my_Pheader = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr));
  woody->my_Pheader->p_type = PT_LOAD;
  woody->my_Pheader->p_offset = (highest_vaddr + 0xfff) & ~0xfff;
  woody->my_Pheader->p_vaddr = (highest_vaddr + 0xfff) & ~0xfff;
  woody->my_Pheader->p_paddr = (highest_vaddr + 0xfff) & ~0xfff;
  woody->my_Pheader->p_filesz = 0x1000;
  woody->my_Pheader->p_memsz = 0x1000;
  woody->my_Pheader->p_flags = PF_X | PF_W | PF_R;
  woody->my_Pheader->p_align = 0x1000;
}

void mod_origin_header(t_woody *woody, void *origin_file, ssize_t origin_len) {
  // TODO
  (void)woody;

  Elf64_Ehdr *tmp_header = (Elf64_Ehdr *)origin_file;
  tmp_header->e_phnum++;
  // printf("POFF Before: %ld\n", tmp_header->e_phoff);
  // printf("origin_len: %ld\n", origin_len);
  tmp_header->e_phoff = origin_len;
  // printf("POFF After: %ld\n", tmp_header->e_phoff);
}

void mod_phdr(t_woody *woody, void *origin_file, ssize_t origin_len) {

  (void)origin_file;
  Elf64_Addr highest_vaddr = get_max_add(woody);

  printf("[!] %ld - %ld\n", (highest_vaddr + 0xfff) & ~0xfff, highest_vaddr);
  printf("[!] %ld\n", ((highest_vaddr + 0xfff) & ~0xfff) - highest_vaddr);
  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_type == PT_PHDR) {
      woody->p_header[i].p_offset = origin_len;
      woody->p_header[i].p_vaddr = origin_len;
      woody->p_header[i].p_paddr = origin_len;
      woody->p_header[i].p_memsz += 54;
      woody->p_header[i].p_filesz += 54;
      break;
    }
  }
  // woody->cover_Pheader = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr));
  // woody->cover_Pheader->p_type = PT_LOAD;
  // woody->cover_Pheader->p_offset = origin_len;
  // woody->cover_Pheader->p_vaddr = origin_len;
  // woody->cover_Pheader->p_paddr = origin_len;
  // woody->cover_Pheader->p_filesz = woody->p_header[i].p_memsz;
  // woody->cover_Pheader->p_memsz = woody->p_header[i].p_memsz;
  // woody->cover_Pheader->p_flags = PF_X | PF_W | PF_R;
  // woody->cover_Pheader->p_align = 0x8;
}

// static int get_padding(t_woody *woody, ssize_t origin_len) {
//   (void)origin_len;
//   int padding = 0;
//
//   Elf64_Addr highest_vaddr = get_max_add(woody);
//   padding = ((highest_vaddr + 0xfff) & ~0xfff) - highest_vaddr;
//
//   printf("[!] %d\n", padding);
//   return (padding);
// }

void put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  int fd;
  int tmp;
  // Elf64_Addr highest_vaddr = get_max_add(woody);
  // char a = '0';

  fd = open("woody", O_WRONLY | O_CREAT, 0644);
  tmp = write(fd, origin_file, origin_len);
  printf("[ 1] Write: %d\n", tmp);
  // for (long unsigned int i = 0;
  //      i < ((highest_vaddr + 0xfff) & ~0xfff) - highest_vaddr; i++) {
  //   write(fd, &a, 1);
  // }
  tmp = write(fd, woody->p_header,
              (sizeof(Elf64_Phdr) * (woody->header->e_phnum)));
  printf("[ 2] Write: %d\n", tmp);
  tmp = write(fd, woody->my_Pheader, sizeof(Elf64_Phdr));
  printf("[ 3] Write: %d\n", tmp);
  close(fd);
}
