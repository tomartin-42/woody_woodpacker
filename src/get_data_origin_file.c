#include "../includes/woody.h"
#include <elf.h>
#include <sys/types.h>

void get_elf64_data(t_woody *woody, void *origin_file, ssize_t origin_len) {
  get_elf64_header(woody, origin_file, origin_len);
  get_elf64_pheader(woody, origin_file, origin_len);
  get_elf64_sheader(woody, origin_file, origin_len);
  get_text_section(woody, origin_file, origin_len);
  get_origin_entry_point(woody);
  reserve_memory_to_my_file(woody, origin_file, origin_len);
}

// Copy Elf Header
void get_elf64_header(t_woody *woody, void *origin_file, ssize_t origin_len) {
  woody->header = (Elf64_Ehdr *)malloc(sizeof(Elf64_Ehdr));
  if (woody->header == NULL) {
    launch_headers_error(MALLOC_FAIL, woody, origin_file, origin_len);
  }
  ft_memcpy(woody->header, origin_file, sizeof(Elf64_Ehdr));
}

// Copy Program_Headers
void get_elf64_pheader(t_woody *woody, void *origin_file, ssize_t origin_len) {
  woody->p_header = (Elf64_Phdr *)malloc(
      (sizeof(Elf64_Phdr) *
       (woody->header->e_phnum + 1))); // +1 to add my Program_Header
  if (woody->p_header == NULL) {
    launch_headers_error(MALLOC_FAIL, woody, origin_file, origin_len);
  }
  ft_memcpy(woody->p_header, woody->header->e_phoff + origin_file,
            sizeof(Elf64_Phdr) * (woody->header->e_phnum + 1));
}

// Copy Section_Headers
void get_elf64_sheader(t_woody *woody, void *origin_file, ssize_t origin_len) {
  woody->s_header =
      (Elf64_Shdr *)malloc((sizeof(Elf64_Shdr) * (woody->header->e_shnum)));
  if (woody->s_header == NULL) {
    launch_headers_error(MALLOC_FAIL, woody, origin_file, origin_len);
  }
  ft_memcpy(woody->s_header, woody->header->e_shoff + origin_file,
            sizeof(Elf64_Shdr) * (woody->header->e_shnum));
}

void get_text_section(t_woody *woody, void *origin_file, ssize_t origin_len) {
  char check = 0;
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
    launch_headers_error(NOT_TEXT_SECTION, woody, origin_file, origin_len);
    exit(42);
  }
}

// get_origin_entry_point
void get_origin_entry_point(t_woody *woody) {
  woody->origin_entry = woody->header->e_entry;
}

Elf64_Addr get_max_paddr(t_woody *woody) {
  Elf64_Addr highest_paddr = 0;

  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_offset + woody->p_header[i].p_filesz >
        highest_paddr) {
      highest_paddr = woody->p_header[i].p_offset + woody->p_header[i].p_filesz;
    }
  }
  return (highest_paddr);
}

Elf64_Addr get_max_vaddr(t_woody *woody) {
  Elf64_Addr highest_vaddr = 0;

  for (int i = 0; i < woody->header->e_phnum; i++) {
    if (woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz >
        highest_vaddr) {
      highest_vaddr = woody->p_header[i].p_vaddr + woody->p_header[i].p_memsz;
    }
  }
  return (highest_vaddr);
}

unsigned int calculate_padding(t_woody *woody, ssize_t origin_len) {
  Elf64_Addr highest_paddr = get_max_paddr(woody);

  unsigned int padding = ((highest_paddr + 0xfff) & ~0xfff) - origin_len;
  return (padding);
}

void calculate_my_size_file(t_woody *woody, ssize_t origin_len) {
  size_t size = 0;
  unsigned int padding = calculate_padding(woody, origin_len);
  woody->padding = padding;

  size += origin_len; // Origin file leng
  size += padding;
  size += ((woody->header->e_phnum + 1) *
           (woody->header->e_phentsize)); // p_header leng
  size += PAYLOAD_LEN;

  woody->file_size = size;
}

void reserve_memory_to_my_file(t_woody *woody, void *origin_file,
                               ssize_t origin_len) {
  calculate_my_size_file(woody, origin_len);
  woody->file = malloc(woody->file_size);
  if (woody->file == NULL) {
    launch_headers_error(MALLOC_FAIL, woody, origin_file, origin_len);
  }
}
