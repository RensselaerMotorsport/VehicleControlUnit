#include "../../../Inc/Sensors/AnalogSensors/ShockPot.h"

#include <math.h>
#include <stdio.h>

void initShockPot(ShockPot* shockPot, int hz, int channel, WHEEL_LOCATION location) {
    initAnalogSensor(&shockPot->base, "ShockPot", hz, channel);
    shockPot->distance = -1;
    shockPot->rawData = 0.0f;
    shockPot->wheel_location = location;
    shockPot->base.sensor.updateable.update = updateShockPot;
    shockPot->base.sensor.updateable.context = shockPot;
    shockPot->base.sensor.updateable.log = logShockPot;
}

float getShockPotDistance(ShockPot* shockPot) {
    return shockPot->distance;
}

void updateShockPot(void* shockPot) {
    ShockPot *myShockPot = (ShockPot *)shockPot;
    printf("Implement ShockPot::update\n");
    float rawData = 50.0;
    myShockPot->rawData = rawData;
    myShockPot->distance = transferFunctionShockPot(rawData);
}

void logShockPot(void* shockPot) {
    ShockPot *mySp = (ShockPot *)shockPot;
    char logEntry[LOG_ENTRY_SIZE];

    snprintf(logEntry, LOG_ENTRY_SIZE, "ShockPot, %.2f, %.2f",
             mySp->rawData, mySp->distance);
    logToBuffer(logEntry);
}

float transferFunctionShockPot(float rawVal) {
    if (rawVal < 15) {
        printf("ShockPot::transfer_function rawValue is too low\n");
        return -1;
    }
    else if (rawVal > 135) {
        printf("ShockPot::transfer_function rawValue is too high\n");
        return -1;
    }
    return 2.71965 * pow(rawVal, 0.837683) - 16.2622;
}
