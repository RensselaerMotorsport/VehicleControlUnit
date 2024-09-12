#ifndef RENSSELAERMOTORSPORT_STEERINGCOLUMN_H
#define RENSSELAERMOTORSPORT_STEERINGCOLUMN_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float rotation_angle;
    float zero_offset;
} SteeringColumn;

/**
 * @brief Initializes the SteeringColumn sensor with the given frequency,
 * channel, and zero offset.
 *
 * @param sc         Pointer to the SteeringColumn structure to initialize.
 * @param hz         The frequency in Hertz at which the sensor operates.
 * @param channel    The analog channel the sensor is connected to.
 * @param zeroOffset The zero offset value for the steering column.
 */
void initSteeringColumn(SteeringColumn* sc, int hz, int channel, float zeroOffset);

/**
 * @brief Gets the current rotation angle of the steering column.
 *
 * @param sc Pointer to the SteeringColumn structure to query.
 * @return   The current rotation angle of the steering column.
 */
float getSteeringColumnRotationAngle(SteeringColumn* sc);

/**
 * @brief Updates the SteeringColumn data.
 *
 * @param sc Pointer to the SteeringColumn structure to update.
 */
void updateSteeringColumn(void* sc);

/**
 * @brief Converts raw steering column data to a meaningful rotation angle.
 *
 * @param rawVal The raw data from the steering column sensor.
 * @return       The converted rotation angle.
 */
float transferFunctionSteeringColumn(float rawVal);

#endif // RENSSELAERMOTORSPORT_STEERINGCOLUMN_H
