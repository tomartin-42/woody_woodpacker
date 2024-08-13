#include "../includes/woody.h"
#include <fcntl.h>

void main_encrypt(t_woody *woody) {
  woody->key = get_random_key();
  // encrypt_file(woody);
}

char *get_random_key() {
  int fd;
  char c[2] = {0, 0};
  char *key;
  int j = 0;

  key = (char *)malloc(32);
  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    launch_error(KEYGEN_FAIL, NULL, 0);
    exit(1);
  }
  for (int i = 0; i < 32; i++) {
    j = 0;
    while (j < 33 || j > 126) {
      if (read(fd, c, 2) == -1) {
        launch_error(KEYGEN_FAIL, NULL, 0);
      }
      j = ft_atoi(c);
    }
    key[i] = j;
  }
  close(fd);
  printf("KEY: %.32s\n", key);
  return (key);
}

// void encrypt_file(t_woody *woody) {}
//
