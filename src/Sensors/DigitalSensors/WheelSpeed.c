#include "../../../includes/WheelSpeed.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * Initalation function for a wheel speed sensor.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @pram flux A pointer to the WheelFlux structure.
 * @pram hz Rate at which the sensor is called (in hz)
 * @pram port Location of sensor
 */
void initWheelSpeed(WheelSpeed* ws, WheelFlux* flux, int hz, int port) {
    initDigitalSensor(&ws->base, "Wheel Speed", hz, port);
    ws->base.base.update = updateWheelSpeed;
    ws->flux = flux;
    ws->speed = 0.0f;
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
 * Translates wheel flux data to speed in mph.
 * 
 * @param ws A pointer to the WheelSpeed structure.
 * @return The speed in miles per hour.
 */
float translateFluxToSpeed(WheelSpeed* ws) {
    WheelFlux* flux = ws->flux;

    if (ws == NULL || flux == NULL) {
        // Error handling: No flux detected
        printf("Error: No flux detected.\n");
        return 0.0f;
    }
    
    int  numTeeth = getNumTeeth(flux);
    float radius = getWheelRadius(flux);
    float pulses = getPulseCount(flux);
    double delta = getTimeInterval(flux);

    // Verify inputs
    if (numTeeth <= 0 || radius <= 0.0f || pulses <= 0 || delta <= 0.0) {
        printf("Error: Invalid argument.\n");
        return 0.0f;
    }

    // Calculations
    float circumference = 2 * M_PI * radius;
    float distancePerPulse = circumference / numTeeth;
    float speedMetersPerSecond = (distancePerPulse * pulses) / delta;
    int speed = convertMpsToMph(speedMetersPerSecond);

    // Reset pulse count and elapsed time for the next measurement interval
    resetPulseCount(flux);
    resetTimeInterval(flux);

    return speed;
}

/*
 * Updated the speed var in this sensor.
 *
 * @param ws A pointer to the WheelSpeed structure.
 */
void updateWheelSpeed(void* ws) {
    WheelSpeed* wsPtr = (WheelSpeed*)ws;
    wsPtr->speed = translateFluxToSpeed(ws);
}
