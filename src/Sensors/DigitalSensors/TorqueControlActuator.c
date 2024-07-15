#include "../../../includes/TorqueControlActuator.h"

/*
 * Initializes the Torque Control Actuator with initial settings.
 *
 * @param tc A pointer to the TorqueControlActuator structure.
 * @pram hz Rate at which the sensor is called (in hz).
 * @pram port Location of sensor.
 * @param initialTorque The initial desired torque for the system (in Nm).
 * @param maxTorque The maximum torque limit set for the system (in Nm).
 */
void initTorqueControlActuator(TorqueControlActuator* tc, int hz, int port,
                               float maxTorque) {
    initDigitalSensor(&tc->base, "Torque Control Actuator", hz, port);
    tc->base.base.update = updateTorqueControlActuator;
    tc->currentTorque = 0;
    tc->desiredTorque = 0;
    tc->maxAllowedTorque = maxTorque;
    tc->status = TORQUE_OK;
}

/*
 * Sets the desired torque for the Torque Control Actuator.
 * NOTE: Should this be a map?
 *
 * @param tc A pointer to the TorqueControlActuator structure.
 * @param torque The desired torque to be set (in Nm).
 */
void setDesiredTorque(TorqueControlActuator* tc, float torque) {
    tc->desiredTorque = torque;
}

/*
 * Sets the current torque for Torque Control Actuator
 * For testing and debugging.
 *
 * @param tc A pointer to the TorqueControlActuator structure.
 * @param torque The torque to be set (in Nm).
 */
void setTorque(TorqueControlActuator* tc, float torque) {
    tc->currentTorque = torque;
}

/*
 * Updates the Torque Control Actuator based on sensor inputs and calculations.
 *
 * @param tc A pointer to the TorqueControlActuator structure.
 */
void updateTorqueControlActuator(void* tc) {
    TorqueControlActuator* tcPtr = (TorqueControlActuator*)tc;
    tcPtr->status = checkTorqueLimits(tc);
    sendTorqueCommand(tc);
}

/*
 * Checks if the current torque is within the defined limits and desired range.
 * NOTE: When should each torque condition be in place?
 *
 * @param tc A pointer to the TorqueControlActuator structure.
 * @return The current status of the torque.
 */
TorqueStatus checkTorqueLimits(TorqueControlActuator* tc) {
    if (tc->currentTorque > tc->maxAllowedTorque) {
        return TORQUE_OVER_LIMIT;
    } else if (tc->currentTorque < 0) {
        return TORQUE_UNDER_LIMIT;
    }

    float lowerBound = tc->desiredTorque * (1 - TORQUE_ERROR_MARGIN);
    float upperBound = tc->desiredTorque * (1 + TORQUE_ERROR_MARGIN);

    if (tc->currentTorque < lowerBound || tc->currentTorque > upperBound) {
        return TORQUE_SENSOR_ERROR;
    }

    return TORQUE_OK;
}

/*
 * NOTE: I cant really set this up till i know more about esc command.
 *
 * Sends the torque command to the ESC.
 *
 * @param tc A pointer to the TorqueControlActuator structure.
 */
void sendTorqueCommand(const TorqueControlActuator* tc) {
    // Send command to motor controller or ESC
    // Placeholder for actual communication logic
    if (tc->status == TORQUE_OK) {
        /*sendMotorCommand(tc->currentTorque);*/
    } else {
        // Handle error or limit exceeded condition
        /*sendMotorCommand(0);  // Safety: cut-off torque in case of an issue*/
    }
}
