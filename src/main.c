#include "../includes/woody.h"
#include "libft.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

// Generación de clave.
// Genero directamente de longitud 64 y luego la manejo con key_size
static int generate_key(unsigned char *key) {
  ssize_t bytes_read;
  int fd;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    return (0);
  }

  bytes_read = read(fd, key, 64);
  close(fd);

  if (bytes_read != 64) {
    return (0);
  }

  return (1);
}

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
  size_t key_size = 32;
  unsigned char key[64];
  struct s_elf_info elf_info = {0};
  struct s_cave_info cave_info = {0};

  // Comprobaciones
  if (argc < 2 || argc > 3) {
    write(2, "Error: Invalid option format\n", 29);
    write(2, "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n",
          63);
    exit(EXIT_FAILURE);
  }

  // Validación 2do arg
  if (argc == 3) {
    key_size = validate_second_arg(argv[2]);
    if (key_size == 0) {
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

  // Validaciones y finder segmento .text
  if (!validate_text_segment(origin_file, origin_len, &elf_info)) {
    write(2, "Error: Invalid section .text\n", 29);
    munmap(origin_file, origin_len);
    exit(EXIT_FAILURE);
  }

  if (!generate_key(key)) {
    write(2, "Error: Can not generate encrypt key\n", 36);
    munmap(origin_file, origin_len);
    exit(EXIT_FAILURE);
  }

  if (elf_info.elf_class == WOODY_ELF64) {
    if (!generate_cave64(origin_file, origin_len, &elf_info, &cave_info, key,
                         key_size)) {
      write(2, "Error: Can not generate cave\n", 29);
      munmap(origin_file, origin_len);
      exit(EXIT_FAILURE);
    }
  } else {
    // 32bits
  }

  if (!write_woody(origin_file, origin_len, &cave_info)) {
    write(2, "Error: Can write woody\n", 23);
    free(cave_info.payload);
    free(cave_info.new_phdrs);
    munmap(origin_file, origin_len);
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
