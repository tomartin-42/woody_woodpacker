#include "../includes/woody.h"
#include <fcntl.h>

void encrypt_main(t_woody *woody) { woody->key = get_random_key(); }

char *get_random_key() {
  int fd;
  char c[2] = {0, 0};
  char *key;
  int j = 0;

  key = (char *)malloc(64);
  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    // error
    exit(1);
  }
  for (int i = 0; i < 64; i++) {
    j = 0;
    while (j < 33 || j > 126) {
      read(fd, c, 2);
      j = ft_atoi(c);
    }
    key[i] = j;
  }
  close(fd);
  printf("KEY: %.64s\n", key);
  return (key);
}
