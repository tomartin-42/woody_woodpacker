#include "../includes/woody.h"
#include <elf.h>
#include <stdint.h>

static void init_t_woody(t_woody *woody) {
  woody->header = NULL;
  woody->p_header = NULL;
  woody->s_header = NULL;
  woody->my_Pheader = NULL;
  woody->file = NULL;
  woody->key = NULL;
  woody = NULL;
}

static uint8_t get_key_size(int argc, char **argv) {
  if (argc == 2) {
    return (64);
  };
  if (!ft_strncmp(argv[2], "-8", 2)) {
    return (8);
  } else if (!ft_strncmp(argv[2], "-16", 3)) {
    return (16);
  } else if (!ft_strncmp(argv[2], "-32", 3)) {
    return (32);
  } else if (!ft_strncmp(argv[2], "-64", 3)) {
    return (64);
  }
  printf("Incorrect key size: %s\n", argv[2]);
  printf("The key size is set to 64 bytes\n");

  return (64);
}

int main(int argc, char **argv) {
  int fd;
  void *origin_file;
  ssize_t origin_len;
  int check = 0;
  t_woody *woody;
  t_woody_32 *woody_32;

  if (argc > 3 || argc < 2) {
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

  /* <<<<<<< HEAD */
  /* if (check == ELFCLASS64) { */
  if (check == 0) {
    // elf64_worker
    woody = (t_woody *)malloc(sizeof(t_woody));
    ft_bzero(&woody->key_size, 8);
    woody->key_size = 64;
    get_elf64_data(woody, origin_file, origin_len);
    main_set_data(woody, origin_file, origin_len);
    put_file(woody, origin_file, origin_len);
  } else if (check == 1) {
    /* } else if (check == ELFCLASS32) { */
    // elf32_worker
    woody_32 = (t_woody_32 *)malloc(sizeof(t_woody_32));
    ft_bzero(&woody_32->key_size, 8);
    woody_32->key_size = 32;
    get_elf32_data(woody_32, origin_file, origin_len);
    main_set_data_32(woody_32, origin_file, origin_len);
    put_file_32(woody_32, origin_file, origin_len);
  }
  /* ======= */
  /*   woody = (t_woody *)malloc(sizeof(t_woody)); */
  /*   init_t_woody(woody); */
  /*   ft_bzero(&woody->key_size, 8); */
  /*   woody->key_size = get_key_size(argc, argv); */
  /**/
  /*   get_elf64_data(woody, origin_file, origin_len); */
  /*   main_set_data(woody, origin_file, origin_len); */
  /*   put_file(woody, origin_file, origin_len); */
  /* >>>>>>> 27f6c0427f3ec25d839ee0e6de26758ddd374ab3 */
}
