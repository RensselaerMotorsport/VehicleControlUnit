#ifndef RENSSELAERMOTORSPORT_WHEELSPEED_H
#define RENSSELAERMOTORSPORT_WHEELSPEED_H

#include "DigitalSensor.h"
#include "WheelLocation.h"

typedef struct {
    DigitalSensor base;
    WHEEL_LOCATION wheel_location;
    float speed;
    float radius;
    float interval;
    int numTeeth;
    int pulses;
} WheelSpeed;

void initWheelSpeed(WheelSpeed* ws, int hz, int port, float radius, int numTeeth,
                    WHEEL_LOCATION location);
float calculateSpeed(WheelSpeed* ws);
void updateWheelSpeed(void* ws);
void setTimeInterval(WheelSpeed* ws, float interval);
void addPulse(WheelSpeed* wf, int num);

#endif
