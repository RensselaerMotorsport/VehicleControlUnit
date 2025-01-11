#ifndef RENSSELAERMOTORSPORT_BRAKE_SYSTEM_CONTROL_H
#define RENSSELAERMOTORSPORT_BRAKE_SYSTEM_CONTROL_H

#include "../ControllerSystem.h"

typedef enum{
    BRAKES_OK,
    PRESSURE_UNDER_LIMIT,
    PRESSURE_OVER_LIMIT,
    PRESSURE_SENSOR_ERROR,
    TEMPERATURE_OVER_LIMIT,
    TEMPERATURE_SENSOR_ERROR
} BrakeSystemStatus;

typedef struct{
    ControllerSystem base;
    float minPressure;
    float maxPressure;
    float frontPressure;
    float rearPressure;
    float temperature;
    float maxTemperatureAllowed;
    BrakeSystemStatus status;
    float brakeLightActivationPoint;
    int brakeLightActive;
    float heavyBrakingActivationPoint;
    int heavyBraking;
} BrakeSystemControl;

//Will probably update the pressure vars so it works with psi and not voltage so it's easier to understand
//^As well as the temp sensor
/**
 * @brief Initializes the Braking System with initial settings.
 *
 * @param bsc A pointer to the BrakeControl structure.
 * @param hz Rate at which the sensorors are called (in hz).
 * @param maxTemp The maximum temperature limit set for the system (in celcius?).
 * @param brakeLightActivationPoint At what voltage the brake light will activate.
 * @param heavyBrakingActivationPoint At what voltage the system recognises "heavy braking".
 */
void initBrakeSystemControl(BrakeSystemControl *bsc, int hz, int maxTemp, float brakeLightActivationPoint, float heavyBrakingActivationPoint);

/**
 * @brief Updates BrakeSystemStatus with current sensor data.
 *
 * @param bsc A pointer to the BrakeControl structure.
 * @param frontPressure The voltage reading from the front brake line pressure sensor.
 * @param rearPressure The voltage reading from the rear brake line pressure sensor.
 * @param temperature The voltage reading from the brake rotor temperature sensor.
 */
void setSensorReadings(BrakeSystemControl *bsc, float frontPressure, float rearPressure, float temperaure);

/**
 * @brief Checks whether the line pressure is higher than the activation point specified by the user.
 *
 * @param bsc A pointer to the BrakeControl structure.
 */
void activateBrakeLight(BrakeSystemControl *bsc);

/**
 * @brief Checks whether the line pressure is higher than the activation point specified by the user.
 *
 * @param bsc A pointer to the BrakeControl structure.
 */
void inHeavyBreaking(BrakeSystemControl *bsc);

/**
 * @brief Checks if the brake system is within the defined limits and desired ranges.
 *
 * @param bsc A pointer to the BrakeControl structure.
 */
BrakeSystemStatus checkSensorLimits(BrakeSystemControl *bsc);

/**
 * @brief Checks the saftey of the braking system.
 *
 * @param bsc A pointer to the BrakeControl structure.
 */
int brakeSafteyCheck(void* bsc);

#endif