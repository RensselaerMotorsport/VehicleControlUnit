#include "../../../Inc/Systems/Monitor/TorquePolice.h"
#include "../../../Inc/Utils/Common.h"

// Make new TorquePolice MonitorSystem
void initTorquePolice(TorquePolice* tp, TorqueControl *tc, BrakeSystemControl *bsc, RTD* rtd, int hz, float maxTorque) {
    initMonitorSystem(&tp->base, "Torque Police", hz, m_TORQUE, VEHICLE_SHUTDOWN, checkTorquePolice);
    tp->maxAllowedTorque = maxTorque;
    tp->status = TORQUE_OK;
    tp->torqueControl = tc;
    tp->brakeSystemControl = bsc;
    tp->rtd = rtd;
}

// Start the TorquePolice MonitorSystem
int startTorquePolice(TorquePolice* tp) {
    if (tp->base.runMonitor == NULL) {
        printf("Monitor function not set for TorquePolice\n");
        return _FAILURE;
    }
    ENABLE(tp->base.system);
    return _SUCCESS;
}

// Check the TorquePolice MonitorSystem
int checkTorquePolice(void* tp) {
    TorquePolice* tpPtr = (TorquePolice*)tp;
    TorqueControl* tc = tpPtr->torqueControl;
    BrakeSystemControl* bsc = tpPtr->brakeSystemControl;
    RTD* rtd = tpPtr->rtd;

    if (rtd->readyToDrive == 0) {
        tpPtr->status = TORQUE_RTD;
        tc->desiredTorque = 0;
        #ifdef DEBUGn
        printf("RTD is not ready to drive, reducing torque to zero\r\n");
        #endif
        return _FAILURE;
    } 
    if (tc->desiredTorque > tpPtr->maxAllowedTorque) {
        tpPtr->status = TORQUE_OVER_LIMIT;
        return _FAILURE;
    } else if (tc->status == TORQUE_SENSOR_ERROR) {
        tpPtr->status = TORQUE_SENSOR_ERROR;
        return _FAILURE;
    }

    if (bsc->base.state=c_validated && bsc->heavyBraking == 1) {
        tpPtr->status = TORQUE_BRAKE_FIGHT;
        printf("Brake System is in heavy braking, reducing torque to zero\r\n");
        tc->desiredTorque = 0;
        // Brake fight, reduce torque to zero. Not a system failure, but a safety measure.
        // Thus, return _SUCCESS.s
        return _SUCCESS;
    }

    if (tc->desiredTorque < TORQUE_LOW_MARGIN) {
        tpPtr->status = TORQUE_LOW;
        printf("Desired torque (%f) is too low, reducing to zero\r\n", tc->desiredTorque);
        tc->desiredTorque = 0;
    } else {
        tpPtr->status = TORQUE_OK;
    }

    #ifdef DEBUGn
    printf("Actual Torque: %f\r\n", tc->desiredTorque);
    #endif
    return _SUCCESS;
}
