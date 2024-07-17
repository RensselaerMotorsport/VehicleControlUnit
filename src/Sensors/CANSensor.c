#include "../../includes/CANSensor.h"
#include <stdio.h>  // For printf

void initCANSensor(CANSensor* cansensor, const char* name, int hz) {
    initSensor(&cansensor->sensor, name, hz, CAN);
}

// Function to retrieve data from CAN sensor
// int getCANSensorData(CANSensor* sensor) {
//     printf("Retrieving data for CAN sensor ID: %d\n", sensor->id);
//     // Implementation depends on specific project requirements
//     return 0; // Placeholder return
// }
