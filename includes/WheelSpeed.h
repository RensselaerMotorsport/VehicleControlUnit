#ifndef RENSSELAERMOTORSPORT_WHEELSPEED_H
#define RENSSELAERMOTORSPORT_WHEELSPEED_H

#include "DigitalSensor.h"
#include "WheelFlux.h"

typedef struct {
    DigitalSensor base;
    WheelFlux* flux;
    float speed;
} WheelSpeed;

void initWheelSpeed(WheelSpeed* ws, WheelFlux* flux, int hz, int port);
float translateFluxToSpeed(WheelSpeed* ws);
void updateWheelSpeed(void* ws);

#endif
