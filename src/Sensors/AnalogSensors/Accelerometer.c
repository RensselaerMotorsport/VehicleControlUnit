#include "../../../includes/Accelerometer.h"
#include <stdio.h>

const float kOffsetVolts = 2.5; // 2.5 V at 0 g
const float kVoltsPerG = 0.440; // 440 mV/g

void initAccelerometer(Accelerometer* accel, int channel, int hz, int muxChannel) {
    initAnalogSensor(&accel->base, "Accelerometer", hz, channel);
    accel->value = -1;
    accel->base.base.update = updateAccelerometer;
}

float getAccelerometerValue(Accelerometer* accel) {
    return accel->value;
}

void updateAccelerometer(void* accel) {
    Accelerometer *myAccel = (Accelerometer *)accel;
    float rawData = 0.0f; // Placeholder for sensor reading
    printf("Updating Accelerometer\n");
    myAccel->value = transferFunctionAccelerometer(rawData);
}

float transferFunctionAccelerometer(float rawVal) {
    return (rawVal - kOffsetVolts) / kVoltsPerG;
}
