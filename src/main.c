#include "../includes/woody.h"

// void run_instructions() {
//   // Cogemos el offset donde está guardado instructions
//   size_t region = (size_t)instructions;
//   region = region & (~0xFFF); // Igualamos los 3 ultimos bytes a 0
//   int ret = mprotect((void *)region, 0x1000, PROT_READ | PROT_EXEC);
//   if (ret != 0) {
//     printf("Error in mprotect %d\n", errno);
//   }
//   void (*f)(void) = (void *)instructions; // declaramos un puntero a funcion
//                                           // que apunta a instructions
//   printf("[!] Iniciamos instructions\n");
//   f();
//   printf("finalizamos instructions\n\n");
// }
//
// void mapping_p_headers(int fd, Elf64_Ehdr *header, Elf64_Phdr *p_headers) {
//
//   for (int i = 0; i < header->e_phnum; i++) {
//     if (p_headers[i].p_type == PT_LOAD) {
//       int prot = 0;
//
//       off_t aligned_offset = p_headers[i].p_offset & ~(4096 - 1);
//       size_t adjustment = p_headers[i].p_offset - aligned_offset;
//       size_t length = p_headers[i].p_memsz + adjustment;
//
//       if (p_headers[i].p_flags & PF_R)
//         prot |= PROT_READ;
//       if (p_headers[i].p_flags & PF_W)
//         prot |= PROT_WRITE;
//       if (p_headers[i].p_flags & PF_X)
//         prot |= PROT_EXEC;
//
//       printf("\n[!] INIT MAP\n");
//       void *p_map = mmap((void *)(p_headers[i].p_vaddr & ~(4096 - 1)),
//       length,
//                          prot, MAP_PRIVATE | MAP_FIXED, fd, aligned_offset);
//       // void *p_map = mmap((void *)p_headers[i].p_vaddr,
//       p_headers[i].p_memsz,
//       //                    prot, MAP_PRIVATE, fd, p_headers[i].p_offset);
//       //    void *p_map = mmap((void *)p_headers[i].p_vaddr,
//       //    p_headers[i].p_memsz,
//       //                      prot, MAP_PRIVATE, fd, offset);
//       if (p_map == MAP_FAILED) {
//         // Imprimir el error específico de mmap
//         fprintf(stderr, "mmap failed: %s\n", strerror(errno));
//         exit(1);
//       }
//       printf("mapeando p_header %d -> %p\n", i, p_map);
//       lseek(fd, p_headers[i].p_offset, SEEK_SET);
//       read(fd, p_map, p_headers[i].p_memsz);
//       printf("copiando\n");
//       // mprotect((void *)p_map, 0x1000, prot);
//       // printf("cambiando permisos\n");
//     }
//   }
//   printf("[!] END MAP\n\n");
//   printf("Init %p\n", (void *)header->e_entry);
//   void (*f)(void) = (void *)header->e_entry;
//   f();
//   printf("[!] END F()\n");
//   close(fd);
// }

int main(int argc, char **argv) {
  int check;
  int fd;
  void *origin_file;
  ssize_t origin_len;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) < 2) {
    perror("Can not open target file");
    exit(errno);
  }
  printf("FD -> %d\n", fd);

  origin_len = lseek(fd, 0, SEEK_END);
  if (origin_len == -1) {
    close(fd);
    perror("lseek() error");
    exit(errno);
  }

  origin_file =
      mmap(NULL, origin_len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (origin_file == MAP_FAILED) {
    close(fd);
    perror("mmap() error");
    exit(errno);
  }
  close(fd);

  check = check_origin_elf(origin_file, origin_len);
  if (check == ELFCLASS64) {
    // elf64_worker
    printf("64 WORKER\n");
  } else {
    // elf32_worker
    printf("32 WORKER\n");
  }

  // header = get_elf64_header(fd);
  // print_header(header);
  // if (!header) {
  //   perror("Can not read target file\n");
  //   exit(1);
  // }
  // p_headers = get_target_program_headers(header, fd);
  // print_info_p_headers(p_headers, header->e_phnum);
  // mapping_p_headers(fd, header, p_headers);
  //    declaramos un puntero a funcion

  //  reopen input file and read
}
