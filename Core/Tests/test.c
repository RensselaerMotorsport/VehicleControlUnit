#include "test.h"

#include <math.h>
#include <stdio.h>

void test_start(const char *test_label) { printf("START\t%s\n", test_label); }

void test(const char *pass_message, const char *fail_message, bool passes) {
  if (passes) {
    printf("PASS\t%s\n", pass_message);
  } else {
    printf("FAIL\t%s\n", fail_message);
  }
}

void test_equal(const char *a_label, const char *b_label, float a, float b) {
  bool passes = fabs(a - b) < TEST_EQUAL_EPSILON;
  if (passes) {
    printf("PASS\t%s = %s (%f = %f)\n", a_label, b_label, a, b);
  } else {
    printf("FAIL\t%s ≠ %s (%f ≠ %f)\n", a_label, b_label, a, b);
  }
}

void test_within_error(const char *actual_label, const char *expected_label,
                       float actual, float expected) {
  float percent_error = (actual - expected) / expected * 100;
  bool passes = fabs(percent_error) < TEST_ERROR_DELTA_PERCENT;
  if (passes) {
    printf("PASS\t%s ≈ %s (%f ≈ %f)\n", actual_label, expected_label, actual,
           expected);
  } else {
    printf("FAIL\t%s ≠ %s (%f ≠ %f)\n", actual_label, expected_label, actual,
           expected);
  }
}
