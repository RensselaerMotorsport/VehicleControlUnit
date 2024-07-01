#ifndef RENSSELAERMOTORSPORT_ANALOGSENSOR_H
#define RENSSELAERMOTORSPORT_ANALOGSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor base;
    int channel;
} AnalogSensor;

void initAnalogSensor(AnalogSensor* sensor, const char* name, int hz, int channel);

int getAnalogSensorData(AnalogSensor* sensor);

#endif
