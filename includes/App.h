#ifndef RENSSELAERMOTORSPORT_APP_H
#define RENSSELAERMOTORSPORT_APP_H

#include "AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float position;
} App;

void initApp(App* app, int hz);
float getAppPosition(App* app);
void updateApp(void* app);
float transferFunctionApp(float rawVal);

#endif
