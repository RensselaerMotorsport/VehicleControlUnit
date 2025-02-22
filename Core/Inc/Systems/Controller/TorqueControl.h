#ifndef RENSSELAERMOTORSPORT_TORQUE_CONTROL_H
#define RENSSELAERMOTORSPORT_TORQUE_CONTROL_H

#include "../ControllerSystem.h"
#include "../../Utils/Constants.h"
#include "Apps.h"

typedef enum {
    TORQUE_OK,
    TORQUE_RTD,
    TORQUE_LOW,
    TORQUE_OVER_LIMIT,
    TORQUE_UNDER_LIMIT,
    TORQUE_SENSOR_ERROR,
    TORQUE_BRAKE_FIGHT
} TorqueStatus;

typedef struct {
    ControllerSystem base;
    Apps *apps;
    float desiredTorque;
    float maxAllowedTorque;
    TorqueStatus status;
} TorqueControl;

/**
 * @brief Initializes the Torque Control Actuator with initial settings.
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param hz Rate at which the sensor is called (in hz).
 * @param maxTorque The maximum torque limit set for the system (in Nm).
 */
void initTorqueControl(TorqueControl* tc, Apps* apps, int hz, float maxTorque);

/**
 * @brief Starts the Torque Control Actuator.
 *
 * @param tc A pointer to the TorqueControl structure.
 * @return _SUCCESS if the actuator was started, _FAILURE otherwise.
 */
int startTorqueControl(TorqueControl* tc);

/**
 * @brief Sets the desired torque for the Torque Control Actuator.
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param torque The desired torque to be set (in Nm).
 * @return _SUCCESS if the desired torque was set, _FAILURE otherwise.
 */
int setDesiredTorque(void* self);

#endif // RENSSELAERMOTORSPORT_TORQUE_CONTROL_H
