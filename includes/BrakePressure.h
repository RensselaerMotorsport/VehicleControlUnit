#ifndef RENSSELAERMOTORSPORT_BRAKEPRESSURE_H
#define RENSSELAERMOTORSPORT_BRAKEPRESSURE_H

#include "AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float pressure;
} BrakePressure;

void initBrakePressure(BrakePressure* bp, int hz, int channel);
float getBrakePressure(BrakePressure* bp);
void updateBrakePressure(void* bp);
float transferFunctionBrakePressure(float rawVal);

#endif
