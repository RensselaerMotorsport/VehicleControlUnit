#ifndef RENNSSELAERMOTORSPORT_LUT_H
#define RENNSSELAERMOTORSPORT_LUT_H
#include <stdbool.h>

/* Defines a mapping from one input to one output. */
typedef struct {
  /* Input value. */
  double input;
  /* Output value. */
  double output;
} point;

/* Compares two points. */
int point_compare(const void *a, const void *b);

/* Returns true if the input is between the minimum input and maximum input. */
bool point_is_between(const point *min, const point *max, double in);

/* Defines a interpolated table of multiple reference points, forming a
 * continuous mapping from inputs to outputs. */
typedef struct {
  /* Number of reference points. */
  unsigned long count;
  /* Reference points. */
  point points[];
} table;

/* Allocates memory for a table. */
table *table_alloc(unsigned long count);

/* Releases memory occupied by a table. */
void table_release(table *table);

/* Gets the minimum reference point (the reference point with the least input).
 * If the minimum reference point is not defined, returns NULL.
 */
const point *table_min_point(const table *table);

/* Gets the maximum reference point (the reference point with the greatest
 * input). If the maximum reference point is not defined, returns NULL. */
const point *table_max_point(const table *table);

/* Tests if the input is valid for the table. */
bool table_is_okay_input(const table *table, double in);

/* Sets a reference point in the table. Returns true if the reference point was
 * added. */
void table_set_reference_point(table *table, unsigned long n, double input,
                               double output);

/* Initializes the reference points such that input and output of all reference
 * points are equally spaced. */
void table_init_linear(table *table, double in_min, double in_max,
                       double out_min, double out_max);

/* Prints the reference points of the table. */
void table_print(const table *table);

/* Sorts the reference points by input. */
void table_sort(table *table);

/* Search for the index of the first reference point in the table that has a
 * greater input. */
unsigned long table_search(const table *table, double in);

/* Samples the table and calculates the approximate output value. Returns true
 * on success. */
bool table_sample(const table *table, double in, double *out);
#endif // RENNSSELAERMOTORSPORT_LUT_H
