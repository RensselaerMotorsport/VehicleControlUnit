#include "../../../Inc/Sensors/AnalogSensors/WheelFlux.h"
#include <stdio.h>

void initWheelFlux(WheelFlux* wf, int hz, int channel, int muxChannel, float r, WHEEL_LOCATION w) {
    initAnalogSensor(&wf->base, "WheelFlux", hz, channel);
    wf->flux = false;  // Default value indicating no flux detected
    wf->radius = r;
    wf->wheel_location = w;
    wf->base.base.update = updateWheelFlux;
}

float getWheelFlux(WheelFlux* wf) {
    return (float)wf->flux;  // Cast to float if necessary
}

float getWheelRadius(WheelFlux* wf) {
    return wf->radius;
}

WHEEL_LOCATION getWheelLocation(WheelFlux* wf) {
    return wf->wheel_location;
}

void updateWheelFlux(void* wf) {
    WheelFlux *wheelFlux = (WheelFlux *)wf;
    float rawData = 0.0f; // Placeholder for sensor reading
    printf("Implement WheelFlux Update\n");
    wheelFlux->flux = (int)transferFunctionWheelFlux(rawData);
}

float transferFunctionWheelFlux(float rawVal) {
    // Implementation of the transfer function
    // For now, it just returns the raw value
    printf("Implement WheelFlux Transfer Function\n");
    return rawVal;
}
