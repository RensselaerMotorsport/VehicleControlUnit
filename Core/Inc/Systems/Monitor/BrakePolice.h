#ifndef RENSSELAERMOTORSPORT_BRAKE_POLICE_H
#define RENSSELAERMOTORSPORT_BRAKE_POLICE_H

#include "../MonitorSystem.h"
#include "../Controller/BrakeSystemControl.h"

typedef struct {
    MonitorSystem base;
    int maxTemperatureAllowed;
    BrakeSystemStatus status;
    BrakeSystemControl* bsc;
} BrakePolice;

void initBrakePolice(BrakePolice* bp, BrakeSystemControl *bsc, int hz, int maxTemp);
int startBrakePolice(BrakePolice* bp);
int checkBrakePolice(void* bp);

#endif // RENSSELAERMOTORSPORT_BRAKE_POLICE_H
