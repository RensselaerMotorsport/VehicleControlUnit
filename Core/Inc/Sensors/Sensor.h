#ifndef RENSSELAERMOTORSPORT_SENSOR_H
#define RENSSELAERMOTORSPORT_SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENSOR_NAME_LENGTH 25

typedef struct {
    char sensorName[MAX_SENSOR_NAME_LENGTH];
    int hz;
    // TODO: Unsure if id is needed
    void (*update)(void* self);
} Sensor;

void initSensor(Sensor* sensor, const char* name, int hz);
void defaultUpdate(void* self);
void writeDataToFileImplementation(const char* filename, void* self);

#endif
