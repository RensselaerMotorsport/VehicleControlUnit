#ifndef RENNSSELAERMOTORSPORT_LUT_H
#define RENNSSELAERMOTORSPORT_LUT_H
#include <stdbool.h>
#include <stdint.h>

/* Defines a mapping from one input to one output. */
typedef struct {
  /* Input value. */
  float input;
  /* Output value. */
  float output;
} point;

/* Compares two points. */
int point_compare(const void *a, const void *b);

/* Returns true if the input is between the minimum input and maximum input. */
bool point_is_between(const point *min, const point *max, float in);

#define TABLE_CAPACITY 64

/* Defines a interpolated table of multiple reference points, forming a
 * continuous mapping from inputs to outputs. */
typedef struct {
  /* Number of reference points. */
  uint16_t count_;
  /* Number of added reference points. */
  uint16_t added_;
  /* True if the table is initialized; cache for table_is_initialized. */
  bool initialized_;
  /* Reference points. */
  point points_[TABLE_CAPACITY];
} table;

/* Performs table initalization. Does not add points. Returns true if the table was initalized. */
bool table_init(table *table, uint16_t count);

/* Adds a reference point to the table. Returns true if the reference point was
 * added. */
bool table_add_reference_point(table *table, float in, float out);

/* Returns true if all reference points in the table have been added. */
bool table_is_initialized(table *table);

/* Gets the minimum reference point (the reference point with the least input).
 * If the minimum reference point is not defined, returns NULL.
 */
const point *table_min_point(table *table);

/* Gets the maximum reference point (the reference point with the greatest
 * input). If the maximum reference point is not defined, returns NULL. */
const point *table_max_point(table *table);

/* Tests if the input is a valid sample for the table. */
bool table_can_sample(table *table, float in);

/* Sorts the reference points by input. */
void table_sort(table *table);

/* Samples the table and calculates the approximate output value. Returns true
 * on success. */
bool table_sample(table *table, float in, float *out);
#endif // RENNSSELAERMOTORSPORT_LUT_H
