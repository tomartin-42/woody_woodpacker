#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main_set_data_32(t_woody_32 *woody, void *origin_file,
                      ssize_t origin_len) {
  ssize_t count = 0;
  init_my_Pheader_32(woody, origin_len);
  mod_origin_header_32(woody, origin_file);
  mod_phdr_32(woody, origin_len);
  mod_phdr_text_section_32(woody);
  count = put_data_in_buffer_32(woody, origin_file, origin_len);
  main_encrypt_32(woody);
  patch_data_32(woody, count);
}

void init_my_Pheader_32(t_woody_32 *woody, ssize_t origin_len) {
  Elf32_Addr highest_vaddr = get_max_vaddr_32(woody);
  // printf("HIGH vaddr: 0x%lx\n", highest_vaddr);
  Elf32_Addr highest_paddr = get_max_paddr_32(woody);
  // printf("HIGH paddr: 0x%lx\n", highest_paddr);

  woody->padding = calculate_padding_32(woody, origin_len);
  woody->my_Pheader = (Elf32_Phdr *)malloc(sizeof(Elf32_Phdr));
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

void mod_origin_header_32(t_woody_32 *woody, void *origin_file) {
  Elf32_Ehdr *tmp_header = (Elf32_Ehdr *)origin_file;

  tmp_header->e_phnum++;
  tmp_header->e_phoff = woody->my_Pheader->p_offset;
}

void mod_phdr_32(t_woody_32 *woody, ssize_t origin_len) {

  woody->padding = calculate_padding_32(woody, origin_len);
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

ssize_t put_data_in_buffer_32(t_woody_32 *woody, void *origin_file,
                              ssize_t origin_len) {
  ssize_t count = 0;
  size_t enty_off = 0;

  // Origin file
  ft_memcpy(woody->file, origin_file, origin_len);
  count = origin_len;
  // Padding
  ft_memset(woody->file + count, 43, woody->padding);
  count += woody->padding;
  // P_headers
  ft_memcpy(woody->file + count, woody->p_header,
            (sizeof(Elf32_Phdr) * (woody->header->e_phnum)));
  count += (sizeof(Elf32_Phdr) * (woody->header->e_phnum));
  enty_off = count + sizeof(Elf32_Phdr);
  woody->my_entry =
      woody->my_Pheader->p_vaddr + (enty_off - woody->my_Pheader->p_offset);
  // My_p_header
  ft_memcpy(woody->file + count, woody->my_Pheader, sizeof(Elf32_Phdr));
  count += sizeof(Elf32_Phdr);
  char code[] =
      "\x52\x8d\x1d\x00\x90\x04\x08\xbf\x01\x00\x00\x00\x8d\x35\x67\x90\x04\x08"
      "\xba\x0e\x00\x00\x00\xb8\x04\x00\x00\x00\x0f\x05\x5a\x50\x53\x56\x52\x51"
      "\x8d\x05\x9d\x90\x04\x08\x2b\x18\x89\xdf\x8d\x35\xa5\x90\x04\x08\x8b\x36"
      "\x8d\x15\x75\x90\x04\x08\x8d\x0d\x95\x90\x04\x08\x8b\x09\x31\xdb\x8a\x07"
      "\x32\x04\x1a\x88\x07\x47\x43\x39\xcb\x7c\x02\x31\xdb\x4e\x75\xee\x59\x5a"
      "\x5e\x5b\x58\x8d\x05\xad\x90\x04\x08\x2b\x18\xff\xe3\x2e\x2e\x2e\x2e\x57"
      "\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x43\x43\x43\x43\x43\x43\x43\x43\x43"
      "\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43"
      "\x43\x43\x43\x43\x43\x5a\x5a\x5a\x5a\x5a\x5a\x5a\x5a\x4f\x4f\x4f\x4f\x4f"
      "\x4f\x4f\x4f\x53\x53\x53\x53\x53\x53\x53\x53\x44\x44\x44\x44\x44\x44\x44"
      "\x44";

  // printf("init shellcode: 0x%lx\n", woody->my_entry);
  ft_memcpy(woody->file + count, code, (sizeof(code) / sizeof(code[0])));
  count += (sizeof(code) / sizeof(code[0]));
  // count += PAYLOAD_LEN;
  // change entry
  Elf32_Ehdr *tmp = (Elf32_Ehdr *)woody->file;
  woody->origin_entry = tmp->e_entry;
  tmp->e_entry = woody->my_entry;
  woody->entry_distance = (woody->my_entry - woody->origin_entry);
  // .text offset to decrypt.
  woody->text_dist = (woody->my_entry - woody->text_off);
  // woody->file + cound = end_of_file
  // Patch origin_entry to return addr
  // printf("Origin entry: 0x%lx\n", woody->origin_entry);
  /* printf("Entry distance: 0x%lx\n", woody->entry_distance); */
  /* printf("Entry distance calculate: 0x%lx\n", */
  /*        woody->my_entry - woody->entry_distance); */
  return (count);
}

void patch_data_32(t_woody_32 *woody, ssize_t count) {
  // char *tmp = woody->key;
  (void)count;
  printf("Entry distance: 0x%lx\n", woody->entry_distance);
  ft_memcpy(((woody->file + count) - 9), (void *)&woody->entry_distance, 8);
  /* ft_memcpy(((woody->file + count) - 17), &woody->text_size, 8); */
  /* ft_memcpy(((woody->file + count) - 25), &woody->text_dist, 8); */
  /* ft_memcpy(((woody->file + count) - 33), &woody->key_size, 8); */
  /* ft_memcpy(((woody->file + count) - 97), woody->key, woody->key_size); */
}

// Found and change section p_flags to can encrypt/decrypt .text secction
void mod_phdr_text_section_32(t_woody_32 *woody) {
  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_type == PT_LOAD) {
      if (woody->p_header[i].p_vaddr <= woody->text_off &&
          woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz >=
              woody->text_off) {
        woody->p_header[i].p_flags = 7;
      }
    }
  }
}
