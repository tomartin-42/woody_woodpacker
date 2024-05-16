#include "../Unity/src/unity.h"
#include "../includes/woody.h"
#include <fcntl.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

Elf64_Ehdr *get_elf64_header(char *file);

void test_get_data_origin_file(void) {
  TEST_ASSERT_NOT_NULL(get_elf64_header("./files/normal"));
  Elf64_Ehdr *header = (Elf64_Ehdr *)malloc(sizeof(Elf64_Ehdr));
  header = get_elf64_header("./files/normal");
  TEST_ASSERT_EQUAL_MEMORY(header, get_elf64_header("./files/normal"), 64);
  free(header);
}
