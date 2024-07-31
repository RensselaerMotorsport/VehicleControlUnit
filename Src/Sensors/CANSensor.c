#include "../../Inc/Sensors/CANSensor.h"

void initCANSensor(CANSensor* canSensor, const char* name, int hz) {
    initSensor(&canSensor->sensor, name, hz, CAN);
}
