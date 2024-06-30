#ifndef RENNSSELAERMOTORSPORT_WHEELFLUX_H
#define RENNSSELAERMOTORSPORT_WHEELFLUX_H

#include "AnalogSensor.h"

typedef enum {
    front_left,
    front_right,
    back_left,
    back_right
} WHEEL_LOCATION;

typedef struct {
    AnalogSensor base;
    float radius;
    WHEEL_LOCATION wheel_location;
    int numTeeth;
    int pulses;
    float interval;
} WheelFlux;

void initWheelFlux(WheelFlux* wf, int hz, int channel, int muxChannel, float r,
                   int numTeeth, WHEEL_LOCATION w);
int getNumTeeth(WheelFlux* wf);
float getWheelRadius(WheelFlux* wf);
WHEEL_LOCATION getWheelLocation(WheelFlux* wf);
int getPulseCount(WheelFlux* wf);
double getTimeInterval(WheelFlux* wf);
void setTimeInterval(WheelFlux* wf, double interval);
void addPulse(WheelFlux* wf, int num);
void resetPulseCount(WheelFlux* wf);
void resetTimeInterval(WheelFlux* wd);
void updateWheelFlux(void* wf);
float transferFunctionWheelFlux(float rawVal);

#endif
