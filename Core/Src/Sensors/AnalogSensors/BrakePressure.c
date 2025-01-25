#include "../../../Inc/Sensors/AnalogSensors/BrakePressure.h"

#include <stdio.h>

// Constants for brake pressure calculations
static const float kOffsetVoltage = 0.201; // Volts
static const float kVoltsPerPSIA = 0.002;
static const float kLowOutputSaturation = 0.35;  // Output saturation for undersupplied sensor
static const float kHighOutputSaturation = 4.65; // Output saturation for oversupplied sensor

void initBrakePressure(BrakePressure* bp, int hz, int channel) {
    initAnalogSensor(&bp->base, "BrakePressure", hz, channel);
    bp->pressure = -1;
    bp->base.sensor.updateable.update = updateBrakePressure;
}

float getBrakePressure(BrakePressure* bp) {
    return bp->pressure;
}

void updateBrakePressure(void* bp) {
    BrakePressure *brakePressure = (BrakePressure *)bp;
    float rawData = 0.0f; // This should come from sensor read function or simulation
    printf("Implement BrakePressure Update\n");
    brakePressure -> pressure = (rawData - kOffsetVoltage) / kVoltsPerPSIA;
}

float transferFunctionBrakePressure(float rawVal) {//not nessecary because BrakeSystemControl already checks if the sensors are within the specified bounds
    if (rawVal < kLowOutputSaturation) {
        printf("BrakePressure::transfer_function rawVal is too low\n");
        return 0;
    }
    else if (rawVal > kHighOutputSaturation) {
        printf("BrakePressure::transfer_function rawVal is too high\n");
        return 115;
    }
    else {
        return ((rawVal - kOffsetVoltage) / kVoltsPerPSIA);
    }
}
