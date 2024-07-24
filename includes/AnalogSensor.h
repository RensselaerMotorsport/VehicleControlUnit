#ifndef RENSSELAERMOTORSPORT_ANALOGSENSOR_H
#define RENSSELAERMOTORSPORT_ANALOGSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor sensor;
    int channel;
} AnalogSensor;

void initAnalogSensor(AnalogSensor* analogsensor, const char* name, int hz, int channel);

int getAnalogSensorData(AnalogSensor* analogsensor);

#endif
