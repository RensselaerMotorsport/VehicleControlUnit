#include "../../../Inc/Sensors/CANSensors/Bms.h"

#include <stdio.h>  // For printf

void initBms(Bms* bms, int hz) {
    if (!bms) {
        return;
    }

    initCANSensor(&bms->base, "Bms", hz);
    // Assume initBmsData initializes with default or zero values
    initBmsData(&bms->data, 0, 0, 0, 0, 0, 0, 0, 0, false);
    bms->base.sensor.updateable.update = updateBms;
}

void updateBms(void* bms) {
    Bms* myBms = (Bms*) bms;
    printf("Updating BMS\n");
    // Example: Fetch raw data somehow, here simplified as static data
    float rawData[] = {1.0, 1.0, 25.0, 50.0, 0.1, 0.1, 3.7, 48.0, true};
    myBms->data = transferFunctionBms(rawData);
    printf("BMS Data: %s\n", toStringBmsData(&myBms->data));
}

BmsData transferFunctionBms(float* rawData) {
    // Convert raw float array to BmsData struct
    // Assuming the array order matches BmsData constructor parameters
    return (BmsData){rawData[0], rawData[1], rawData[2], rawData[3],
                     rawData[4], rawData[5], rawData[6], rawData[7], rawData[8]};
}
