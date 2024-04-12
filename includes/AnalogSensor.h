#ifndef RENSSELAERMOTORSPORT_ANALOGSENSOR_H
#define RENSSELAERMOTORSPORT_ANALOGSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor base;
} AnalogSensor;

void initAnalogSensor(AnalogSensor* sensor, const char* name, int hz);
// UDOUBLE getAnalogSensorData(AnalogSensor* sensor);

#endif
