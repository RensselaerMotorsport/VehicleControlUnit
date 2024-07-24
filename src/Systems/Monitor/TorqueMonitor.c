#include "../../../includes/TorqueMonitor.h"
#include "../../../includes/Common.h"

void initTorqueControl(TorqueControl* tc, int hz, float maxTorque) {
    initControllerSystem(&tc->base, "Torque Control", hz, c_TORQUE);
    tc->base.system.updateable.update = updateTorqueControl;
    tc->actualTorque = 0;
    tc->desiredTorque = 0;
    tc->maxAllowedTorque = maxTorque;
    tc->status = TORQUE_OK;
    tc->base.safety = torqueSafetyCheck;
}