// In this file going the print functions
#include "../includes/woody.h"
#include <elf.h>
#include <stdio.h>

void print_header(Elf64_Ehdr *header) {
  // Imprimir e_ident
  printf("e_ident: ");
  for (int i = 0; i < EI_NIDENT; i++) {
    printf("%02x ", header->e_ident[i]);
  }
  printf("\n");

  // Imprimir e_type
  printf("e_type: %u\n", header->e_type);

  // Imprimir e_machine
  printf("e_machine: %u\n", header->e_machine);

  // Imprimir e_version
  printf("e_version: %u\n", header->e_version);

  // Imprimir e_entry
  printf("e_entry: %lx\n", header->e_entry);

  // Imprimir e_phoff
  printf("e_phoff: %lx\n", header->e_phoff);

  // Imprimir e_shoff
  printf("e_shoff: %lx\n", header->e_shoff);

  // Imprimir e_flags
  printf("e_flags: %u\n", header->e_flags);

  // Imprimir e_ehsize
  printf("e_ehsize: %u\n", header->e_ehsize);

  // Imprimir e_phentsize
  printf("e_phentsize: %u\n", header->e_phentsize);

  // Imprimir e_phnum
  printf("e_phnum: %u\n", header->e_phnum);

  // Imprimir e_shentsize
  printf("e_shentsize: %u\n", header->e_shentsize);

  // Imprimir e_shnum
  printf("e_shnum: %u\n", header->e_shnum);

  // Imprimir e_shstrndx
  printf("e_shstrndx: %u\n", header->e_shstrndx);
}
