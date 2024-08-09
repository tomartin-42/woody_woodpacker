#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main_set_data(t_woody *woody, void *origin_file, ssize_t origin_len) {
  init_my_Pheader(woody, origin_len);
  mod_origin_header(woody, origin_file);
  mod_phdr(woody, origin_len);
}

void init_my_Pheader(t_woody *woody, ssize_t origin_len) {
  Elf64_Addr highest_vaddr = get_max_vaddr(woody);
  // printf("HIGH vaddr: 0x%lx\n", highest_vaddr);
  Elf64_Addr highest_paddr = get_max_paddr(woody);
  // printf("HIGH paddr: 0x%lx\n", highest_paddr);

  woody->padding = calculate_padding(woody, origin_len);
  woody->my_Pheader = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr));
  woody->my_Pheader->p_type = PT_LOAD;
  woody->my_Pheader->p_offset = (highest_paddr + 0xfff) & ~0xfff;
  // printf("[!] Offset My New Section: 0x%lx\n", woody->my_Pheader->p_offset);
  woody->my_Pheader->p_vaddr = (highest_vaddr + 0xfff) & ~0xfff;
  // printf("[!] V_addr My New Section: 0x%lx\n", woody->my_Pheader->p_vaddr);
  woody->my_Pheader->p_paddr = (highest_vaddr + 0xfff) & ~0xfff;
  woody->my_Pheader->p_filesz = 0x5000;
  woody->my_Pheader->p_memsz = 0x5000;
  woody->my_Pheader->p_flags = PF_X | PF_W | PF_R;
  woody->my_Pheader->p_align = 0x1000;
}

void mod_origin_header(t_woody *woody, void *origin_file) {
  Elf64_Ehdr *tmp_header = (Elf64_Ehdr *)origin_file;

  tmp_header->e_phnum++;
  tmp_header->e_phoff = woody->my_Pheader->p_offset;
}

void mod_phdr(t_woody *woody, ssize_t origin_len) {

  woody->padding = calculate_padding(woody, origin_len);
  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_type == PT_PHDR) {
      woody->p_header[i].p_offset = woody->my_Pheader->p_offset;
      woody->p_header[i].p_vaddr = woody->my_Pheader->p_vaddr;
      woody->p_header[i].p_paddr = woody->my_Pheader->p_vaddr;
      woody->p_header[i].p_memsz += 256;
      woody->p_header[i].p_filesz += 256;
      break;
    }
  }
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
  // My_p_header
  ft_memcpy(woody->file + count, woody->my_Pheader, sizeof(Elf64_Phdr));
  count += sizeof(Elf64_Phdr);

  char code[] =
      "\x52\x48\x8d\x1d\xf8\xff\xff\xff\xbf\x01\x00\x00\x00\x48\x8d\x35\x19\x00"
      "\x00\x00\xba\x0a\x00\x00\x00\xb8\x01\x00\x00\x00\x0f\x05\x5a\x48\x8d\x05"
      "\x0f\x00\x00\x00\x48\x2b\x18\xff\xe3\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e"
      "\x0a\x90\xd3\x01\x00\x00\x00\x00";

  // printf("init shellcode: 0x%lx\n", woody->my_entry);
  ft_memcpy(woody->file + count, code, (sizeof(code) / sizeof(code[0])));
  count += (sizeof(code) / sizeof(code[0]));
  // count += PAYLOAD_LEN;
  // change entry
  Elf64_Ehdr *tmp = (Elf64_Ehdr *)woody->file;
  woody->origin_entry = tmp->e_entry;
  tmp->e_entry = woody->my_entry;
  woody->entry_distance = (woody->my_entry - woody->origin_entry);
  // Patch origin_entry to return addr
  ft_memcpy(((woody->file + count) - 8), (void *)&woody->entry_distance, 8);

  // printf("Origin entry: 0x%lx\n", woody->origin_entry);
  // printf("Entry distance: 0x%lx\n", woody->entry_distance);
  // printf("Entry distance calculate: 0x%lx\n",
  //        woody->my_entry - woody->entry_distance);

  fd = open("woody2", O_WRONLY | O_CREAT, 0777);
  if (write(fd, woody->file, woody->file_size) != (long int)woody->file_size) {
    launch_error(WRITE_FAIL, origin_file, origin_len);
  }
  close(fd);
  clean_up(woody, origin_file, origin_len);
}
