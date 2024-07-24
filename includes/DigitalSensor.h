#ifndef RENNSSELAERMOTORSPORT_DIGITALSENSOR_H
#define RENNSSELAERMOTORSPORT_DIGITALSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor sensor;
    int port;
} DigitalSensor;

void initDigitalSensor(DigitalSensor* digitalsensor, const char* name, int hz, int port);

#endif
