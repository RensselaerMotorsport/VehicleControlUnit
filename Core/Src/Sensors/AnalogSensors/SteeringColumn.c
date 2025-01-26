#include "../../../Inc/Sensors/AnalogSensors/SteeringColumn.h"

#include <stdio.h>

void initSteeringColumn(SteeringColumn* sc, int hz, int channel, float zeroOffset) {
    initAnalogSensor(&sc->base, "SteeringColumn", hz, channel);
    sc->rotation_angle = -1;
    sc->rawData = 0.0f;
    sc->zero_offset = zeroOffset;
    sc->base.sensor.updateable.update = updateSteeringColumn;
    sc->base.sensor.updateable.context = sc;
    sc->base.sensor.updateable.log = logSteeringColumn;
}

float getSteeringColumnRotationAngle(SteeringColumn* sc) {
    return sc->rotation_angle;
}

void updateSteeringColumn(void* sc) {
    SteeringColumn *steeringColumn = (SteeringColumn *)sc;
    float rawData = 0.0f; // This should come from sensor read function or simulation
    printf("Implement SteeringColumn Update.\n");
    steeringColumn->rawData = rawData;
    steeringColumn->rotation_angle
        = transferFunctionSteeringColumn(rawData + steeringColumn->zero_offset);
}

void logSteeringColumn(void* sc) {
    SteeringColumn *mySc = (SteeringColumn *)sc;
    char logEntry[LOG_ENTRY_SIZE];

    snprintf(logEntry, LOG_ENTRY_SIZE, "Steering Column, %.2f, %.2f",
             mySc->rawData, mySc->rotation_angle);
    logToBuffer(logEntry);
}

float transferFunctionSteeringColumn(float rawVal) {
    // Placeholder for actual transfer function
    printf("Implement SteeringColumn::transfer_function\n");
    return rawVal;
}
