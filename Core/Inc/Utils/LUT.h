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

/* Defines a table of multiple points, forming a continuous mapping from inputs
 * to outputs. */
typedef struct {
  /* Number of points. */
  unsigned long count;
  /* Points themselves. */
  point points[];
} table;

/* Allocates memory for a table. */
table *table_alloc(unsigned long count);

/* Releases memory occupied by a table. */
void table_release(table *table);

/* Initializes the points such that input and output of all points are equally
 * spaced. */
void table_init_linear(table *table, double in_min, double in_max,
                       double out_min, double out_max);

/* Prints the points of the table. */
void table_print(const table *table);

/* Sorts the table by input,  */
void table_sort(table *table);

/* Search for the index of the first point in the table that has a greater
 * input. */
unsigned long table_search(const table *table, double in);

/* Samples the table and calculates the approximate output value. Returns true
 * on success. */
bool table_sample(const table *table, double in, double *out);
#endif // RENNSSELAERMOTORSPORT_LUT_H
