#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

// Validador flag de parametro de encriptación
static int validate_second_arg(char **argv) {
  int flag_value;

  flag_value = atoi(&argv[2][1]);

  if (flag_value != 8 && flag_value != 16 && flag_value != 32 &&
      flag_value != 64) {
    flag_value = 0;
  }
  return flag_value;
}

int main(int argc, char **argv) {

  void *origin_file = NULL;
  off_t origin_len;
  int fd;
  struct s_elf_info *elf_info = {0};

  // Comprobaciones
  if (argc < 2 || argc > 3) {
    write(2, "Error: Invalid option format\n", 29);
    write(2, "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n",
          63);
    exit(EXIT_FAILURE);
  }

  if (argc == 3 && (argv[2][0] != '-' || !validate_second_arg(argv))) {
    write(2, "Error: Invalid option format\n", 29);
    write(2, "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n",
          63);
    exit(EXIT_FAILURE);
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

  validate_elf(origin_file, origin_len, elf_info);
}
