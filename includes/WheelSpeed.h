#ifndef RENSSELAERMOTORSPORT_WHEELSPEED_H
#define RENSSELAERMOTORSPORT_WHEELSPEED_H

#include "WheelFlux.h"

typedef struct {
    WheelFlux flux;
    float speed;  // Simplified from a DataLogger to a single float TODO:  Figure out
} WheelSpeed;

void initWheelSpeed(WheelSpeed* ws, WheelFlux* flux);
float translateFluxToSpeed(WheelSpeed* ws);

#endif
