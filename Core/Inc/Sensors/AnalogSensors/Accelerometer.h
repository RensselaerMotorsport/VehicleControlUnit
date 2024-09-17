#ifndef RENNSMOTORSPORT_ACCELEROMETER_H
#define RENNSMOTORSPORT_ACCELEROMETER_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float value;
} Accelerometer;

/**
 * @brief Initializes the accelerometer with the given channel, frequency,
 * and multiplexer channel.
 *
 * @param accel      Pointer to the Accelerometer structure to initialize.
 * @param channel    The analog channel the accelerometer is connected to.
 * @param hz         The frequency in Hertz at which the accelerometer operates.
 * @param muxChannel The multiplexer channel for the accelerometer.
 */
void initAccelerometer(Accelerometer* accel, int channel, int hz, int muxChannel);

/**
 * @brief Gets the current value of the accelerometer.
 *
 * @param accel Pointer to the Accelerometer structure to query.
 * @return      The current accelerometer value.
 */
float getAccelerometerValue(Accelerometer* accel);

/**
 * @brief Updates the accelerometer data.
 *
 * @param accel Pointer to the Accelerometer structure to update.
 */
void updateAccelerometer(void* accel);

/**
 * @brief Converts raw accelerometer data to a meaningful value.
 *
 * @param rawVal The raw data from the accelerometer.
 * @return       The converted accelerometer value.
 */
float transferFunctionAccelerometer(float rawVal);

#endif // RENNSMOTORSPORT_ACCELEROMETER_H
