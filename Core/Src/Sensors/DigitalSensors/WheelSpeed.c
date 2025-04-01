#include "../../../Inc/Sensors/DigitalSensors/WheelSpeed.h"
#include "../../../Inc/Utils/Conversions.h"
#include "../../../Inc/Utils/TimeUtils.h"
#include "../../../Inc/Utils/Common.h"

#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void initWheelSpeed(WheelSpeed* ws, int hz, int port, float radius, int numTeeth,
                    WHEEL_LOCATION location) {
    initDigitalSensor(&ws->base, "Wheel Speed", hz, port, ws);
    ws->base.sensor.updateable.update = updateWheelSpeed;
    ws->radius = radius;
    ws->wheel_location = location;
    ws->numTeeth = numTeeth;
    ws->pulses = 0;
    ws->speed = 0.0f;
    ws->interval = getCurrentTime();
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
    ws->interval = getCurrentTime();
    ws->pulses = 0;

    // Verify inputs
    if (numTeeth <= 0 || radius <= 0.0f || pulses <= 0 || delta <= 0.0) {
        printf("Error: Invalid argument. "
               "NumTeeth = %d, Radius = %.2f, Pulses = %d, Time Delta = %.2f\n",
               numTeeth, radius, pulses, delta);
        return 0.0f;
    }

    // Calculations
    float circumference = 2 * M_PI * radius;
    float distancePerPulse = circumference / numTeeth;
    float speedMetersPerSecond = (distancePerPulse * pulses) / delta;
    float speed = convertMpsToMph(speedMetersPerSecond);

    return speed;
}

int updateWheelSpeed(Updateable* updateable) {
    // Cast the updateable pointer to a wheel speed sensor
    Sensor* sensor = (Sensor*)updateable->child;
    DigitalSensor* digitalSensor = (DigitalSensor*)sensor->child;
    WheelSpeed* ws = (WheelSpeed*)digitalSensor->child;

    ws->speed = calculateSpeed(ws);
    if (ws->speed < 0) {
        printf("Error: Speed calculation failed\n");
        return _FAILURE;
    }
    return _SUCCESS;
}

void setTimeInterval(WheelSpeed* ws, float interval) {
    ws->interval = interval + getCurrentTime();
}

void addPulse(WheelSpeed* ws, int num) {
    ws->pulses += num;
}
