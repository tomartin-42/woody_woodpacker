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
  TEST_ASSERT(check_the_origin_file("./files/normal") > 0);
  TEST_ASSERT(check_the_origin_file("./files/without_permissions") < 0);
}
