#ifndef RENNSSELAERMOTORSPORT_DIGITALSENSOR_H
#define RENNSSELAERMOTORSPORT_DIGITALSENSOR_H

#include "Sensor.h"
#include "../Utils/Constants.h"

#include <stdint.h>

typedef struct {
    Sensor sensor;
    int port;
} DigitalSensor;

extern uint8_t digital_in_buffer[NUM_DIGITAL_INPUTS];

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

/**
 * @brief Gets the reading from the digital sensor.
 * 
 * @param digitalsensor Pointer to the Sensor structure to get the reading from.
 * @return int The reading from the digital sensor.
 */
int getDigitalSensorReading(DigitalSensor* digitalsensor);
#endif // RENNSSELAERMOTORSPORT_DIGITALSENSOR_H
