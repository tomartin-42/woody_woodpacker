#include "../includes/woody.h"
#include "libft.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

// Validador flag de parametro de encriptación
static int validate_second_arg(char *arg) {
  if (ft_strncmp(arg, "-8", 3) == 0) {
    return (8);
  }
  if (ft_strncmp(arg, "-16", 4) == 0)
    return (16);
  if (ft_strncmp(arg, "-32", 4) == 0)
    return (32);
  if (ft_strncmp(arg, "-64", 4) == 0)
    return (64);
  return (0);
}

int main(int argc, char **argv) {

  unsigned char *origin_file = NULL;
  off_t origin_len;
  int fd;
  struct s_elf_info elf_info = {0};

  // Comprobaciones
  if (argc < 2 || argc > 3) {
    printf("N ARG\n");
    write(2, "Error: Invalid option format\n", 29);
    write(2, "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n",
          63);
    exit(EXIT_FAILURE);
  }

  // Validación 2do arg
  if (argc == 3) {
    if (validate_second_arg(argv[2]) == 0) {
      write(2, "Error: Invalid option format\n", 29);
      write(2,
            "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n",
            63);
      exit(EXIT_FAILURE);
    }
  }

  if ((fd = open(argv[1], O_RDONLY)) == -1) {
    perror("Can not open target file");
    exit(EXIT_FAILURE);
  }

  // Otención de la longitud del archivo original
  origin_len = lseek(fd, 0, SEEK_END);
  if (origin_len == -1) {
    close(fd);
    perror("lseek() error");
    exit(EXIT_FAILURE);
  }

  // Comprobación para que el archivo original tenga garantizado
  // como tamáño mínimo una cabecera Elf32 y no de fallo al intentar
  // leer la cabecera.
  if (origin_len < (off_t)sizeof(Elf32_Ehdr)) {
    write(2, "Error: Invalid file size\n", 25);
    close(fd);
    exit(EXIT_FAILURE);
  }

  // Mapeo del archivo original al buffer origin_file
  // No se ve afectado el archivo original por la flag MAP_PRIVATE
  origin_file =
      mmap(NULL, origin_len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

  if (origin_file == MAP_FAILED) {
    close(fd);
    perror("mmap() error");
    exit(EXIT_FAILURE);
  }

  close(fd);

  // Validaciones para la integridad y definición del Elf
  if (!validate_elf(origin_file, origin_len, &elf_info)) {
    write(2, "Error: Invalid Elf format\n", 26);
    munmap(origin_file, origin_len);
    exit(EXIT_FAILURE);
  }

  if (!validate_text_segment(origin_file, origin_len, &elf_info)) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
