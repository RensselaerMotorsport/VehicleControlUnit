#include "../../Inc/Utils/TimeUtils.h"

double getCurrentTime() {
    return (double)clock() / CLOCKS_PER_SEC;
}

double measureInterval(double lastIntervalTime) {
    double currentTime = getCurrentTime();
    double interval = currentTime - lastIntervalTime;
    return interval;
}
