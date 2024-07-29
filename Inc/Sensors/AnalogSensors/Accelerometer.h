#ifndef RENNSMOTORSPORT_ACCELEROMETER_H
#define RENNSMOTORSPORT_ACCELEROMETER_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float value;
} Accelerometer;

void initAccelerometer(Accelerometer* accel, int channel, int hz, int muxChannel);
float getAccelerometerValue(Accelerometer* accel);
void updateAccelerometer(void* accel);
float transferFunctionAccelerometer(float rawVal);

#endif
