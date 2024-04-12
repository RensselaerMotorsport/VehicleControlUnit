#include "../../../includes/WheelSpeed.h"
#include <stdio.h>

void initWheelSpeed(WheelSpeed* ws, WheelFlux* flux) {
    ws->flux = *flux;
    ws->speed = 0.0f;
}

float translateFluxToSpeed(WheelSpeed* ws) {
    // Placeholder for converting flux to speed
    printf("Implement translation of flux to speed\n");
    // Assuming some calculation based on flux data
    return ws->speed;
}
