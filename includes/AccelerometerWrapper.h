#ifndef RENNSMOTORSPORT_ACCELEROMETERWRAPPER_H
#define RENNSMOTORSPORT_ACCELEROMETERWRAPPER_H

#include "Accelerometer.h"

typedef struct {
    Accelerometer x;
    Accelerometer y;
    Accelerometer z;
} AccelerometerWrapper;

void initAccelerometerWrapper(AccelerometerWrapper* wrapper, 
                              Accelerometer* ax, 
                              Accelerometer* ay, 
                              Accelerometer* az);

#endif
