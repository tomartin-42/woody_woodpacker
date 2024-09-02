#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>

extern void asm_encrypt(void *text, unsigned long text_size, char *key,
                        unsigned int key_size);

void main_encrypt(t_woody *woody) {
  woody->key = get_random_key(woody->key_size);
  print_key(woody->key, woody->key_size);
  encrypt_file(woody);
}

char *get_random_key(uint64_t key_size) {
  int fd;
  char c[2] = {0, 0};
  char *key;
  int j = 0;

  key = (char *)malloc(8);
  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    launch_error(KEYGEN_FAIL, NULL, 0);
    exit(1);
  }
  for (uint8_t i = 0; i < key_size; i++) {
    j = 0;
    while (j < 33 || j > 126) {
      if (read(fd, c, 2) == -1) {
        launch_error(KEYGEN_FAIL, NULL, 0);
      }
      j = ft_atoi(c);
    }
    key[i] = j;
    // key[i] = 66;
  }
  close(fd);
  return (key);
}

void encrypt_file(t_woody *woody) {
  Elf64_Off count = woody->text_off;

  // printf("text_offset %10x\n", (int)woody->text_off);
  // printf("text_offset + count %p\n", woody->file + count);
  // printf("some bytes before %.20ld\n", *(Elf64_Off *)(woody->file + count));
  // printf("text_size %10x\n", (int)woody->text_size);
  // printf("key %.8s\n", woody->key);
  asm_encrypt(count + woody->file, woody->text_size, woody->key,
              woody->key_size);
}
