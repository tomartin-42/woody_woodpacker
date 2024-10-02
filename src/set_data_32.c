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
  Elf32_Addr highest_paddr = get_max_paddr_32(woody);

  woody->padding = calculate_padding_32(woody, origin_len);
  woody->my_Pheader = (Elf32_Phdr *)malloc(sizeof(Elf32_Phdr));
  woody->my_Pheader->p_type = PT_LOAD;
  woody->my_Pheader->p_offset = (highest_paddr + 0xfff) & ~0xfff;
  woody->my_Pheader->p_vaddr = (highest_vaddr + 0xfff) & ~0xfff;
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
      "\x54\x50\x53\x51\x52\xe8\x00\x00\x00\x00\x5e\x83\xee\x0a\x52\x89\xf1\x83"
      "\xc1\x69\xbb\x01\x00\x00\x00\xba\x0e\x00\x00\x00\xb8\x04\x00\x00\x00\xcd"
      "\x80\x5a\x8d\x86\x9b\x00\x00\x00\x89\xf7\x2b\x38\x8d\x86\x9f\x00\x00\x00"
      "\x8b\x00\x8d\x56\x77\x8d\x8e\x97\x00\x00\x00\x8b\x09\x31\xdb\x50\x8a\x07"
      "\x32\x04\x1a\x88\x07\x47\x43\x39\xcb\x7c\x02\x31\xdb\x58\x48\x75\xec\x8d"
      "\x86\xa3\x00\x00\x00\x2b\x30\x5a\x59\x5b\x58\x5c\xff\xd6\xc3\x2e\x2e\x2e"
      "\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x43\x43\x43\x43\x43\x43\x43"
      "\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43"
      "\x43\x43\x43\x43\x43\x43\x43\x5a\x5a\x5a\x5a\x4f\x4f\x4f\x4f\x53\x53\x53"
      "\x53\x44\x44\x44\x44";

  ft_memcpy(woody->file + count, code, (sizeof(code)));
  count += (sizeof(code) / sizeof(code[0]));
  // change entry
  Elf32_Ehdr *tmp = (Elf32_Ehdr *)woody->file;
  woody->origin_entry = tmp->e_entry;
  tmp->e_entry = woody->my_entry;
  woody->entry_distance = (woody->my_entry - woody->origin_entry);
  // .text offset to decrypt.
  woody->text_dist = (woody->my_entry - woody->text_off);
  return (count);
}

void patch_data_32(t_woody_32 *woody, ssize_t count) {
  ft_memcpy(((woody->file + count) - 5), (void *)&woody->entry_distance, 4);
  ft_memcpy(((woody->file + count) - 9), &woody->text_size, 4);
  ft_memcpy(((woody->file + count) - 13), &woody->text_dist, 4);
  ft_memcpy(((woody->file + count) - 17), &woody->key_size, 4);
  ft_memcpy(((woody->file + count) - 49), woody->key, woody->key_size);
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
