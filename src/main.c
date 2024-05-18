#include "../includes/woody.h"
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

const char *instructions =
    "\xBF\x01\x00\x00\x00\x48\xBE\x00\x20\x40\x00\x00\x00\x00\x00\xBA\x0D\x00"
    "\x00\x00\xB8\x01\x00\x00\x00\x0F\x05\x48\x31\xFF\xB8\x3C\x00\x00\x00\x0F"
    "\x05";

void run_instructions() {
  // Cogemos el offset donde está guardado instructions
  size_t region = (size_t)instructions;
  region = region & (~0xFFF); // Igualamos los 3 ultimos bytes a 0
  int ret = mprotect((void *)region, 0x1000, PROT_READ | PROT_EXEC);
  if (ret != 0) {
    printf("Error in mprotect %d\n", errno);
  }
  void (*f)(void) = (void *)instructions; // declaramos un puntero a funcion
                                          // que apunta a instructions
  printf("[!] Iniciamos instructions\n");
  f();
  printf("finalizamos instructions\n");
}

int main(int argc, char **argv) {
  int check;
  int fd;
  Elf64_Ehdr *header = NULL;
  Elf64_Phdr *p_headers = NULL;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }
  check = main_check_input_file(argv[1]);
  if (check < 0) {
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("Can not open target file\n");
    exit(1);
  }

  header = get_elf64_header(fd);
  p_headers = get_target_program_headers(header, fd);
  printf("%p\n", p_headers);
  if (!header) {
    perror("Can not read target file\n");
    exit(1);
  }
  p_headers = get_target_program_headers(header, *(argv[1]));
  printf("[?] Main Entry Point -> %p\n", &main);
  printf("[?] instructions address %p\n", instructions);
  run_instructions();

  //  reopen input file and read
}
