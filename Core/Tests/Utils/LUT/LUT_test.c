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
void table_add_reference_point(table *table, unsigned int index,
                               double pressure_mb) {
  // NOTE Probably not necessary for testing, but in case the index is somehow
  // out of range
  assert(index >= 0 && index < table->count);
  table->points[index].input = pressure_mb;
  // TODO If this gets added to the table implementation itself, use a parameter
  // for output
  table->points[index].output = altitude_m_at_pressure_mb(pressure_mb);
}

// Tests if sampling a known reference input in the table returns the known
// reference output
void test_sampling_reference_point(table *table) {
  double reference_pressure_mb = 300;
  double reference_altitude_m =
      altitude_m_at_pressure_mb(reference_pressure_mb);
  double altitude_m;
  bool okay = table_sample(table, reference_pressure_mb, &altitude_m);
  test_start("sampling reference point");
  test("is okay", "is not okay", okay);
  test_equal("altitude", "reference altitude", altitude_m,
             reference_altitude_m);
}

int main() {
  table *pressure_mb_to_altitude_mb = table_alloc(9);
  // Initializes the table with equally-spaced points using the transfer
  // function
  for (unsigned int index = 0; index < pressure_mb_to_altitude_mb->count;
       ++index) {
    double reference_pressure_mb = 300 + index * 100;
    table_add_reference_point(pressure_mb_to_altitude_mb, index,
                              reference_pressure_mb);
  }

  test_sampling_reference_point(pressure_mb_to_altitude_mb);
}
