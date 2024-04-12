#include "AnalogSensor.h"
#include <stdio.h>

void initAnalogSensor(AnalogSensor* sensor, const char* name, int hz) {
    initSensor(&sensor->base, name, hz);
}

// TODO: Make a function to retrieve data
// UDOUBLE getAnalogSensorData(AnalogSensor* sensor) {
//     // Placeholder for sensor data retrieval logic
//     // Would normally interface with hardware here
//     return 0; // Default return
// }
