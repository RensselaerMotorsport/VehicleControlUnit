#ifndef RENNSSELAERMOTORSPORT_TIME_UTILS_H
#define RENNSSELAERMOTORSPORT_TIME_UTILS_H

#include <time.h>

/**
 * @brief Retrieves the current time in seconds.
 *
 * @return double The current time in seconds.
 */
double getCurrentTime();

/**
 * @brief Measures the time interval since the last recorded time.
 *
 * @param lastIntervalTime The last recorded time in seconds.
 * @return double The time interval in seconds.
 */
double measureInterval(double lastIntervalTime);

#endif // RENNSSELAERMOTORSPORT_TIME_UTILS_H

