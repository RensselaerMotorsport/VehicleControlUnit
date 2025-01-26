#include "../../../Inc/Sensors/AnalogSensors/Accelerometer.h"

#include <stdio.h>

const float kOffsetVolts = 2.5; // 2.5 V at 0 g
const float kVoltsPerG = 0.440; // 440 mV/g

void initAccelerometer(Accelerometer* accel, int channel, int hz, int muxChannel) {
    initAnalogSensor(&accel->base, "Accelerometer", hz, channel);
    accel->rawData = 0.0f;
    accel->value = -1;
    accel->base.sensor.updateable.update = updateAccelerometer;
    accel->base.sensor.updateable.context = accel;
    accel->base.sensor.updateable.log = logAccelerometer;
}

float getAccelerometerValue(Accelerometer* accel) {
    return accel->value;
}

void updateAccelerometer(void* accel) {
    Accelerometer *myAccel = (Accelerometer *)accel;
    float rawData = 0.0f; // Placeholder for sensor reading
    printf("Updating Accelerometer\n");
    myAccel->rawData = rawData;
    myAccel->value = transferFunctionAccelerometer(rawData);
}

void logAccelerometer(void* accel) {
    Accelerometer *myAccel = (Accelerometer *)accel;
    char logEntry[LOG_ENTRY_SIZE];

    snprintf(logEntry, LOG_ENTRY_SIZE, "Accelerometer, %.2f, %.2f",
             myAccel->rawData, myAccel->value);
    logToBuffer(logEntry);
}

float transferFunctionAccelerometer(float rawVal) {
    return (rawVal - kOffsetVolts) / kVoltsPerG;
}
