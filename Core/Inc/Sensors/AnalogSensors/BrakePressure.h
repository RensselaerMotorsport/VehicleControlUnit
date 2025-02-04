#ifndef RENSSELAERMOTORSPORT_BRAKEPRESSURE_H
#define RENSSELAERMOTORSPORT_BRAKEPRESSURE_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float pressure;
} BrakePressure;

/**
 * @brief Initializes the BrakePressure sensor with the given frequency and channel.
 *
 * @param bp      Pointer to the BrakePressure structure to initialize.
 * @param hz      The frequency in Hertz at which the sensor operates.
 * @param channel The analog channel the sensor is connected to.
 */
void initBrakePressure(BrakePressure* bp, int hz, int channel);

/**
 * @brief Gets the current brake pressure.
 *
 * @param bp Pointer to the BrakePressure structure to query.
 * @return   The current brake pressure.
 */
float getBrakePressure(BrakePressure* bp);

/**
 * @brief Updates the brake pressure data.
 *
 * @param bp Pointer to the BrakePressure structure to update.
 */
void updateBrakePressure(void* bp);


/**
 * @brief Converts raw brake pressure data to a meaningful pressure value.
 *
 * @param rawVal The raw data from the brake pressure sensor.
 * @return       The converted brake pressure value.
 */
float transferFunctionBrakePressure(float rawVal);

#endif // RENSSELAERMOTORSPORT_BRAKEPRESSURE_H
