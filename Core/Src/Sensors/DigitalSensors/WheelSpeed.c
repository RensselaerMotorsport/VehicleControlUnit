#include "../../../Inc/Sensors/DigitalSensors/WheelSpeed.h"
#include "../../../Inc/Utils/Conversions.h"
#include "../../../Inc/Utils/PlatformTime.h"
#include "../../../Inc/Systems/PrintHelpers.h"

#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void initWheelSpeed(WheelSpeed* ws, int hz, int port, float radius, int numTeeth,
                    WHEEL_LOCATION location) {
    initDigitalSensor(&ws->base, "Wheel Speed", hz, port);
    ws->base.sensor.updateable.update = updateWheelSpeed;
    ws->base.sensor.updateable.context = ws;
    ws->radius = radius;
    ws->wheel_location = location;
    ws->numTeeth = numTeeth;
    ws->pulses = 0;
    ws->speed = 0.0f;
    ws->interval = CURRENT_TIME_MS();
}

float calculateSpeed(WheelSpeed* ws) {
    // Check if the pointer is null
    if (ws == NULL) {
        fprintf(stderr, "Error: Null pointer passed to calculateSpeed\n");
        return -1; // Or handle the error as appropriate
    }

    int numTeeth = ws->numTeeth;
    float radius = ws->radius;
    int pulses = ws->pulses;
    float delta = measureInterval(ws->interval);

    // Reset pulse count and elapsed time for the next measurement interval
    ws->interval = CURRENT_TIME_MS();
    ws->pulses = 0;

    // Verify inputs
    if (numTeeth <= 0 || radius <= 0.0f || pulses <= 0 || delta <= 0.0) {
        fprintf(stderr, ANSI_COLOR_RED "Error: Invalid argument. "
            "NumTeeth = %d, Radius = %.2f, Pulses = %d, Time Delta = %.2f\n"
            ANSI_COLOR_RESET, numTeeth, radius, pulses, delta
        );
        return 0.0f;
    }

    // Calculations
    float circumference = 2 * M_PI * radius;
    float distancePerPulse = circumference / numTeeth;
    float speedMetersPerSecond = (distancePerPulse * pulses) / delta;
    float speed = convertMpsToMph(speedMetersPerSecond);

    return speed;
}

void updateWheelSpeed(void* ws) {
    if (ws == NULL) {
        printf(ANSI_COLOR_RED
            "Error: Null pointer passed to updateWheelSpeed.\n"
            ANSI_COLOR_RESET
        );
        return;
    }
    WheelSpeed* wsPtr = (WheelSpeed*)ws;
    wsPtr->speed = calculateSpeed(ws);
}

Updateable* GetUpdateableWheelSpeed(WheelSpeed* ws) {
    return &(ws->base.sensor.updateable);
}

void setTimeInterval(WheelSpeed* ws, float interval) {
    ws->interval = interval + CURRENT_TIME_MS();
}

void addPulse(WheelSpeed* ws, int num) {
    ws->pulses += num;
}
