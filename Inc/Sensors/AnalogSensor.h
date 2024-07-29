#ifndef RENSSELAERMOTORSPORT_ANALOGSENSOR_H
#define RENSSELAERMOTORSPORT_ANALOGSENSOR_H

#include "Sensor.h"

typedef struct {
    Sensor sensor;
    int channel;
} AnalogSensor;


/**
 * @brief Initializes an analog sensor.
 *
 * @param analogSensor Pointer to the analog sensor to initialize.
 * @param name Name of the sensor.
 * @param hz Sampling frequency in Hertz.
 * @param channel Analog channel number.
 */
void initAnalogSensor(AnalogSensor* analogsensor, const char* name, int hz, int channel);

/**
 * @brief Retrieves data from the analog sensor.
 *
 * @param analogSensor Pointer to the analog sensor.
 * @return int Data from the analog sensor.
 */
int getAnalogSensorData(AnalogSensor* analogsensor);

#endif
