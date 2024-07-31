#ifndef RENSSELAERMOTORSPORT_WHEELSPEED_H
#define RENSSELAERMOTORSPORT_WHEELSPEED_H

#include "../DigitalSensor.h"
#include "../../Utils/WheelLocation.h"

typedef struct {
    DigitalSensor base;
    WHEEL_LOCATION wheel_location;
    float speed;
    float radius;
    float interval;
    int numTeeth;
    int pulses;
} WheelSpeed;

/**
 * @brief Initialization function for a wheel speed sensor.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @pram hz Rate at which the sensor is called (in hz).
 * @pram port Location of sensor.
 * @pram radius Radius of wheel (in mm).
 * @pram numTeeth Number of teeth on reluctor wheel.
 * @pram location Location of sensor on car.
 */
void initWheelSpeed(WheelSpeed* ws, int hz, int port, float radius, int numTeeth,
                    WHEEL_LOCATION location);
/**
 * @brief Translates data to speed in mph.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @return The speed in miles per hour.
 */
float calculateSpeed(WheelSpeed* ws);

/**
 * @brief Updated the speed var in this sensor.
 *
 * @param ws A pointer to the WheelSpeed structure.
 */
void updateWheelSpeed(void* ws);

/**
 * @brief Set time interval.
 * @note For testing and debugging.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @param interval Time interval to set.
 */
void setTimeInterval(WheelSpeed* ws, float interval);

/**
 * @brief Add pulses to sensor;
 * @note For testing and debugging.
 *
 * @param ws A pointer to the WheelSpeed structure.
 * @param num Number of pulses to add.
 */
void addPulse(WheelSpeed* wf, int num);

#endif // RENSSELAERMOTORSPORT_WHEELSPEED_H
