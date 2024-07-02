#include "../includes/woody.h"
#include <elf.h>

void get_elf64_data(t_woody *woody, void *origin_file) {
  get_elf64_header(woody, origin_file);
  get_elf64_pheader(woody, origin_file);
  get_entry_point(woody, origin_file);
}

void get_elf64_header(t_woody *woody, void *origin_file) {
  // Copy Elf Header
  woody->header = (Elf64_Ehdr *)malloc(sizeof(Elf64_Ehdr));
  ft_memcpy(woody->header, origin_file, sizeof(Elf64_Ehdr));
  // print_elf_header(woody->header);
}

void get_elf64_pheader(t_woody *woody, void *origin_file) {
  // Copy Program_Headers
  woody->p_header = (Elf64_Phdr *)malloc(
      (sizeof(Elf64_Phdr) *
       (woody->header->e_phnum + 1))); // +1 to add my Program_Header
  ft_memcpy(woody->p_header, woody->header->e_phoff + origin_file,
            sizeof(Elf64_Phdr) * (woody->header->e_phnum + 1));
  // print_elf64_phdrs(woody->p_header, woody->header->e_phnum + 1);
}

void get_entry_point(t_woody *woody, void *origin_file) {
  // get_origin_entry_point
  (void)woody;
  (void)origin_file;
}
