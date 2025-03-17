#include "../Inc/Utils/LUT.h"
#include "test.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

// Generates expected altitude measurements in meters for pressure readings in
// millibar. Uses the international barometric formula.
// https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
float altitude_m_at_pressure_mb(float pressure_mb) {
  const float SEA_LEVEL_PRESSURE_MB = 1013.25;
  // TODO Explain how this comes about
  float result = pow(pressure_mb / SEA_LEVEL_PRESSURE_MB, 1 / 5.255);
  return 44330 * (1 - result);
}

// Tests if an uninitialized table behaves as expected; everything should be
// zero-valued and methods should not be functional
void test_uninitialized_table(table *table) {
  test_start("uninitialized table");
  test("table is uninitialized", "table is initialized",
       !table_is_initialized(table));
  test("min is undefined", "min is defined", table_min_point(table) == NULL);
  test("max is undefined", "max is defined", table_max_point(table) == NULL);
  test("cannot sample 0 mb", "can sample 0 mb", !table_can_sample(table, 0));
  test("cannot sample 300 mb", "can sample 300 mb",
       !table_can_sample(table, 300));
  float altitude_m;
  bool okay = table_sample(table, 300, &altitude_m);
  test("is not okay", "is okay", !okay);
}

// Tests if an uninitialized table behaves as expected; nothing should be
// zero-valued and methods should be functional
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
  test("cannot sample 0 mb", "can sample 0 mb", !table_can_sample(table, 0));
  test("can sample 300 mb", "cannot sample 300 mb",
       table_can_sample(table, 300));
}

// Tests if sampling a known reference input in the table returns the known
// reference output
void test_sampling_endpoints(table *table) {
  float min_reference_pressure_mb = 300;
  float max_reference_pressure_mb = 1100;
  float min_reference_altitude_m =
      altitude_m_at_pressure_mb(min_reference_pressure_mb);
  float max_reference_altitude_m =
      altitude_m_at_pressure_mb(max_reference_pressure_mb);
  float altitude_m = 0.0;

  test_start("sampling min reference point");
  test("is okay", "is not okay",
       table_sample(table, min_reference_pressure_mb, &altitude_m));
  test_equal("altitude", "reference altitude", altitude_m,
             min_reference_altitude_m);
  test_start("sampling max reference point");
  test("is okay", "is not okay",
       table_sample(table, max_reference_pressure_mb, &altitude_m));
  test_equal("altitude", "reference altitude", altitude_m,
             max_reference_altitude_m);
}

// Tests if sampling a known reference input in the table returns the known
// reference output
void test_sampling_reference_point(table *table) {
  float reference_pressure_mb = 600;
  float reference_altitude_m =
      altitude_m_at_pressure_mb(reference_pressure_mb);
  float altitude_m = 0.0;

  test_start("sampling reference point");
  test("is okay", "is not okay",
       table_sample(table, reference_pressure_mb, &altitude_m));
  test_equal("altitude", "reference altitude", altitude_m,
             reference_altitude_m);
}

// Tests if sampling an unknown input in the table returns an
// approximately-correct output
void test_sampling_unknown_point(table *table) {
  float pressure_mb = 650;
  float expected_altitude_m = altitude_m_at_pressure_mb(pressure_mb);
  float altitude_m = 0.0;

  test_start("sampling reference point");
  test("is okay", "is not okay", table_sample(table, pressure_mb, &altitude_m));
  float percent_error =
      (altitude_m - expected_altitude_m) / expected_altitude_m * 100;
  test_within_error("altitude", "expected altitude", altitude_m, expected_altitude_m);
}

void lut_main() {
  table pressure_mb_to_altitude_m;
  assert(table_init(&pressure_mb_to_altitude_m, 9));

  test_uninitialized_table(&pressure_mb_to_altitude_m);

  // Initializes the table with equally-spaced points using the transfer
  // function
  for (unsigned int reference_pressure_mb = 300; reference_pressure_mb <= 1100;
       reference_pressure_mb += 100) {
    assert(table_add_reference_point(
        &pressure_mb_to_altitude_m, reference_pressure_mb,
        altitude_m_at_pressure_mb(reference_pressure_mb)));
  }

  test_initialized_table(&pressure_mb_to_altitude_m);

  test_sampling_endpoints(&pressure_mb_to_altitude_m);

  test_sampling_reference_point(&pressure_mb_to_altitude_m);

  test_sampling_unknown_point(&pressure_mb_to_altitude_m);
}
