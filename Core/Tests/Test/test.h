#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

#ifndef TEST_EQUAL_EPSILON
#define TEST_EQUAL_EPSILON 0.1
#endif

#ifndef TEST_ERROR_DELTA_PERCENT
#define TEST_ERROR_DELTA_PERCENT 5
#endif

// TODO Find a way to do
// TEST(NAME) {
//   // test body
// }
// TODO Also find a way for the name to have spaces / nicer formatting
#define TEST(NAME, BODY)                                                       \
  {                                                                            \
    test_t *T = test_start(#NAME);                                             \
    BODY test_end(T);                                                          \
  }

// TODO Add versions of the macros where labels are the variable names

#define ASSERT(BOOL, OK, ERR) test_assert(T, OK, ERR, BOOL)

#define ASSERT_OK(BOOL, LABEL)                                                 \
  ASSERT(BOOL, LABEL " is okay", LABEL " is not okay")

#define ASSERT_EQ(GOT, WANT, GOT_LABEL, WANT_LABEL)                            \
  test_assert_equal(T, GOT_LABEL, WANT_LABEL, GOT, WANT)

#define ASSERT_IN_ERROR(GOT, WANT, GOT_LABEL, WANT_LABEL)                      \
  test_assert_within_error(T, GOT_LABEL, WANT_LABEL, GOT, WANT)

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
void test_assert_equal(test_t *t, const char *a_label, const char *b_label,
                       float a, float b);

// Tests if the values are within a percent error threshold (pass) or not
// (fail); percent error threshold depends on the value of δ
// (TEST_ERROR_DELTA_PERCENT)
void test_assert_within_error(test_t *t, const char *actual_label,
                              const char *expected_label, float actual,
                              float expected);

#endif // TEST_H
