#include "../../../includes/TorqueControl.h"
#include "../../../includes/Common.h"

/*
 * Initializes the Torque Control Actuator with initial settings.
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param hz Rate at which the sensor is called (in hz).
 * @param maxTorque The maximum torque limit set for the system (in Nm).
 */
void initTorqueControl(TorqueControl* tc, int hz, float maxTorque) {
    initControllerSystem(&tc->base, "Torque Control", hz, c_TORQUE);
    tc->base.system.updateable.update = updateTorqueControl;
    tc->actualTorque = 0;
    tc->desiredTorque = 0;
    tc->maxAllowedTorque = maxTorque;
    tc->status = TORQUE_OK;
    tc->base.safety = torqueSafetyCheck;
}

int startTorqueControl(TorqueControl* tc) {
    if (tc->base.safety == NULL) {
        printf("Safety system not set for Torque Control\n");
        return FAILURE;
    } else if (tc->base.safety(tc) == FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return FAILURE;
    }
    ENABLE(tc->base.system);
    return SUCCESS;
}
/*
 * Sets the desired torque for the Torque Control Actuator.
 * NOTE: Should this be a map?
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param torque The desired torque to be set (in Nm).
 */
void setDesiredTorque(TorqueControl* tc, float torque) {
    if (tc->safety(tc) == FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return;
    }
    if (torque > tc->maxAllowedTorque) {
        printf("Desired torque exceeds the maximum allowed torque\n");
        return;
    }
    tc->desiredTorque = torque;
}

/*
 * Sets the actual torque for Torque Control Actuator
 * For testing and debugging.
 *
 * @param tc A pointer to the TorqueControl structure.
 * @param torque The torque to be set (in Nm).
 */
void setActualTorque(TorqueControl* tc, float torque) {
    tc->actualTorque = torque;
}

/*
 * Updates the Torque Control Actuator based on sensor inputs and calculations.
 *
 * @param tc A pointer to the TorqueControl structure.
 */
void updateTorqueControl(void* tc) {
    if (tc->safety(tc) == FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return;
    }
    TorqueControl* tcPtr = (TorqueControl*)tc;
    tcPtr->status = checkTorqueLimits(tc);
    if (tcPtr->status != TORQUE_OK) {
        printf("Torque Control Actuator is not in OK\n");
        return;
    }
    // sendTorqueCommand(tc); // TODO: Implement some way to send the torque where it needs to go
}

//TODO: move into torque police
/*
 * Checks if the actual torque is within the defined limits and desired range.
 * NOTE: When should each torque condition be in place?
 *
 * @param tc A pointer to the TorqueControl structure.
 * @return The actual status of the torque.
 */
TorqueStatus checkTorqueLimits(TorqueControl* tc) {
    if (tc->actualTorque > tc->maxAllowedTorque) {
        return TORQUE_OVER_LIMIT;
    } else if (tc->actualTorque < 0) {
        return TORQUE_UNDER_LIMIT;
    }

    float lowerBound = tc->desiredTorque * (1 - TORQUE_ERROR_MARGIN);
    float upperBound = tc->desiredTorque * (1 + TORQUE_ERROR_MARGIN);

    if (tc->actualTorque < lowerBound || tc->actualTorque > upperBound) {
        return TORQUE_SENSOR_ERROR;
    }

    return TORQUE_OK;
}

int torqueSafetyCheck(void* tc) {
    TorqueControl* tcPtr = (TorqueControl*)tc;
    if(tc->base.num_monitors == 0) {
        printf("No monitors set for Torque Control\n");
        return FAILURE;
    } else if (tc->TorqueStatus != TORQUE_OK) {
        printf("Torque Control Actuator is not in a safe state\n");
        return FAILURE;
    }
    return SUCCESS;
}