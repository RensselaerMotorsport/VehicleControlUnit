#include "../../../Inc/Sensors/AnalogSensors/SteeringColumn.h"
#include "../../../Inc/Utils/Common.h"

#include <stdio.h>

void initSteeringColumn(SteeringColumn* steeringColumn, int hz, int channel, float zeroOffset) {
    initAnalogSensor(&steeringColumn->base, "SteeringColumn", hz, channel, steeringColumn);
    steeringColumn->rotation_angle = -1;
    steeringColumn->zero_offset = zeroOffset;
    steeringColumn->base.sensor.updateable.update = updateSteeringColumn;
}

float getSteeringColumnRotationAngle(SteeringColumn* steeringColumn) {
    return steeringColumn->rotation_angle;
}

int updateSteeringColumn(SteeringColumn* steeringColumn) {
    // Check if the pointer is null
    if (steeringColumn == NULL) {
        //printf(stderr, "Error: Null pointer passed to updateSteeringColumn\n");
        return _FAILURE;
    }

    float rawData = 0.0f; // This should come from sensor read function or simulation
    //printf("Implement SteeringColumn Update.\n");
    steeringColumn->rotation_angle
        = transferFunctionSteeringColumn(rawData + steeringColumn->zero_offset);

    return _SUCCESS;
}

float transferFunctionSteeringColumn(float rawVal) {
    // Placeholder for actual transfer function
    //printf("Implement SteeringColumn::transfer_function\n");
    return rawVal;
}
