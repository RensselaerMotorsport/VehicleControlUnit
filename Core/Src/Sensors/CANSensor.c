#include "../../Inc/Sensors/CANSensor.h"
#include <stdio.h>  // For printf

void initCANSensor(CANSensor* sensor, const char* name, int hz) {
    initSensor(&sensor->base, name, hz);
}

// Function to retrieve data from CAN sensor
// int getCANSensorData(CANSensor* sensor) {
//     printf("Retrieving data for CAN sensor ID: %d\n", sensor->id);
//     // Implementation depends on specific project requirements
//     return 0; // Placeholder return
// }
