#include "../includes/woody.h"
#include <elf.h>

int main(int argc, char **argv) {
  int check;
  int fd;
  void *origin_file;
  ssize_t origin_len;
  t_woody *woody;
  t_woody_32 *woody_32;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) < 2) {
    perror("Can not open target file");
    exit(errno);
  }
  // printf("FD -> %d\n", fd);

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
    woody = (t_woody *)malloc(sizeof(t_woody));
    ft_bzero(&woody->key_size, 8);
    woody->key_size = 64;
    get_elf64_data(woody, origin_file, origin_len);
    main_set_data(woody, origin_file, origin_len);
    put_file(woody, origin_file, origin_len);
  } else if (check == ELFCLASS32) {
    // elf32_worker
    woody_32 = (t_woody_32 *)malloc(sizeof(t_woody_32));
    ft_bzero(&woody_32->key_size, 8);
    woody_32->key_size = 32;
    get_elf32_data(woody_32, origin_file, origin_len);
    main_set_data_32(woody_32, origin_file, origin_len);
    put_file_32(woody_32, origin_file, origin_len);
  }
}
