#include "../includes/woody.h"

static void init_t_woody(t_woody *woody) {
  woody->header = NULL;
  woody->p_header = NULL;
  woody->s_header = NULL;
  woody->my_Pheader = NULL;
  woody->file = NULL;
  woody->key = NULL;
  woody = NULL;
}

int main(int argc, char **argv) {
  int fd;
  void *origin_file;
  ssize_t origin_len;
  t_woody *woody = NULL;

  if (argc != 2) {
    printf("incorrect num of arguments %i\n", argc);
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) < 2) {
    perror("Can not open target file");
    exit(errno);
  }

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

  main_checker(origin_file, origin_len);

  woody = (t_woody *)malloc(sizeof(t_woody));
  init_t_woody(woody);
  ft_bzero(&woody->key_size, 8);
  woody->key_size = 64;

  get_elf64_data(woody, origin_file, origin_len);
  main_set_data(woody, origin_file, origin_len);
  put_file(woody, origin_file, origin_len);
}
