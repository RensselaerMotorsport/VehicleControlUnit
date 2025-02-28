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

/**
 * @brief Compares two points by input.
 *
 * @param a The first point to compare.
 * @param b The second point to compare.
 * @return A negative integer if the first point is before the second point,
 * zero if the first point is equal to the second point, or a positive integer
 * if the first point is greater than the second point.
 */
int point_compare(const void *a, const void *b);

/**
 * @brief Determines if the input value is between the input of two points.
 *
 * @param min A point with an input less than the input of max.
 * @param max A point with an input greater than the input of min.
 * @param in An input value to test with the two points.
 * @return True if the input value is between the minimum input and maximum
 * input.
 */
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

/**
 * @brief Initializes the table.
 *
 * @param table A pointer to the table to initialize.
 * @param count The number of reference points in the table.
 * @return True if the table was successfully initialized.
 */
bool table_init(table *table, uint16_t count);

/**
 * @brief Adds a reference point to the table.
 *
 * @param table A pointer to the table to add the point to.
 * @param in The input ("from") value.
 * @param out The output ("to") value at the input value.
 * @return True if the reference point was added to the table.
 */
bool table_add_reference_point(table *table, float in, float out);

/**
 * @brief Checks if the table has been fully initialized, with all reference
 * points added.
 *
 * @param table A pointer to the table to check.
 * @return True if the table has been fully initialized.
 */
bool table_is_initialized(table *table);

/**
 * @brief Retrieves the point in the table with the least input value.
 *
 * @param table A pointer to the table.
 * @return If there is a minimum point, a pointer to the minimum point,
 * otherwise NULL.
 */
const point *table_min_point(table *table);

/**
 * @brief Retrieves the point in the table with the greatest input value.
 *
 * @param table A pointer to the table.
 * @return If there is a maximum point, a pointer to the maximum point,
 * otherwise NULL.
 */
const point *table_max_point(table *table);

/**
 * @brief Checks if the input input is valid for the table.
 *
 * @param table A pointer to the table to check.
 * @param in The input value to check.
 * @return True if the input value is valid for the table.
 */
bool table_can_sample(table *table, float in);

/**
 * @brief Samples a output value from the table for an input value.
 *
 * @param table A pointer to the table to sample.
 * @param in The input value to sample.
 * @param out A pointer to the destination of the output value.
 * @return True if the output value was written, otherwise false.
 */
bool table_sample(table *table, float in, float *out);
#endif // RENNSSELAERMOTORSPORT_LUT_H
