#ifndef RENNSSELAERMOTORSPORT_DIGITALSENSOR_H
#define RENNSSELAERMOTORSPORT_DIGITALSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor sensor;
    int port;
} DigitalSensor;

/**
 * @brief Initializes a digital sensor with the given parameters.
 *
 * @param digitalsensor Pointer to the Sensor structure to initialize.
 * @param name   Pointer to a string containing the name of the sensor.
 * @param hz     The frequency in Hertz at which the sensor operates.
 * @param port   Location of the digital sensor on board.
 */
void initDigitalSensor(DigitalSensor* digitalsensor, const char* name, int hz,
                       int port);

#endif // RENNSSELAERMOTORSPORT_DIGITALSENSOR_H
