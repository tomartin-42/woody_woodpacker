#include "../includes/woody.h"

// int check_the_origin_file(char *file) {
//   int fd;
//   fd = open(file, O_RDONLY);
//   if (fd == -1) {
//     perror("Can not open the source file");
//     return (fd);
//   }
//   return (fd);
// }

/*
typedef struct {
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
} Elf64_Ehdr;
*/

// This functon comprobate the header of imput file
// return -1 if error
// return 1 if ELF64
// return 2 if ELF32
int check_origin_elf(uint8_t *origin_file, size_t origin_len) {
  int ret_value;

  if (ft_memcmp(origin_file, ELFMAG, SELFMAG)) {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }

  if (origin_file[EI_CLASS] == ELFCLASS64 ||
      origin_file[EI_CLASS] == ELFCLASS32) {
    ret_value = origin_file[EI_CLASS];
  } else {
    launch_error(NOT_ELF_ERROR, origin_file, origin_len);
  }
  return (ret_value);
}

// if (i != 64) { // The file don't have correct header
//   return (-1);
// }

//   type = ft_memcmp(header.e_ident, elf, 4);
//   if (type != 0) { // The file itś not a ELF file
//     perror("The file in not a elf type\n");
//     return (-1);
//   }
//   if (header.e_ident[EI_CLASS] == ELFCLASS64) { // Te file is ELF64
//     return (1);
//   }
//   if (header.e_ident[EI_CLASS] == ELFCLASS32) { // The file in ELF32
//     return (2);
//   }
//   return (-1);
// }
//
// int main_check_input_file(char *file) {
//   int fd = check_the_origin_file(file);
//   int elftype;
//
//   if (fd < 0) {
//     return (-1);
//   }
//   elftype = check_elf64_origin_file(fd);
//   if (elftype == -1) {
//     close(fd);
//     return (-1);
//   }
//   close(fd);
//   return (elftype);
// }
