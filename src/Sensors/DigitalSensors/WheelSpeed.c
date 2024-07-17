#include "../../../includes/WheelSpeed.h"
#include "../../../includes/TimeUtils.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * Initialization function for a wheel speed sensor.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @pram hz Rate at which the sensor is called (in hz).
 * @pram port Location of sensor.
 * @pram radius Radius of wheel (in mm).
 * @pram numTeeth Number of teeth on reluctor wheel.
 * @pram location Location of sensor on car.
 */
void initWheelSpeed(WheelSpeed* ws, int hz, int port, float radius, int numTeeth,
                    WHEEL_LOCATION location) {
    initDigitalSensor(&ws->base, "Wheel Speed", hz, port);
    ws->base.sensor.updateable.update = updateWheelSpeed;
    ws->radius = radius;
    ws->wheel_location = location;
    ws->numTeeth = numTeeth;
    ws->pulses = 0;
    ws->speed = 0.0f;
    ws->interval = getCurrentTime();
}

// TODO: Perhaps add class for conversions.
float MPS_TO_MPH = 2.236936;

/**
 * Converts from meters per second to miles per hour
 *
 * @param metersPerSecond The speed to convert in meters per second.
 * @return The speed in miles per hour.
 */
float convertMpsToMph(float metersPerSecond) {
    return metersPerSecond * MPS_TO_MPH;
}

/**
 * Translates data to speed in mph.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @return The speed in miles per hour.
 */
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

/*
 * Updated the speed var in this sensor.
 *
 * @param ws A pointer to the WheelSpeed structure.
 */
void updateWheelSpeed(void* ws) {
    WheelSpeed* wsPtr = (WheelSpeed*)ws;
    wsPtr->speed = calculateSpeed(ws);
}


/*
 * Add pulses to sensor;
 * For testing and debugging.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @param num Number of pulses to add.
 */
void addPulse(WheelSpeed* ws, int num) {
    ws->pulses += num;
}

/*
 * Set time interval.
 * For testing and debugging.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @param interval Time interval to set.
 */
void setTimeInterval(WheelSpeed* ws, float interval) {
    ws->interval = interval + getCurrentTime();
}
