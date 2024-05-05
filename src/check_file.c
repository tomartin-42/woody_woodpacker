#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int check_the_origin_file(char *file) {
  int fd;
  fd = open(file, O_RDONLY);
  if (fd == -1) {
    perror("Can not open the source file");
    return (fd);
  }
  return (fd);
}

/*
    unsigned char e_ident[EI_NIDENT]; Identificación de archivo ELF
    Elf64_Half    e_type;             Tipo de archivo
    Elf64_Half    e_machine;          Arquitectura del procesador
    Elf64_Word    e_version;          Versión del archivo ELF
    Elf64_Addr    e_entry;            Punto de entrada del programa
    Elf64_Off     e_phoff;            Desplazamiento del encabezado del programa
    Elf64_Off     e_shoff;            Desplazamiento del encabezado de sección
    Elf64_Word    e_flags;            Flags específicos del procesador
    Elf64_Half    e_ehsize;           Tamaño del encabezado ELF
    Elf64_Half    e_phentsize;        Tamaño de la entrada del encabezado del
   programa
   Elf64_Half    e_phnum;            Número de entradas del encabezado
   del programa
   Elf64_Half    e_shentsize;        Tamaño de la entrada del
   encabezado de sección
   Elf64_Half    e_shnum;            Número de entradas
   del encabezado de sección
   Elf64_Half    e_shstrndx;         Índice de la
   tabla de cadenas del encabezado de sección
*/

// This functon comprobate the header of imput file
// return -1 if error
// return 1 if ELF64
// return 2 if ELF32
int check_elf64_origin_file(int fd) {
  Elf64_Ehdr header = {0};
  char elf[4] = {0x7f, 0x45, 0x4c, 0x46}; //"7f 45 4c 46" -> ELF
  ssize_t i;
  int type;

  i = read(fd, &header, sizeof(header));

  if (i != 64) { // The file don't have correct header
    return (-1);
  }

  type = ft_memcmp(header.e_ident, elf, 4);
  if (type != 0) { // The file itś not a ELF file
    return (-1);
  }

  if (header.e_ident[EI_CLASS] == ELFCLASS64) { // Te file is ELF64
    return (1);
  }
  if (header.e_ident[EI_CLASS] == ELFCLASS32) { // The file in ELF32
    return (2);
  }
  return (-1);
}
