#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   
#include <unistd.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

// Validador flag de parametro de encriptación
static int validate_second_arg(char **argv) {
  int flag_value;

  flag_value = atoi(&argv[2][1]);

  if (flag_value != 8 && flag_value != 16 
      && flag_value != 32 && flag_value != 64) {
    flag_value = 0;
  }
  return flag_value;
}

int main(int argc, char **argv) {

  void *origin_file; 
  ssize_t origin_len;
  int fd;
  
  // Comprobaciones
  if (argc < 2 || argc > 3) {
    write(2, "Error: Invalid option format\n", 29);
    write(2, "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n", 63);
    exit(EXIT_FAILURE);
  }

  if (argc == 3 && (argv[2][0] != '-' || !validate_second_arg(argv))) {
    write(2, "Error: Invalid option format\n", 29);
    write(2, "Usage: ./woody_woodpacker <target_file> [-8 | -16 | -32 | -64]\n", 63);
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

  origin_file =
      mmap(NULL, origin_len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

  if (origin_file == MAP_FAILED) {
    close(fd);
    perror("mmap() error");
    exit(EXIT_FAILURE);
  }
  close(fd);
}
