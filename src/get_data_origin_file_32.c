#include "../includes/woody.h"
#include <elf.h>
#include <sys/types.h>

void get_elf32_data(t_woody_32 *woody, void *origin_file, ssize_t origin_len) {
  get_elf32_header(woody, origin_file);
  get_elf32_pheader(woody, origin_file);
  get_elf32_sheader(woody, origin_file);
  get_origin_entry_point_32(woody);
  reserve_memory_to_my_file_32(woody, origin_file, origin_len);
}

void get_elf32_header(t_woody_32 *woody, void *origin_file) {
  // Copy Elf Header
  woody->header = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
  ft_memcpy(woody->header, origin_file, sizeof(Elf32_Ehdr));
  /* print_elf_header_32(woody->header); */
}

void get_elf32_pheader(t_woody_32 *woody, void *origin_file) {
  // Copy Program_Headers
  woody->p_header = (Elf32_Phdr *)malloc(
      (sizeof(Elf32_Phdr) *
       (woody->header->e_phnum + 1))); // +1 to add my Program_Header
  ft_memcpy(woody->p_header, woody->header->e_phoff + origin_file,
            sizeof(Elf32_Phdr) * (woody->header->e_phnum + 1));
  /* print_elf32_phdrs(woody->p_header, woody->header->e_phnum + 1); */
}

void get_elf32_sheader(t_woody_32 *woody, void *origin_file) {
  char check = 0;

  woody->s_header =
      (Elf32_Shdr *)malloc((sizeof(Elf32_Shdr) * (woody->header->e_shnum)));
  ft_memcpy(woody->s_header, woody->header->e_shoff + origin_file,
            sizeof(Elf32_Shdr) * (woody->header->e_shnum));

  const char *shstrtab =
      woody->s_header[woody->header->e_shstrndx].sh_offset + origin_file;

  for (int i = 0; i < woody->header->e_shnum; i++) {
    if (!ft_strncmp(shstrtab + woody->s_header[i].sh_name, ".text", 5)) {
      check = 1;
      woody->text_off = woody->s_header[i].sh_offset;
      woody->text_size = woody->s_header[i].sh_size;
      break;
    }
  }
  if (check == 0) {
    // error not .text secction
    exit(42);
  }
}

void get_origin_entry_point_32(t_woody_32 *woody) {
  // get_origin_entry_point
  woody->origin_entry = woody->header->e_entry;
}

Elf32_Addr get_max_paddr_32(t_woody_32 *woody) {
  Elf32_Addr highest_paddr = 0;

  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_offset + woody->p_header[i].p_filesz >
        highest_paddr) {
      highest_paddr = woody->p_header[i].p_offset + woody->p_header[i].p_filesz;
    }
  }
  return (highest_paddr);
}

Elf32_Addr get_max_vaddr_32(t_woody_32 *woody) {
  Elf32_Addr highest_vaddr = 0;

  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz >
        highest_vaddr) {
      highest_vaddr = woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz;
    }
  }
  return (highest_vaddr);
}

unsigned int calculate_padding_32(t_woody_32 *woody, ssize_t origin_len) {
  Elf32_Addr highest_paddr = get_max_paddr_32(woody);

  unsigned int padding = ((highest_paddr + 0xfff) & ~0xfff) - origin_len;
  return (padding);
}

void calculate_my_size_file_32(t_woody_32 *woody, ssize_t origin_len) {
  size_t size = 0;
  unsigned int padding = calculate_padding_32(woody, origin_len);
  woody->padding = padding;

  size += origin_len; // Origin file leng
  size += padding;
  // printf("PADDING: %d\n", woody->padding);
  size += ((woody->header->e_phnum + 1) *
           (woody->header->e_phentsize)); // p_header leng
  size += PAYLOAD_LEN;

  woody->file_size = size;
  // printf("My Buffer Len: %ld\n", woody->file_size);
}

void reserve_memory_to_my_file_32(t_woody_32 *woody, void *origin_file,
                                  ssize_t origin_len) {
  calculate_my_size_file_32(woody, origin_len);
  woody->file = malloc(woody->file_size);
  if (woody->file == NULL) {
    launch_error(MALLOC_FAIL, origin_file, origin_len);
  }
}