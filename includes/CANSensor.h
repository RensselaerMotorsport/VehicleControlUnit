#ifndef RENSSELAERMOTORSPORT_CANSENSOR_H
#define RENSSELAERMOTORSPORT_CANSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor sensor;
} CANSensor;

void initCANSensor(CANSensor* cansensor, const char* name, int hz);

#endif
