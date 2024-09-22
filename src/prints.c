#include "../includes/woody.h"
#include <elf.h>
#include <stdint.h>

void print_elf_header(Elf64_Ehdr *header) {
  printf("ELF Header:\n");

  // e_ident
  printf("  Magic:   ");
  for (int i = 0; i < EI_NIDENT; i++) {
    printf("%02x ", header->e_ident[i]);
  }
  printf("\n");
  // e_type
  printf("  Type:                             %u\n", header->e_type);
  // e_machine
  printf("  Machine:                          %u\n", header->e_machine);
  // e_version
  printf("  Version:                          %u\n", header->e_version);
  // e_entry
  printf("  Entry point address:              0x%lx\n", header->e_entry);
  // e_phoff
  printf("  Start of program headers:         %lu (bytes into file)\n",
         header->e_phoff);
  // e_shoff
  printf("  Start of section headers:         %lu (bytes into file)\n",
         header->e_shoff);
  // e_flags
  printf("  Flags:                            %u\n", header->e_flags);
  // e_ehsize
  printf("  Size of this header:              %u (bytes)\n", header->e_ehsize);
  // e_phentsize
  printf("  Size of program headers:          %u (bytes)\n",
         header->e_phentsize);
  // e_phnum
  printf("  Number of program headers:        %u\n", header->e_phnum);
  // e_shentsize
  printf("  Size of section headers:          %u (bytes)\n",
         header->e_shentsize);
  // e_shnum
  printf("  Number of section headers:        %u\n", header->e_shnum);
  // e_shstrndx
  printf("  Section header string table index:%u\n", header->e_shstrndx);
}

void print_elf_header_32(Elf32_Ehdr *header) {
  printf("ELF Header:\n");

  // e_ident
  printf("  Magic:   ");
  for (int i = 0; i < EI_NIDENT; i++) {
    printf("%02x ", header->e_ident[i]);
  }
  printf("\n");
  // e_type
  printf("  Type:                             %u\n", header->e_type);
  // e_machine
  printf("  Machine:                          %u\n", header->e_machine);
  // e_version
  printf("  Version:                          %u\n", header->e_version);
  // e_entry
  printf("  Entry point address:              0x%x\n", header->e_entry);
  // e_phoff
  printf("  Start of program headers:         %u (bytes into file)\n",
         header->e_phoff);
  // e_shoff
  printf("  Start of section headers:         %u (bytes into file)\n",
         header->e_shoff);
  // e_flags
  printf("  Flags:                            %u\n", header->e_flags);
  // e_ehsize
  printf("  Size of this header:              %u (bytes)\n", header->e_ehsize);
  // e_phentsize
  printf("  Size of program headers:          %u (bytes)\n",
         header->e_phentsize);
  // e_phnum
  printf("  Number of program headers:        %u\n", header->e_phnum);
  // e_shentsize
  printf("  Size of section headers:          %u (bytes)\n",
         header->e_shentsize);
  // e_shnum
  printf("  Number of section headers:        %u\n", header->e_shnum);
  // e_shstrndx
  printf("  Section header string table index:%u\n", header->e_shstrndx);
}

void print_elf64_phdrs(const Elf64_Phdr *p_header, int phnum) {
  printf("Program Headers:\n");

  for (int i = 0; i < phnum; i++) {
    printf("  Program Header %d:\n", i);
    printf("    Type:           %u\n", p_header[i].p_type);
    printf("    Flags:          0x%x\n", p_header[i].p_flags);
    printf("    Offset:         0x%lx\n", p_header[i].p_offset);
    printf("    Virtual Addr:   0x%lx\n", p_header[i].p_vaddr);
    printf("    Physical Addr:  0x%lx\n", p_header[i].p_paddr);
    printf("    File Size:      %lu bytes\n", p_header[i].p_filesz);
    printf("    Mem Size:       %lu bytes\n", p_header[i].p_memsz);
    printf("    Alignment:      %lu\n", p_header[i].p_align);
  }
}

void print_elf32_phdrs(const Elf32_Phdr *p_header, int phnum) {
  printf("Program Headers:\n");

  for (int i = 0; i < phnum; i++) {
    printf("  Program Header %d:\n", i);
    printf("    Type:           %u\n", p_header[i].p_type);
    printf("    Flags:          0x%x\n", p_header[i].p_flags);
    printf("    Offset:         0x%x\n", p_header[i].p_offset);
    printf("    Virtual Addr:   0x%x\n", p_header[i].p_vaddr);
    printf("    Physical Addr:  0x%x\n", p_header[i].p_paddr);
    printf("    File Size:      %u bytes\n", p_header[i].p_filesz);
    printf("    Mem Size:       %u bytes\n", p_header[i].p_memsz);
    printf("    Alignment:      %u\n", p_header[i].p_align);
  }
}

void print_key(char *key, uint64_t key_size) {
  printf("KEY: ");
  for (uint64_t i = 0; i < key_size; i++) {
    printf("%c", key[i]);
  }
  printf("\n");
}
