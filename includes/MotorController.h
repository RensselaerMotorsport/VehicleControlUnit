#ifndef RENNSSELAERMOTORSPORT_MOTORCONTROLLER_H
#define RENNSSELAERMOTORSPORT_MOTORCONTROLLER_H

#include "Esc.h"
#include "App.h"

typedef struct {
    Esc* esc;
    App* app;
} MotorController;

void initMotorController(MotorController* mc, Esc* esc, App* app);
float getPedalPosition(const MotorController* mc);
float getMotorTemp(const MotorController* mc);

#endif

