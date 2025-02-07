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
        return FAILURE;
    }
    ENABLE(bp->base.system);
    return SUCCESS;
}

// Check the BrakePolice MonitorSystem
int checkBrakePolice(void* bp) {
    BrakePolice* bpPtr = (BrakePolice*)bp;
    BrakeSystemControl* bsc = bpPtr->bsc;
    if (bsc->status != BRAKES_OK) {
      return FAILURE;
    } 
    
    float front = getBrakePressure(bsc -> frontPressure);
    float rear = getBrakePressure(bsc -> rearPressure);
    float temp = getTemperatureFahrenheit(bsc -> temperature);
    if (front > bsc -> maxPressure || rear > bsc -> maxPressure){
      return PRESSURE_OVER_LIMIT;
    }
    else if (front > bsc -> minPressure || rear > bsc -> minPressure){
        return PRESSURE_UNDER_LIMIT;
    }
    else if (temp > bsc -> maxTemperatureAllowed){
        return TEMPERATURE_OVER_LIMIT;
    }
    else if (temp < 0){
        return TEMPERATURE_SENSOR_ERROR;
    }
    return SUCCESS;
}
