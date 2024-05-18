#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

Elf64_Ehdr *get_elf64_header(int fd) {
  Elf64_Ehdr *header = (Elf64_Ehdr *)malloc(sizeof(Elf64_Ehdr));

  if (header == NULL) {
    close(fd);
    perror("Malloc Fail!!\n");
    return (header);
  }

  if (read(fd, header, sizeof(Elf64_Ehdr)) != (ssize_t)(sizeof(Elf64_Ehdr))) {
    perror("Can not read Header in target file\n");
    free(header);
    close(fd);
    return (NULL);
  }

  return (header);
}

/*
typedef struct {
    Elf64_Word   p_type;    Tipo de segmento
    Elf64_Word   p_flags;   Permisos del segmento
    Elf64_Off    p_offset;  Offset del segmento en el archivo
    Elf64_Addr   p_vaddr;   Dirección virtual del segmento en memoria
    Elf64_Addr   p_paddr;   Dirección física del segmento en memoria (no usada
normalmente) Elf64_Xword  p_filesz;  Tamaño del segmento en el archivo
    Elf64_Xword  p_memsz;   Tamaño del segmento en memoria
    Elf64_Xword  p_align;   Alineación del segmento
} Elf64_Phdr;
*/

Elf64_Phdr *get_target_program_headers(Elf64_Ehdr *header, int fd) {
  Elf64_Phdr *p_headers =
      (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr) * header->e_phnum);

  if (lseek(fd, header->e_shoff, SEEK_SET) == -1) {
    perror("Error seeking origin file");
    free(p_headers);
    free(header);
    close(fd);
    return (NULL);
  }

  if (read(fd, p_headers, sizeof(Elf64_Phdr) * header->e_phnum) !=
      (ssize_t)(sizeof(Elf64_Phdr) * header->e_phnum)) {
    perror("Error in load Program_Headers origin file");
    free(p_headers);
    free(header);
    close(fd);
    return (NULL);
  }

  return (p_headers);
}
