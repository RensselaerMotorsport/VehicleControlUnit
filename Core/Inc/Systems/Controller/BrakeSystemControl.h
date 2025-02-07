#ifndef RENSSELAERMOTORSPORT_BRAKE_SYSTEM_CONTROL_H
#define RENSSELAERMOTORSPORT_BRAKE_SYSTEM_CONTROL_H

#include "../ControllerSystem.h"
#include "../../Sensors/AnalogSensors/BrakePressure.h"
#include "../../Sensors/AnalogSensors/Temperature.h"

typedef enum {
    BRAKES_OK,
    PRESSURE_UNDER_LIMIT,
    PRESSURE_OVER_LIMIT,
    PRESSURE_SENSOR_ERROR,
    TEMPERATURE_OVER_LIMIT,
    TEMPERATURE_SENSOR_ERROR
} BrakeSystemStatus;

typedef struct{
    ControllerSystem base;
    int minPressure;
    int maxPressure;
    BrakePressure *frontPressure;
    BrakePressure *rearPressure;
    Temperature *temperature;
    int maxTemperatureAllowed;
    BrakeSystemStatus status;
    int brakeLightActivationPoint;
    int brakeLightActive;
    int heavyBrakingActivationPoint;
    int heavyBraking;
} BrakeSystemControl;

//Will probably update the pressure vars so it works with psi and not voltage so it's easier to understand
//^As well as the temp sensor
/**
 * @brief Initializes the Braking System with initial settings.
 *
 * @param bsc A pointer to the BrakeControl structure.
 * @param hz Rate at which the sensorors are called (in hz).
 * @param maxTemp The maximum temperature limit set for the system (in farenheight).
 * @param brakeLightActivationPoint At what voltage the brake light will activate.
 * @param heavyBrakingActivationPoint At what voltage the system recognises "heavy braking".
 * @param fbp_channel The channel number for the front brake pressure sensor
 * @param rbp_channel The channel number for the rear brake pressure sensor
 * @param temp_channel The channel number for the temperature sensor
 */
void initBrakeSystemControl(BrakeSystemControl *bsc, int hz, int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, int fbp_channel, int rbp_channel, int temp_channel);

/**
 * @brief Updates BrakeSystemStatus with current sensor data.
 *
 * @param bsc A pointer to the BrakeControl structure.
 */
void setSensorReadings(BrakeSystemControl *bsc);

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

// The following functions below are for testing functionality and should not be used elsewhere
void setFrontPressure(BrakeSystemControl *bsc, float pressure);
void setRearPressure(BrakeSystemControl *bsc, float pressure);
void setTemperature(BrakeSystemControl *bsc, float temperature);

#endif