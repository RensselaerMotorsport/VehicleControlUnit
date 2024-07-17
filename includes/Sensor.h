#ifndef RENSSELAERMOTORSPORT_SENSOR_H
#define RENSSELAERMOTORSPORT_SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Updateable.h"

typedef enum {
    ANALOG,
    DIGITAL,
    CAN
} SensorType;

typedef struct {
    Updateable updateable;
    SensorType type;
} Sensor;

void initSensor(Sensor* sensor, const char* name, int hz, SensorType type);

#endif
