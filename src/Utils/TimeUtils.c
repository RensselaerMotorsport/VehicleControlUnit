#include "../../includes/TimeUtils.h"

// Get current time in seconds
double getCurrentTime() {
    return (double)clock() / CLOCKS_PER_SEC;
}

// Measure interval and update last interval time
double measureInterval(double lastIntervalTime) {
    double currentTime = getCurrentTime();
    double interval = currentTime - lastIntervalTime;
    return interval;
}
