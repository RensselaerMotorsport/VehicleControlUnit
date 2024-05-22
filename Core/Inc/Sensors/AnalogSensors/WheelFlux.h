#ifndef RENNSSELAERMOTORSPORT_WHEELFLUX_H
#define RENNSSELAERMOTORSPORT_WHEELFLUX_H

#include "AnalogSensor.h"
#include <stdbool.h>

typedef enum {
    front_left, front_right, back_left, back_right
} WHEEL_LOCATION;

typedef struct {
    AnalogSensor base;
    bool flux;
    float radius;
    WHEEL_LOCATION wheel_location;
} WheelFlux;

void initWheelFlux(WheelFlux* wf, int hz, int channel, int muxChannel, float r, WHEEL_LOCATION w);
float getWheelFlux(WheelFlux* wf);
float getWheelRadius(WheelFlux* wf);
WHEEL_LOCATION getWheelLocation(WheelFlux* wf);
void updateWheelFlux(void* wf);
float transferFunctionWheelFlux(float rawVal);

#endif
