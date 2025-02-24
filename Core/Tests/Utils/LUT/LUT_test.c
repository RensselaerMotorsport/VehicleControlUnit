#include "../Inc/Utils/LUT.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

// Prints a test start message.
void test_start(const char *test_label) { printf("START\t%s\n", test_label); }

// Prints a message depending on if the test passes or fails
void test(const char *pass_message, const char *fail_message, bool passes) {
  if (passes) {
    printf("PASS\t%s\n", pass_message);
  } else {
    printf("FAIL\t%s\n", fail_message);
  }
}

// Tests if the values are approximately equal (pass) or
// not (fail); approximately equal depends on value of ϵ
void test_equal(const char *a_label, const char *b_label, double a, double b) {
  bool passes = fabs(a - b) < 0.1;
  if (passes) {
    printf("PASS\t%s = %s (%f = %f)\n", a_label, b_label, a, b);
  } else {
    printf("FAIL\t%s ≠ %s (%f ≠ %f)\n", a_label, b_label, a, b);
  }
}

// Generates expected altitude measurements in meters for pressure readings in
// millibar. Uses the international barometric formula.
// https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
double altitude_m_at_pressure_mb(double pressure_mb) {
  const double SEA_LEVEL_PRESSURE_MB = 1013.25;
  // TODO Explain how this comes about
  double result = pow(pressure_mb / SEA_LEVEL_PRESSURE_MB, 1 / 5.255);
  return 44330 * (1 - result);
}

// Adds the expected result of a pressure measurement to the table in the given
// position. In the future, this should be added as a method of table itself.
void table_add_reference_point_(table *table, double pressure_mb) {}

// Tests if an uninitialized table behaves as expected; everything should be
// zero-valued and methods should not be functional
void test_uninitialized_table(table *table) {
  test_start("uninitialized table");
  test("table is uninitialized", "table is initialized",
       !table_is_initialized(table));
  test("min is undefined", "min is defined", table_min_point(table) == NULL);
  test("max is undefined", "max is defined", table_max_point(table) == NULL);
  test("0 is not okay input", "0 is okay input",
       !table_is_okay_input(table, 0));
  test("300 is not okay input", "300 is okay input",
       !table_is_okay_input(table, 300));
  double altitude_m;
  bool okay = table_sample(table, 300, &altitude_m);
  test("is not okay", "is okay", !okay);
}

// Tests if an uninitialized table behaves as expected; everything should be
// zero-valued and methods should not be functional
void test_initialized_table(table *table) {
  test_start("initialized table");
  test("table is initialized", "table is uninitialized",
       table_is_initialized(table));
  test("min is defined", "min is undefined", table_min_point(table) != NULL);
  test("min is first point in table", "min is not first point in table",
       table_min_point(table) == &table->points_[0]);
  test("max is defined", "max is undefined", table_max_point(table) != NULL);
  test("max is last point in table", "max is not last point in table",
       table_max_point(table) == &table->points_[table->count_ - 1]);
  test("0 is not okay input", "0 is okay input",
       !table_is_okay_input(table, 0));
  test("300 is okay input", "300 is not okay input",
       table_is_okay_input(table, 300));
  double altitude_m;
  bool okay = table_sample(table, 300, &altitude_m);
  test("is okay", "is not okay", okay);
}

// Tests if sampling a known reference input in the table returns the known
// reference output
void test_sampling_reference_point(table *table) {
  double reference_pressure_mb = 300;
  double reference_altitude_m =
      altitude_m_at_pressure_mb(reference_pressure_mb);
  double altitude_m = 0.0;
  bool okay = table_sample(table, reference_pressure_mb, &altitude_m);
  test_start("sampling reference point");
  test("is okay", "is not okay", okay);
  test_equal("altitude", "reference altitude", altitude_m,
             reference_altitude_m);
}

int main() {
  table *pressure_mb_to_altitude_m = table_alloc(9);

  test_uninitialized_table(pressure_mb_to_altitude_m);

  // Initializes the table with equally-spaced points using the transfer
  // function
  for (unsigned int n = 0; n < pressure_mb_to_altitude_m->count_; ++n) {
    double reference_pressure_mb = 300 + n * 100;
    assert(table_add_reference_point(
        pressure_mb_to_altitude_m, reference_pressure_mb,
        altitude_m_at_pressure_mb(reference_pressure_mb)));
  }

  test_initialized_table(pressure_mb_to_altitude_m);

  test_sampling_reference_point(pressure_mb_to_altitude_m);
}
