#include "../../Inc/Sensors/DigitalSensor.h"
#include <stdio.h>  // For printf

void initDigitalSensor(DigitalSensor* sensor, const char* name, int hz, int port) {
    initSensor(&sensor->base, name, hz);
    sensor->port = port;
}

// You could add a function to get data from the digital sensor if needed:
// int getDigitalSensorData(DigitalSensor* sensor) {
//     printf("Retrieving digital data from port: %d\n", sensor->port);
//     // Placeholder for actual data retrieval logic
//     // This would interface with hardware-specific APIs
//     return 0; // Default return for demonstration
// }

