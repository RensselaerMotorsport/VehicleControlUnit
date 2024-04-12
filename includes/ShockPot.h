#ifndef RENSSELAERMOTORSPORT_SHOCKPOT_H
#define RENSSELAERMOTORSPORT_SHOCKPOT_H

#include "AnalogSensor.h"

typedef enum {
    FRONT_LEFT,
    FRONT_RIGHT,
    BACK_LEFT,
    BACK_RIGHT
} WHEEL_LOCATION;

typedef struct {
    AnalogSensor base;
    float distance;
    WHEEL_LOCATION wheel_location;
} ShockPot;

void initShockPot(ShockPot* shockPot, int hz, int channel, WHEEL_LOCATION location);
float getShockPotDistance(ShockPot* shockPot);
void updateShockPot(void* shockPot);
float transferFunctionShockPot(float rawVal);

#endif
