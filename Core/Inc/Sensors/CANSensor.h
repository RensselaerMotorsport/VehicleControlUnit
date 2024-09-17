#ifndef RENSSELAERMOTORSPORT_CANSENSOR_H
#define RENSSELAERMOTORSPORT_CANSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor sensor;
} CANSensor;

/**
 * @brief Initializes a CAN sensor with the given parameters.
 *
 * @param canSensor Pointer to the Sensor structure to initialize.
 * @param name   Pointer to a string containing the name of the sensor.
 * @param hz     The frequency in Hertz at which the sensor operates.
 */
void initCANSensor(CANSensor* canSensor, const char* name, int hz);

#endif // RENSSELAERMOTORSPORT_CANSENSOR_H
