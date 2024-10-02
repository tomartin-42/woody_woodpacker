#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>
#include <stdint.h>

extern void asm_encrypt(void *text, unsigned long text_size, char *key,
                        unsigned int key_size);
extern void asm_encrypt_32(void *text, unsigned long text_size, char *key,
                           unsigned int key_size);

void main_encrypt(t_woody *woody) {
  woody->key = get_random_key(woody->key_size);
  print_key(woody->key, woody->key_size);
  encrypt_file(woody);
}

void main_encrypt_32(t_woody_32 *woody) {
  woody->key = get_random_key_32(woody->key_size);
  print_key(woody->key, woody->key_size);
  encrypt_file_32(woody);
}

char *get_random_key(uint64_t key_size) {
  int fd;
  char c[3] = {0, 0, 0};
  char *key;
  int j = 0;

  key = (char *)malloc(key_size);
  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    launch_error(KEYGEN_FAIL, NULL, 0);
    exit(1);
  }
  for (uint8_t i = 0; i < key_size; i++) {
    j = 0;
    while (j < 33 || j > 126) {
      if (read(fd, c, 3) == -1) {
        launch_error(KEYGEN_FAIL, NULL, 0);
      }
      j = ft_atoi(c);
    }
    key[i] = j;
  }
  close(fd);
  return (key);
}

char *get_random_key_32(uint8_t key_size) {
  int fd;
  char c[3] = {0, 0, 0};
  char *key;
  int j = 0;

  key = (char *)malloc(key_size);
  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    launch_error(KEYGEN_FAIL, NULL, 0);
    exit(1);
  }
  for (uint8_t i = 0; i < key_size; i++) {
    j = 0;
    while (j < 33 || j > 126) {
      if (read(fd, c, 3) == -1) {
        launch_error(KEYGEN_FAIL, NULL, 0);
      }
      j = ft_atoi(c);
    }
    key[i] = j;
  }
  close(fd);
  return (key);
}

void encrypt_file(t_woody *woody) {
  Elf64_Off count = woody->text_off;

  asm_encrypt(count + woody->file, woody->text_size, woody->key,
              woody->key_size);
}

void encrypt_file_32(t_woody_32 *woody) {
  Elf32_Off count = woody->text_off;

  asm_encrypt(count + woody->file, woody->text_size, woody->key,
              woody->key_size);
}
