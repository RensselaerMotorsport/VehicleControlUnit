#include "test.h"

#include <math.h>
#include <stdio.h>

#define ESC "\x1b["
#define ESC_AQUA ESC "36m"
#define ESC_GREEN ESC "32m"
#define ESC_RED ESC "31m"
#define ESC_RESET ESC "0m"

#define TEST_DELIM "\t"
#define TEST_START ESC_AQUA "START" ESC_RESET TEST_DELIM
#define TEST_PASS ESC_GREEN "PASS" ESC_RESET TEST_DELIM
#define TEST_FAIL ESC_RED "FAIL" ESC_RESET TEST_DELIM

void test_start(const char *test_label) {
  printf(TEST_START "%s\n", test_label);
}

void test(const char *pass_message, const char *fail_message, bool passes) {
  if (passes) {
    printf(TEST_PASS "%s\n", pass_message);
  } else {
    printf(TEST_FAIL "%s\n", fail_message);
  }
}

void test_equal(const char *a_label, const char *b_label, float a, float b) {
  bool passes = fabs(a - b) < TEST_EQUAL_EPSILON;
  if (passes) {
    printf(TEST_PASS "%s = %s (%f = %f)\n", a_label, b_label, a, b);
  } else {
    printf(TEST_FAIL "%s ≠ %s (%f ≠ %f)\n", a_label, b_label, a, b);
  }
}

void test_within_error(const char *actual_label, const char *expected_label,
                       float actual, float expected) {
  float percent_error = (actual - expected) / expected * 100;
  bool passes = fabs(percent_error) < TEST_ERROR_DELTA_PERCENT;
  if (passes) {
    printf(TEST_PASS "%s ≈ %s (%f ≈ %f)\n", actual_label, expected_label,
           actual, expected);
  } else {
    printf(TEST_FAIL "%s ≠ %s (%f ≠ %f)\n", actual_label, expected_label,
           actual, expected);
  }
}
