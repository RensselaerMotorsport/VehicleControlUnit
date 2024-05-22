#ifndef RENSSELAERMOTORSPORT_TEMPERATURE_H
#define RENSSELAERMOTORSPORT_TEMPERATURE_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    double degrees;
} Temperature;

void initTemperature(Temperature* temp, int hz, int channel);
double getTemperatureCelsius(Temperature* temp);
double getTemperatureFahrenheit(Temperature* temp);
double transferFunction(Temperature* temp, int rawValue);
void updateTemperature(void* temp);

#endif
