#include "../../../Inc/Sensors/AnalogSensors/AccelerometerWrapper.h"

void initAccelerometerWrapper(AccelerometerWrapper* wrapper,
                              Accelerometer* ax,
                              Accelerometer* ay,
                              Accelerometer* az) {
    if (wrapper != NULL && ax != NULL && ay != NULL && az != NULL) {
        wrapper->x = *ax;
        wrapper->y = *ay;
        wrapper->z = *az;
    }
}
