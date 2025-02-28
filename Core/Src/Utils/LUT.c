#include <stdint.h>
#include <stdlib.h>

#include "../Inc/Utils/LUT.h"

int point_compare(const void *a, const void *b) {
  const point *pa = a;
  const point *pb = b;
  if (pa->input < pb->input)
    return -1;
  if (pa->input > pb->input)
    return 1;
  return 0;
}

bool point_is_between(const point *min, const point *max, float in) {
  return min->input <= in && in <= max->input;
}

/* Returns true if the input can interpolate between the min and max. */
bool can_interpolate(const point *min, const point *max, float in) {
  // Either point is not defined, so can't interpolate
  if (min == NULL || max == NULL) {
    return false;
  }

  // The input can interpolate if it is between the min and max
  return point_is_between(min, max, in);
}

bool table_init(table *table, uint16_t count) {
  if (table == NULL) {
    return false;
  }

  // Table must contain at least two reference points
  if (count < 2 || count > TABLE_CAPACITY) {
    return false;
  }

  // Set up table values to expected values
  table->count_ = count;
  table->added_ = 0;
  table->initialized_ = false;
  for (uint16_t n = 0; n < count; ++n) {
    table->points_[n].input = 0;
    table->points_[n].output = 0;
  }

  return true;
}

bool table_is_initialized(table *table) {
  // Nothing has invalidated the initialized state of the table
  if (table->initialized_) {
    return true;
  }

  bool all_reference_points = table->added_ == table->count_;

  if (!all_reference_points) {
    table->initialized_ = false;
    return table->initialized_;
  }

  bool sorted = true;
  const point *previous_point = &table->points_[0];
  for (uint16_t n = 1; n < table->count_; ++n) {
    const point *point = &table->points_[n];
    // Found a decrease, which violates table being sorted
    if (point->input < previous_point->input) {
      sorted = false;
    }
  }
  table->initialized_ = sorted;
  return table->initialized_;
}

bool table_add_reference_point(table *table, float in, float out) {
  if (table_is_initialized(table)) {
    return false;
  }

  // NOTE This may should not actually be necessary; the only way to get here is
  // if the table is not initialized already; there may be a lurking logic error
  table->initialized_ = false;

  table->points_[table->added_].input = in;
  table->points_[table->added_].output = out;
  table->added_++;

  return true;
}

const point *table_min_point(table *table) {
  if (!table_is_initialized(table))
    return NULL;
  return &table->points_[0];
}

const point *table_max_point(table *table) {
  if (!table_is_initialized(table))
    return NULL;
  return &table->points_[table->count_ - 1];
}

bool table_can_sample(table *table, float in) {
  const point *min = table_min_point(table);
  const point *max = table_max_point(table);
  return can_interpolate(min, max, in);
}

void table_sort(table *table) {
  qsort(table->points_, table->count_, sizeof(point), point_compare);
}

/* Search for the index of the first reference point in the table that has a
 * greater input. */
uint16_t table_search(const table *table, double in) {
  // TODO Implement a more efficient search algorithm; should not be necessary
  // for small tables

  uint16_t n;
  for (n = 0; n < table->count_; n++) {
    if (table->points_[n].input > in) {
      break;
    }
  }

  // Assert that the value is in range.
  // assert(n >= 1);
  // assert(n < table->count);

  return n;
}

bool table_sample(table *table, float in, float *out) {
  // This is where the constraint that the table contains at least two elements
  // is derived from
  const point *min = table_min_point(table);
  const point *max = table_max_point(table);

  // If can't interpolate between the endpoints, can't interpolate between
  // intermediate points
  if (!can_interpolate(min, max, in)) {
    return false;
  }

  if (in == min->input) {
    *out = min->output;
    return true;
  }

  if (in == max->input) {
    *out = max->output;
    return true;
  }

  // Search for the first point in the table that has a greater input
  uint16_t n = table_search(table, in);
  point low = table->points_[n - 1];
  point high = table->points_[n];

  // Map the input value to a t-value [0, 1]
  float t = (in - low.input) / (high.input - low.input);

  *out = low.output + t * (high.output - low.output);
  return true;
}
