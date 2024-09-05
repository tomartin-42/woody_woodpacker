#include "../includes/woody.h"
#include <time.h>

int main(int argc, char **argv) {
  int check;
  int fd;
  void *origin_file;
  ssize_t origin_len;
  t_woody *woody;

  woody = (t_woody *)malloc(sizeof(t_woody));
  if (argc > 3) {
    printf("incorrect num of arguments %i\n", argc);
    free(woody);
    exit(1);
    if (argc == 2) {
      woody->key_size = 32;
    }
  }

  if ((fd = open(argv[1], O_RDONLY)) < 2) {
    launch_error(NOT_OPEN_FILE, NULL, 0);
  }
  // printf("FD -> %d\n", fd);

  origin_len = lseek(fd, 0, SEEK_END);
  if (origin_len == -1) {
    close(fd);
    launch_error(LSEEK_ERROR, NULL, 0);
  }

  origin_file =
      mmap(NULL, origin_len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (origin_file == MAP_FAILED) {
    close(fd);
    launch_error(MMAP_ERROR, NULL, 0);
    exit(errno);
  }
  close(fd);

  check = check_origin_elf(origin_file, origin_len);

  ft_bzero(&woody->key_size, 8);

  if (check == ELFCLASS64) {
    // elf64_worker
    main_get_elf64_data(woody, origin_file, origin_len);
    main_set_data(woody, origin_file, origin_len);
    put_file(woody, origin_file, origin_len);
    // put_file(woody, origin_file, origin_len);
    // printf("64 WORKER\n");
  } else {
    main_get_elf32_data(woody, origin_file, origin_len);
    // elf32_worker
    // printf("32 WORKER\n");
  }
}
