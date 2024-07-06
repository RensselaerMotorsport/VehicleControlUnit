#ifndef RENSSELAERMOTORSPORT_ESCINDICATOR_H
#define RENSSELAERMOTORSPORT_ESCINDICATOR_H

#include "DigitalSensor.h"
#include "stdbool.h"

typedef struct {
    DigitalSensor base;
    bool alive;
} ESCIndicator;

void initECSIndicator(ESCIndicator* esc);
void updateECSInicator(void* esc);
bool getAlive(ESCIndicator* esc);

#endif
