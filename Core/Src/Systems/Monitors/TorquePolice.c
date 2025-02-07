#include "../../../Inc/Systems/Monitor/TorquePolice.h"
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
    ENABLE(tp->base.system);
    return SUCCESS;
}

// Check the TorquePolice MonitorSystem
int checkTorquePolice(void* tp) {
    TorquePolice* tpPtr = (TorquePolice*)tp;
    TorqueControl* tc = tpPtr->torqueControl;
    if (tc->desiredTorque > tpPtr->maxAllowedTorque) {
        tpPtr->status = TORQUE_OVER_LIMIT;
        return FAILURE;
    } else if (tc->status == TORQUE_SENSOR_ERROR) {
        tpPtr->status = TORQUE_SENSOR_ERROR;
        return FAILURE;
    }
    return SUCCESS;
}
