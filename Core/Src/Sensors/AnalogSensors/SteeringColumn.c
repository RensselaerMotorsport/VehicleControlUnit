#include "../../../Inc/Sensors/AnalogSensors/SteeringColumn.h"

#include <stdio.h>

void initSteeringColumn(SteeringColumn* sc, int hz, int channel, float zeroOffset) {
    initAnalogSensor(&sc->base, "SteeringColumn", hz, channel, sc);
    sc->rotation_angle = -1;
    sc->zero_offset = zeroOffset;
    sc->base.sensor.updateable.update = updateSteeringColumn;
}

float getSteeringColumnRotationAngle(SteeringColumn* sc) {
    return sc->rotation_angle;
}

int updateSteeringColumn(Updateable* updateable) {
    // Cast the updateable pointer to a steering column sensor
    Sensor* sensor = (Sensor*)updateable->child;
    AnalogSensor* analogSensor = (AnalogSensor*)sensor->child;
    SteeringColumn* steeringColumn = (SteeringColumn*)analogSensor->child;

    // Check if the pointer is null
    if (steeringColumn == NULL) {
        fprintf(stderr, "Error: Null pointer passed to updateSteeringColumn\n");
        return _FAILURE;
    }

    float rawData = 0.0f; // This should come from sensor read function or simulation
    printf("Implement SteeringColumn Update.\n");
    steeringColumn->rotation_angle
        = transferFunctionSteeringColumn(rawData + steeringColumn->zero_offset);

    return _SUCCESS;
}

float transferFunctionSteeringColumn(float rawVal) {
    // Placeholder for actual transfer function
    printf("Implement SteeringColumn::transfer_function\n");
    return rawVal;
}
