#include "../../../includes/Imd.h"
#include <stdio.h>  // For printf

void initImd(Imd* imd, int hz) {
    initCANSensor(&imd->base, "Imd", hz);
    imd->imd_status = false;
    imd->running_flag = false;
    imd->base.sensor.updateable.update = updateImd;
}

bool getImdStatus(const Imd* imd) {
    return imd->imd_status;
}

bool getRunningFlag(const Imd* imd) {
    return imd->running_flag;
}

void resetRunningFlag(Imd* imd) {
    imd->running_flag = true;  // Assuming reset means setting to true
}

void updateImd(void* imd) {
    Imd* myImd = (Imd*) imd;
    // float var = myImd->base.getData();
    // myImd->imd_status = imdStatusTransferFunction(var);
    // myImd->running_flag = runningFlagTransferFunction(var);
    // printf("IMD Status: %d, Running Flag: %d\n", myImd->imd_status, myImd->running_flag);
    printf("Implement updateImd\n");
}

bool imdStatusTransferFunction(float x) {
    return x > 0.5; // TODO: Implement
}

bool runningFlagTransferFunction(float x) {
    return x > 0.5; // TODO: Implement
}

char* toStringImd(const Imd* imd) {
    static char str[32];
    sprintf(str, "%d,%d", imd->imd_status, imd->running_flag);
    return str;
}
