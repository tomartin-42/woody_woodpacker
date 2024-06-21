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
  printf("==============================\n\n");
}

void print_p_headers(Elf64_Phdr *p_headers, int range) {
  for (int i = 0; i < range; i++) {
    printf("Segmento %d:\n", i);
    printf("  Tipo: %u\n", p_headers[i].p_type);
    printf("  Offset: %lu\n", p_headers[i].p_offset);
    printf("  Dirección Virtual: %lx\n", p_headers[i].p_vaddr);
    printf("  Dirección Física: %lx\n", p_headers[i].p_paddr);
    printf("  Tamaño en Archivo: %lu\n", p_headers[i].p_filesz);
    printf("  Tamaño en Memoria: %lu\n", p_headers[i].p_memsz);
    printf("  Permisos: %u\n", p_headers[i].p_flags);
    printf("  Alineación: %lu\n", p_headers[i].p_align);
    printf("------------------------------\n");
  }
  printf("==============================\n\n");
}

void print_info_p_headers(Elf64_Phdr *p_headers, int range) {
  for (int i = 0; i < range; i++) {
    printf("file %08lx..%08lx | mem %08lx..%08lx | alig %08lx | %i | %x\n",
           p_headers[i].p_offset, p_headers[i].p_filesz, p_headers[i].p_vaddr,
           p_headers[i].p_memsz, p_headers[i].p_align, p_headers[i].p_flags,
           p_headers[i].p_type);
  }
}
