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

int check_elf64_origin_file(int fd) {
  Elf64_Ehdr header = {0};
  ssize_t i;
  i = read(fd, &header, sizeof(header));
  return (i);
}
