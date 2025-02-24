#include <stdbool.h>
#include <stdlib.h>

/* Defines a mapping from one input to one output. */
typedef struct {
  /* Input value. */
  double input;
  /* Output value. */
  double output;
} point;

/* Compares two points. */
int point_compare(const void *a, const void *b) {
  const point *pa = a;
  const point *pb = b;
  if (pa->input < pb->input)
    return -1;
  if (pa->input > pb->input)
    return 1;
  return 0;
}

bool point_is_between(const point *min, const point *max, double in) {
  return min->input <= in && in <= max->input;
}

/* Defines a table of multiple points, forming a continuous mapping from inputs
 * to outputs. */
typedef struct {
  /* Number of points. */
  unsigned long count;
  /* Points themselves. */
  point points[];
} table;

/* Allocates memory for a table. */
table *table_alloc(unsigned long count) {
  // The table must contain at least two elements; a minimum and a maximum
  // assert(count >= 2);
  if (count < 2) {
    return NULL;
  }

  unsigned long size = sizeof(table) + count * sizeof(point);

  table *table = malloc(size);

  table->count = count;

  return table;
}

/* Releases memory occupied by a table. */
void table_release(table *table) {
  // TODO Potential issue freeing flexible array of points
  free(table);
}

const point *table_min_point(const table *table) { 
  // FIXME Return NULL on uninitialized minimum reference point
  return &table->points[0]; 
}

const point *table_max_point(const table *table) {
  // FIXME Return NULL on uninitialized maximum reference point
  return &table->points[table->count - 1];
}

bool table_is_okay_input(const table *table, double in) {
  const point *min = table_min_point(table);
  const point *max = table_max_point(table);
  if (min == NULL || max == NULL) {
    return false;
  }
  return point_is_between(min, max, in);
}

void table_set_reference_point(table *table, unsigned long n, double input,
                               double output) {
  table->points[n].input = input;
  table->points[n].output = output;
}

/* Initializes the points such that input and output of all points are equally
 * spaced. */
void table_init_linear(table *table, double in_min, double in_max,
                       double out_min, double out_max) {
  // The first step is not a scaled step, since it is just [in_min out_min]
  unsigned long scaled_steps = table->count - 1;
  double in_step = (in_max - in_min) / scaled_steps;
  double out_step = (out_max - out_min) / scaled_steps;

  for (unsigned long n = 0; n < table->count; n++) {
    double in = in_min + in_step * n;
    double out = out_min + out_step * n;
    table_set_reference_point(table, n, in, out);
  }
}

/* Prints the points of the table. */
void table_print(const table *table) {
  //  for (unsigned long n = 0; n < table->count; n++) {
  //    point point = table->points[n];
  //    printf("%f -> %f\n", point.input, point.output);
  //  }
}

/* Sorts the table by input,  */
void table_sort(table *table) {
  qsort(table->points, table->count, sizeof(point), point_compare);
}

/* Search for the index of the first point in the table that has a greater
 * input. */
unsigned long table_search(const table *table, double in) {
  // TODO Implement a more efficient search algorithm; should not be necessary
  // for small tables

  unsigned long n;
  for (n = 0; n < table->count; n++) {
    if (table->points[n].input > in) {
      break;
    }
  }

  // Assert that the value is in range.
  // assert(n >= 1);
  // assert(n < table->count);

  return n;
}

/* Samples the table and calculates the approximate output value. Returns true
 * on success. */
bool table_sample(const table *table, double in, double *out) {
  // This is where the constraint that the table contains at least two elements
  // is derived from
  const point *min = table_min_point(table);
  const point *max = table_max_point(table);

  if (min == NULL || max == NULL) {
    return false;
  }

  if (!point_is_between(min, max, in))
    return false;

  if (in == min->input) {
    *out = min->output;
    return true;
  }

  if (in == max->input) {
    *out = max->output;
    return true;
  }

  // Search for the first point in the table that has a greater input
  unsigned long n = table_search(table, in);
  point low = table->points[n - 1];
  point high = table->points[n];

  // Map the input value to a t-value [0, 1]
  double t = (in - low.input) / (high.input - low.input);

  *out = low.output + t * (high.output - low.output);
  return true;
}
