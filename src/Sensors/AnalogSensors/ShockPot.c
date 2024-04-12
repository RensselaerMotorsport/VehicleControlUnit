#include "../../../includes/ShockPot.h"
#include <math.h>
#include <stdio.h>

void initShockPot(ShockPot* shockPot, int hz, WHEEL_LOCATION location) {
    initAnalogSensor(&shockPot->base, "ShockPot", hz);
    shockPot->distance = -1;
    shockPot->wheel_location = location;
    shockPot->base.base.update = updateShockPot;
}

float getShockPotDistance(ShockPot* shockPot) {
    return shockPot->distance;
}

void updateShockPot(void* shockPot) {
    ShockPot *myShockPot = (ShockPot *)shockPot;
    printf("Implement ShockPot::update\n");
    float rawData = 50.0;
    myShockPot->distance = transferFunctionShockPot(rawData);
}

float transferFunctionShockPot(float rawVal) {
    if (rawVal < 15) {
        printf("ShockPot::transfer_function rawValue is too low\n");
        return -1;
    } else if (rawVal > 135) {
        printf("ShockPot::transfer_function rawValue is too high\n");
        return -1;
    }
    return 2.71965 * pow(rawVal, 0.837683) - 16.2622;
}
