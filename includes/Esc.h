#ifndef RENNSSELAERMOTORSPORT_ESC_H
#define RENNSSELAERMOTORSPORT_ESC_H

#include "CANSensor.h"

typedef struct {
    CANSensor base;
    float temp;
    float motor_revolutions;
} Esc;

void initEsc(Esc* esc, int hz);
float getEscTemp(const Esc* esc);
float getEscMotorRevolutions(const Esc* esc);
void updateEsc(void* esc);
char* toStringEsc(const Esc* esc);

#endif
