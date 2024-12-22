#include "../../Inc/Utils/PlatformTime.h"

double measureInterval(double lastIntervalTime) {
    double currentTime = CURRENT_TIME_MS();
    double interval = currentTime - lastIntervalTime;
    return interval;
}
