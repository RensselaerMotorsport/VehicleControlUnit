#ifndef RENNSSELAERMOTORSPORT_BMSDATA_H
#define RENNSSELAERMOTORSPORT_BMSDATA_H

#include <stdbool.h>

typedef struct {
    float chargeCurrentLimit;
    float dischargeCurrentLimit;
    float temperature;
    float stateOfCharge;
    float cellResistance;
    float packResistance;
    float cellVoltage;
    float packVoltage;
    bool criticalFaults;
} BmsData;

void initBmsData(BmsData* data, float ccl, float dcl, float t, float sc, 
                 float cr, float pr, float cv, float pv, bool cf);
char* toStringBmsData(const BmsData* data);

#endif
