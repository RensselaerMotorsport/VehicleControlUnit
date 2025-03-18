#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

#ifndef TEST_EQUAL_EPSILON
#define TEST_EQUAL_EPSILON 0.1
#endif

#ifndef TEST_ERROR_DELTA_PERCENT
#define TEST_ERROR_DELTA_PERCENT 5
#endif

// Records status for a set of assertions.
typedef struct {
  const char *name;
  bool passes;
} test_t;

// Creates a test and prints a test start message.
test_t *test_start(const char *name);

// Destroys a test and prints a test end message.
void test_end(test_t *t);

// Prints a message depending on if the test passes or fails
void test_assert(test_t *t, const char *pass_message, const char *fail_message,
          bool passes);

// Tests if the values are approximately equal (pass) or
// not (fail); approximately equal depends on the value of ϵ
// (TEST_EQUAL_EPSILON)
void test_assert_equal(test_t *t, const char *a_label, const char *b_label, float a,
                float b);

// Tests if the values are within a percent error threshold (pass) or not
// (fail); percent error threshold depends on the value of δ
// (TEST_ERROR_DELTA_PERCENT)
void test_assert_within_error(test_t *t, const char *actual_label,
                       const char *expected_label, float actual,
                       float expected);

#endif // TEST_H
