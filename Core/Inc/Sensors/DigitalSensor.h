#ifndef RENNSSELAERMOTORSPORT_DIGITALSENSOR_H
#define RENNSSELAERMOTORSPORT_DIGITALSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor base;
    int port;
} DigitalSensor;

void initDigitalSensor(DigitalSensor* sensor, const char* name, int hz, int port);

#endif
