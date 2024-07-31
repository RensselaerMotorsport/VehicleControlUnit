#ifndef RENSSELAERMOTORSPORT_TORQUE_CONTROL_H
#define RENSSELAERMOTORSPORT_TORQUE_CONTROL_H

#include "../ControllerSystem.h"

#define TORQUE_ERROR_MARGIN 0.05 // TODO: Determine correct value for the margin, or change to lookup table if needed.

// TODO: Move to torque police in the future
typedef enum {
    TORQUE_OK,
    TORQUE_OVER_LIMIT,
    TORQUE_UNDER_LIMIT,
    TORQUE_SENSOR_ERROR
} TorqueStatus;

typedef struct {
    ControllerSystem base;
    float actualTorque;
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
void initTorqueControl(TorqueControl* tc, int hz, float maxTorque);

/**
 * @brief Sets the desired torque for the Torque Control Actuator.
 * @note NOTE: Should this be a map?
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param torque The desired torque to be set (in Nm).
 */
void setDesiredTorque(TorqueControl* tc, float torque);

/*
 * Sets the actual torque for Torque Control Actuator
 * For testing and debugging.
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param torque The torque to be set (in Nm).
 */
void setActualTorque(TorqueControl* tc, float torque);

/**
 * @brief Updates the Torque Control Actuator based on sensor inputs and calculations.
 *
 * @param tc A pointer to the TorqueControl structure.
 */
void updateTorqueControl(void* tc);


/**
 * TODO: move into torque police
 *
 * @brief Checks if the actual torque is within the defined limits and desired range.
 * @note NOTE: When should each torque condition be in place?
 *
 * @param tc A pointer to the TorqueControl structure.
 * @return The actual status of the torque.
 */
TorqueStatus checkTorqueLimits(TorqueControl* tc);

// TODO: Add doc / add function in if applicable
void sendTorqueCommand(const TorqueControl* tc);

/**
 * @brief Checks the safety of the torque with multiple monitors.
 *
 * @param tc A pointer to the TorqueControl structure.
 */
int torqueSafetyCheck(void* tc);

#endif // RENSSELAERMOTORSPORT_TORQUE_CONTROL_H
