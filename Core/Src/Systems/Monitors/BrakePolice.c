#include "../../../Inc/Systems/Monitor/BrakePolice.h"
#include "../../../Inc/Utils/Common.h"

// Make new BrakePolice MonitorSystem
void initBrakePolice(BrakePolice* bp, BrakeSystemControl *bsc, int hz, int maxTemp) {
    initMonitorSystem(&bp->base, "Brake Police", hz, m_BRAKES, VEHICLE_SHUTDOWN, checkBrakePolice);
    bp->bsc = bsc;
    bp->maxTemperatureAllowed = maxTemp;
    bp->status = BRAKES_OK;
}

// Start the BrakePolice MonitorSystem
int startBrakePolice(BrakePolice* bp) {
    if (bp->base.runMonitor == NULL) {
        printf("Monitor function not set for BrakePolice\n");
        return _FAILURE;
    }
    ENABLE(bp->base.system);
    return _SUCCESS;
}

// Check the BrakePolice MonitorSystem
int checkBrakePolice(void* bp) {
    BrakePolice* bpPtr = (BrakePolice*)bp;
    BrakeSystemControl* bsc = bpPtr->bsc;
    bsc->status = checkSensorLimits(bsc);
    if (bsc->status != BRAKES_OK) {
      return _FAILURE;
    } 

    return _SUCCESS;
}
