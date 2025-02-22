#include "../../../Inc/Sensors/AnalogSensors/BrakePressure.h"

#include <stdio.h>

// Constants for brake pressure calculations
static const float kOffsetVoltage = 0.5; // Volts
static const float kVoltsPerPSIA = 0.002;
static const float kLowOutputSaturation = 0.45;  // Output saturation for undersupplied sensor
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
    float rawData = getAnalogSensorData(&brakePressure->base);
    
    #ifdef DEBUGn
    printf("BrakePressure::update rawVal: %f\r\n", rawData);
    #endif
    brakePressure->pressure = transferFunctionBrakePressure(rawData);
}

float transferFunctionBrakePressure(float rawVal) {
    if (rawVal < kLowOutputSaturation) {
        
        #ifdef DEBUGn
        printf("BrakePressure::transfer_function rawVal is too low\r\n");
        #endif

        return -1;
    }
    else if (rawVal > kHighOutputSaturation) {
        
        #ifdef DEBUGn
        printf("BrakePressure::transfer_function rawVal is too high\r\n");
        #endif

        return 2001;
    }
    else {
        return ((rawVal - kOffsetVoltage) / kVoltsPerPSIA);
    }
}
