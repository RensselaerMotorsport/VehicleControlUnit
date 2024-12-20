#ifndef PLATFORM_TIME_H
#define PLATFORM_TIME_H

#ifdef __linux__

#include <time.h> // Non-realtime for testing
#define CURRENT_TIME_MS() ({ \
    struct timespec ts; \
    clock_gettime(CLOCK_MONOTONIC, &ts); \
    (int)((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000)); \
})

#elif defined(STM32)

/*#include "stm32f4xx_hal.h" // Adjust for your STM32 family*/
/*#define CURRENT_TIME_MS() HAL_GetTick()*/

#else

#error "Platform not supported!"

#endif

#endif // PLATFORM_TIME_H
