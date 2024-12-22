/**
  ******************************************************************************
  * @file           : PlatformTime.h
  * @brief          : Handles Time depending on platform
  ******************************************************************************
*/

#ifndef RENNSSELAERMOTORSPORT_PLATFORM_TIME_H
#define RENNSSELAERMOTORSPORT_PLATFORM_TIME_H

/**
 * @def CURRENT_TIME_MS()
 * @brief Retrieves the current time in milliseconds.
 *
 * This macro returns the current time in milliseconds based on the platform. 
 * For Linux/Unix systems, it uses `clock_gettime` with `CLOCK_MONOTONIC`. 
 * For STM32 systems, it uses the HAL's `HAL_GetTick` function.
 *
 * @note Make sure the correct platform is defined
 *
 * @return int The current time in milliseconds.
 */
#ifdef __linux__

#include <time.h>
#define CURRENT_TIME_MS() ({ \
    struct timespec ts; \
    clock_gettime(CLOCK_MONOTONIC, &ts); \
    (int)((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000)); \
})

#elif defined(STM32)

// #include "stm32f4xx_hal.h" // Adjust for our STM32 specific board
// #define CURRENT_TIME_MS() HAL_GetTick()

#else

#error "Platform not supported!"

#endif

/**
 * @brief Measures the time interval since the last recorded time.
 *
 * @param lastIntervalTime The last recorded time in seconds.
 * @return double The time interval in seconds.
 */
double measureInterval(double lastIntervalTime);

#endif // RENNSSELAERMOTORSPORT_PLATFORM_TIME_H
