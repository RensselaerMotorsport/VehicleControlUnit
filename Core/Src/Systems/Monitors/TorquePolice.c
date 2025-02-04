#include "../../../Inc/Systems/Controller/TorquePolice.h"
#include "../../../Inc/Utils/Common.h"

// Make new TorquePolice MonitorSystem
void initTorquePolice(TorquePolice* tp, TorqueControl *tc, int hz, float maxTorque) {
    initMonitorSystem(&tp->base, "Torque Police", hz, m_TORQUE, VEHICLE_SHUTDOWN, checkTorquePolice);
    tp->maxAllowedTorque = maxTorque;
    tp->status = TORQUE_OK;
    tp->torqueControl = tc;
}

// Start the TorquePolice MonitorSystem
int startTorquePolice(TorquePolice* tp) {
    if (tp->base.runMonitor == NULL) {
        printf("Monitor function not set for TorquePolice\n");
        return FAILURE;
    }
    else if (tp->base.runMonitor(tp) == FAILURE) {
        printf("Torque Police is not in a safe state\n");
        return FAILURE;
    }
    ENABLE(tp->base.system);
    return SUCCESS;
}

// Check the TorquePolice MonitorSystem
int checkTorquePolice(void* tp) {
    TorquePolice* tpPtr = (TorquePolice*)tp;
    if (tpPtr->torqueControl->status != TORQUE_OK) {
        printf("Torque Control Actuator is not in a safe state\n");
        return FAILURE;
    }
    if (tpPtr->torqueControl->desiredTorque > tpPtr->maxAllowedTorque) {
        printf("Desired torque exceeds the maximum allowed torque\n");
        return FAILURE;
    }
    return SUCCESS;
}