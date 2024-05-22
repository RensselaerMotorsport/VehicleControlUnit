#ifndef RENNSSELAERMOTORSPORT_BMS_H
#define RENNSSELAERMOTORSPORT_BMS_H

#include "../CANSensor.h"
#include "BmsData.h"

typedef struct {
    CANSensor base;
    BmsData data;
} Bms;

void initBms(Bms* bms, int hz);
void updateBms(void* bms);
BmsData transferFunctionBms(float* rawdata);

#endif
