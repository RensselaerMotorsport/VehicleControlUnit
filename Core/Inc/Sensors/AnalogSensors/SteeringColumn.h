#ifndef RENSSELAERMOTORSPORT_STEERINGCOLUMN_H
#define RENSSELAERMOTORSPORT_STEERINGCOLUMN_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float rotation_angle;
    float zero_offset;
} SteeringColumn;

void initSteeringColumn(SteeringColumn* sc, int hz, int channel, float zeroOffset);
float getSteeringColumnRotationAngle(SteeringColumn* sc);
void updateSteeringColumn(void* sc);
float transferFunctionSteeringColumn(float rawVal);

#endif
