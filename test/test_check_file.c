#include "../Unity/src/unity.h"
#include "../includes/woody.h"
#include <fcntl.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

int check_the_origin_file(char *file);

void test_check_the_origin_file(void) {
  TEST_ASSERT_EQUAL_INT(0, check_the_origin_file("hola"));
  TEST_ASSERT_EQUAL_INT(1, check_the_origin_file("xxxxx"));
}
