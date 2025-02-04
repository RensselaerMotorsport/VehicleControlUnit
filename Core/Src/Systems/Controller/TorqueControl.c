#include "../../../Inc/Systems/Controller/TorqueControl.h"
#include "../../../Inc/Utils/Common.h"

void initTorqueControl(TorqueControl* tc, int hz, float maxTorque) {
    initControllerSystem(&tc->base, "Torque Control", hz, c_TORQUE);
    tc->base.system.updateable.update = updateTorqueControl;
    tc->actualTorque = 0;
    tc->desiredTorque = 0;
    tc->maxAllowedTorque = maxTorque;
    tc->base.safety = torqueSafetyCheck;
}

int startTorqueControl(TorqueControl* tc) {
    if (tc->base.safety == NULL) {
        printf("Safety system not set for Torque Control\n");
        return FAILURE;
    }
    else if (tc->base.safety(tc) == FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return FAILURE;
    }
    ENABLE(tc->base.system);
    return SUCCESS;
}

void setDesiredTorque(TorqueControl* tc, float torque) {
    if (tc->base.safety(tc) == FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return;
    }
    if (torque > tc->maxAllowedTorque) {
        printf("Desired torque exceeds the maximum allowed torque\n");
        return;
    }
    tc->desiredTorque = torque;
}

void setActualTorque(TorqueControl* tc, float torque) {
    tc->actualTorque = torque;
}

void updateTorqueControl(void* tc) {
    TorqueControl* tcPtr = (TorqueControl*)tc;
    if (tcPtr->base.safety(tc) == FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return;
    }
    tcPtr->status = checkTorqueLimits(tc);
    if (tcPtr->status != TORQUE_OK) {
        printf("Torque Control Actuator is not in OK\n");
        return;
    }
    // TODO: Implement some way to send the torque where it needs to go
    // sendTorqueCommand(tc);
}
