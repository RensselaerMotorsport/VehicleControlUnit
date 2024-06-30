#include "../../../includes/WheelFlux.h"
#include <stdio.h>
#include <time.h>

// Get current time in seconds
double getCurrentTime() {
    return (double)clock() / CLOCKS_PER_SEC;
}

// Measure interval and update last interval time
double measureInterval(double lastIntervalTime) {
    double currentTime = getCurrentTime();
    double interval = currentTime - lastIntervalTime;
    // Return current time as new last interval time
    return interval;
}

void initWheelFlux(WheelFlux* wf, int hz, int channel, int muxChannel, float r,
                   int numTeeth, WHEEL_LOCATION w) {
    initAnalogSensor(&wf->base, "WheelFlux", hz, channel);
    wf->base.base.update = updateWheelFlux;
    wf->radius = r;
    wf->wheel_location = w;
    wf->numTeeth = numTeeth;
    wf->pulses = 0;
    wf->interval = getCurrentTime();
}

int getNumTeeth(WheelFlux* wf) {
    return wf->numTeeth;
}

/*
 * Gets wheel radius in meters
 *
 * @return Radius of wheel in meters.
 */
float getWheelRadius(WheelFlux* wf) {
    return wf->radius;
}

WHEEL_LOCATION getWheelLocation(WheelFlux* wf) {
    return wf->wheel_location;
}

// TODO: Find some way to update the pulse variable with the analog code.
int getPulseCount(WheelFlux* wf) {
    return wf->pulses;
}

// TODO: Have stm32 clock update the time for interval
double getTimeInterval(WheelFlux* wf) {
    double interval = measureInterval(wf->interval);
    wf->interval = getCurrentTime();
    return interval;
}

/*
 * Set time interval.
 * For testing and debugging.
 *
 * @param wf A pointer to the WheelFlux structure.
 * @param interval Time interval to set.
 */
void setTimeInterval(WheelFlux* wf, double interval) {
    wf->interval = interval;
}

// TODO: Perhaps replace with update functionality
void addPulse(WheelFlux* wf, int num) {
    wf->pulses += num;
}

void resetPulseCount(WheelFlux* wf) {
    wf->pulses = 0;
}

void resetTimeInterval(WheelFlux* wf) {
    wf->interval = 0.0f;
}

// I'm pretty sure the updating for this sensor will be done though pulses and
// time interval
void updateWheelFlux(void* wf) {
    /*WheelFlux *wheelFlux = (WheelFlux *)wf;*/
    /*float rawData = 0.0f; // Placeholder for sensor reading*/
    printf("Implement WheelFlux Update\n");
    /*wheelFlux->flux = (int)transferFunctionWheelFlux(rawData);*/
}

float transferFunctionWheelFlux(float rawVal) {
    // Implementation of the transfer function
    // For now, it just returns the raw value
    printf("Implement WheelFlux Transfer Function\n");
    return rawVal;
}
