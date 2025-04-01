#ifndef RENSSELAERMOTORSPORT_SENSOR_H
#define RENSSELAERMOTORSPORT_SENSOR_H

#include <stdio.h>
#include <stdlib.h>

#include "../Utils/Updateable.h"

typedef enum {
    s_ANALOG,
    s_DIGITAL,
} SensorType;

typedef struct {
    Updateable updateable;
    SensorType type;
    void* child; // Pointer to the child struct
} Sensor;

/**
 * @brief Initializes a sensor with the given parameters.
 *
 * @param sensor Pointer to the Sensor structure to initialize.
 * @param name   Pointer to a string containing the name of the sensor.
 * @param hz     The frequency in Hertz at which the sensor operates.
 * @param type   The type of the sensor as defined by the SensorType enum.
 * @param child  Pointer to the child struct.
 */
void initSensor(Sensor* sensor, const char* name, int hz, SensorType type, void* child);

#endif // RENSSELAERMOTORSPORT_SENSOR_H
