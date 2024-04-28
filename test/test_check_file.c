#include "../Unity/src/unity.h"
#include "../includes/woody.h"
#include <fcntl.h>
#include <stdio.h>

void test_check_the_origin_file(void);

void test_check_the_origin_file(void) {
  TEST_ASSERT_EQUAL_INT(0, check_the_origin_file("hola"));
  TEST_ASSERT_EQUAL_INT(1, check_the_origin_file("xxxxx"));
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_check_the_origin_file);

  return UNITY_END();
}
