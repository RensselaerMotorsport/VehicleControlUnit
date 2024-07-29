#include "../../../includes/SteeringColumn.h"
#include <stdio.h>

void initSteeringColumn(SteeringColumn* sc, int hz, int channel, float zeroOffset) {
    initAnalogSensor(&sc->base, "SteeringColumn", hz, channel);
    sc->rotation_angle = -1;
    sc->zero_offset = zeroOffset;
    sc->base.sensor.updateable.update = updateSteeringColumn;
}

float getSteeringColumnRotationAngle(SteeringColumn* sc) {
    return sc->rotation_angle;
}

void updateSteeringColumn(void* sc) {
    SteeringColumn *steeringColumn = (SteeringColumn *)sc;
    float rawData = 0.0f; // This should come from sensor read function or simulation
    printf("Implement SteeringColumn Update.\n");
    steeringColumn->rotation_angle = transferFunctionSteeringColumn(rawData + steeringColumn->zero_offset);
}

float transferFunctionSteeringColumn(float rawVal) {
    // Placeholder for actual transfer function
    printf("Implement SteeringColumn::transfer_function\n");
    return rawVal; // Modify to apply actual transfer function
}
