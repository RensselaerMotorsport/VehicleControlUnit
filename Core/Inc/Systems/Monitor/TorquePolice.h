#ifndef RENSSELAERMOTORSPORT_TORQUE_POLICE_H
#define RENSSELAERMOTORSPORT_TORQUE_POLICE_H

#include "../MonitorSystem.h"
#include "../Controller/BrakeSystemControl.h"
#include "../Controller/TorqueControl.h"
#include "../Controller/RTD.h"

#define TORQUE_ERROR_MARGIN 0.05 // TODO: Determine correct value for the margin, or change to lookup table if needed.

typedef struct {
    MonitorSystem base;
    float maxAllowedTorque;
    TorqueStatus status;
    TorqueControl* torqueControl;
    BrakeSystemControl* brakeSystemControl;
    RTD* rtd;
} TorquePolice;

void initTorquePolice(TorquePolice* tp, TorqueControl *tc, BrakeSystemControl *bsc, RTD* rtd, int hz, float maxTorque);
int startTorquePolice(TorquePolice* tp);
int checkTorquePolice(void* tp);

#endif // RENSSELAERMOTORSPORT_TORQUE_POLICE_H