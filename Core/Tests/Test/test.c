#include "test.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ESC "\x1b["
#define ESC_AQUA ESC "36m"
#define ESC_GREEN ESC "32m"
#define ESC_RED ESC "31m"
#define ESC_RESET ESC "0m"

#define TEST_DELIM "\t"
#define TEST_START ESC_AQUA "START" ESC_RESET TEST_DELIM
#define TEST_PASS ESC_GREEN "PASS" ESC_RESET TEST_DELIM
#define TEST_OK ESC_GREEN "OK" ESC_RESET TEST_DELIM
#define TEST_ERR ESC_RED "ERR" ESC_RESET TEST_DELIM
#define TEST_FAIL ESC_RED "FAIL" ESC_RESET TEST_DELIM

test_t *test_start(const char *name) {
  test_t *t = malloc(sizeof(test_t));
  assert(t != NULL);

  // TODO strncpy?
  t->name = name;
  t->passes = true;

  printf(TEST_START "%s\n", t->name);

  return t;
}

void test_end(test_t *t) {
  if (t->passes) {
    printf(TEST_PASS "%s all passed\n\n", t->name);
  } else {
    printf(TEST_FAIL "%s failed\n\n", t->name);
  }

  // NOTE If t->name is copied in test_start, it needs to be freed
  free(t);
}

void test_assert(test_t *t, const char *pass_message, const char *fail_message,
                 bool passes) {
  if (passes) {
    printf(TEST_OK "%s %s\n", t->name, pass_message);
  } else {
    printf(TEST_ERR "%s %s\n", t->name, fail_message);
  }

  t->passes &= passes;
}

void test_assert_equal(test_t *t, const char *a_label, const char *b_label,
                       float a, float b) {
  bool passes = fabs(a - b) < TEST_EQUAL_EPSILON;
  if (passes) {
    printf(TEST_OK "%s %s = %s (%f = %f)\n", t->name, a_label, b_label, a, b);
  } else {
    printf(TEST_ERR "%s %s ≠ %s (%f ≠ %f)\n", t->name, a_label, b_label, a, b);
  }

  t->passes &= passes;
}

void test_assert_within_error(test_t *t, const char *actual_label,
                              const char *expected_label, float actual,
                              float expected) {
  float percent_error = (actual - expected) / expected * 100;
  bool passes = fabs(percent_error) < TEST_ERROR_DELTA_PERCENT;
  if (passes) {
    printf(TEST_OK "%s %s ≈ %s (%f ≈ %f)\n", t->name, actual_label,
           expected_label, actual, expected);
  } else {
    printf(TEST_ERR "%s %s ≠ %s (%f ≠ %f)\n", t->name, actual_label,
           expected_label, actual, expected);
  }

  t->passes &= passes;
}
