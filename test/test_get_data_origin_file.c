#include "../Unity/src/unity.h"
#include "../includes/woody.h"
#include <elf.h>
#include <fcntl.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

Elf64_Ehdr *get_elf64_header(int fd);
Elf64_Phdr *get_target_program_headers(Elf64_Ehdr *header, int fd);

void test_get_data_origin_file(void) {
  int fd_normal = open("./files/normal", O_RDONLY);
  Elf64_Ehdr *header = get_elf64_header(fd_normal);
  Elf64_Phdr *p_header = get_target_program_headers(header, fd_normal);

  TEST_ASSERT_NOT_NULL(header);
  lseek(fd_normal, 0, SEEK_SET);
  TEST_ASSERT_EQUAL_MEMORY(header, get_elf64_header(fd_normal), 64);
  TEST_ASSERT_NOT_NULL(p_header);
  TEST_ASSERT_EQUAL_MEMORY(p_header,
                           get_target_program_headers(header, fd_normal), 128);
  free(header);
  free(p_header);
  close(fd_normal);
}
