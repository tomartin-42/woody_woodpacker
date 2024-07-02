#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main_set_data(t_woody *woody, void *origin_file, ssize_t origin_len) {
  init_my_Pheader(woody);
  mod_origin_header(woody, origin_file);
  mod_phdr(woody, origin_len);
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

void init_my_Pheader(t_woody *woody) {
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

void mod_origin_header(t_woody *woody, void *origin_file) {
  Elf64_Ehdr *tmp_header = (Elf64_Ehdr *)origin_file;

  tmp_header->e_phnum++;
  tmp_header->e_phoff = woody->my_Pheader->p_offset;
}

void mod_phdr(t_woody *woody, ssize_t origin_len) {
  Elf64_Addr highest_vaddr = get_max_add(woody);

  woody->padding = ((highest_vaddr + 0xfff) & ~0xfff) - origin_len;
  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_type == PT_PHDR) {
      woody->p_header[i].p_offset = woody->my_Pheader->p_offset;
      woody->p_header[i].p_vaddr = woody->my_Pheader->p_offset;
      woody->p_header[i].p_paddr = woody->my_Pheader->p_offset;
      woody->p_header[i].p_memsz += 54;
      woody->p_header[i].p_filesz += 54;
      break;
    }
  }
}
