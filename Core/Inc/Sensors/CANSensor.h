#ifndef RENSSELAERMOTORSPORT_CANSENSOR_H
#define RENSSELAERMOTORSPORT_CANSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor base;
} CANSensor;

void initCANSensor(CANSensor* sensor, const char* name, int hz);
// Placeholder for data retrieval function
// int getCANSensorData(CANSensor* sensor);

#endif
