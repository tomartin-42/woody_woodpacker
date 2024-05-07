#include "../Unity/src/unity.h"
#include "../includes/woody.h"
#include <fcntl.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_check_the_origin_file(void) {
  TEST_ASSERT(check_the_origin_file("./files/normal") > 0);
  TEST_ASSERT(check_the_origin_file("./files/without_permissions") < 0);

  TEST_ASSERT(
      check_elf64_origin_file(check_the_origin_file("./files/normal")) == 1);
  TEST_ASSERT(check_elf64_origin_file(
                  check_the_origin_file("./files/incorrect_header")) == -1);
  TEST_ASSERT(check_elf64_origin_file(check_the_origin_file("./files/noelf")) ==
              -1);
  TEST_ASSERT(
      check_elf64_origin_file(check_the_origin_file("./files/normal")) == 1);
  TEST_ASSERT(
      check_elf64_origin_file(check_the_origin_file("./files/normal32")) == 2);

  TEST_ASSERT(main_check_input_file("./files/normal") > 0);
  TEST_ASSERT(main_check_input_file("./files/without_permissions") < 0);
  TEST_ASSERT(main_check_input_file("./files/incorrect_header") < 0);
  TEST_ASSERT(main_check_input_file("./files/noelf") < 0);
  TEST_ASSERT(main_check_input_file("./files/normal64") < 0);
}
