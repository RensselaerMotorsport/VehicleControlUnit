#ifndef RENSSELAERMOTORSPORT_SENSOR_H
#define RENSSELAERMOTORSPORT_SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/Updateable.h"

typedef enum {
    ANALOG,
    DIGITAL,
    CAN
} SensorType;

typedef struct {
    Updateable updateable;
    SensorType type;
} Sensor;

/**
 * @brief Initializes a sensor with the given parameters.
 *
 * @param sensor Pointer to the Sensor structure to initialize.
 * @param name   Pointer to a string containing the name of the sensor.
 * @param hz     The frequency in Hertz at which the sensor operates.
 * @param type   The type of the sensor as defined by the SensorType enum.
 */
void initSensor(Sensor* sensor, const char* name, int hz, SensorType type);

#endif // RENSSELAERMOTORSPORT_SENSOR_H
