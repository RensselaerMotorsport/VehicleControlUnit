#include "../../Inc/Sensors/AnalogSensor.h"
#include <stdio.h>

void initAnalogSensor(AnalogSensor* sensor, const char* name, int hz, int channel) {
    initSensor(&sensor->base, name, hz);
    sensor->channel = channel;
}

// TODO: Make a function to retrieve data
int getAnalogSensorData(AnalogSensor* sensor) {
    // Placeholder for sensor data retrieval logic
    // Would normally interface with hardware here
    return 0; // Default return
}
