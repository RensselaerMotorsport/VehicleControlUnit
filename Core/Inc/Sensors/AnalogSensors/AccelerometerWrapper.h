#ifndef RENNSMOTORSPORT_ACCELEROMETERWRAPPER_H
#define RENNSMOTORSPORT_ACCELEROMETERWRAPPER_H

#include "Accelerometer.h"

typedef struct {
    Accelerometer x;
    Accelerometer y;
    Accelerometer z;
} AccelerometerWrapper;

/**
 * @brief Initializes the AccelerometerWrapper with the given accelerometers.
 *
 * @param wrapper Pointer to the AccelerometerWrapper structure to initialize.
 * @param ax      Pointer to the x-axis Accelerometer.
 * @param ay      Pointer to the y-axis Accelerometer.
 * @param az      Pointer to the z-axis Accelerometer.
 */
void initAccelerometerWrapper(AccelerometerWrapper* wrapper,
                              Accelerometer* ax,
                              Accelerometer* ay,
                              Accelerometer* az);

#endif // RENNSMOTORSPORT_ACCELEROMETERWRAPPER_H
