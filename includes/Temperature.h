#ifndef RENSSELAERMOTORSPORT_TEMPERATURE_H
#define RENSSELAERMOTORSPORT_TEMPERATURE_H

#include "AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    double temp;
} Temperature;

void initTemperature(Temperature* temp, int hz);
double getTemperatureCelsius(Temperature* temp);
double getTemperatureFahrenheit(Temperature* temp);
double transferFunction(Temperature* temp, int rawValue);
void updateTemperature(void* temp);

#endif
