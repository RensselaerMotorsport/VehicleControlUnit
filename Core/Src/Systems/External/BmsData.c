#include "../../../Inc/Sensors/CANSensors/BmsData.h"

#include <stdio.h>  // For sprintf

void initBmsData(BmsData* data, float ccl, float dcl, float t, float sc,
                 float cr, float pr, float cv, float pv, bool cf) {
    if (data) {
        data->chargeCurrentLimit = ccl;
        data->dischargeCurrentLimit = dcl;
        data->temperature = t;
        data->stateOfCharge = sc;
        data->cellResistance = cr;
        data->packResistance = pr;
        data->cellVoltage = cv;
        data->packVoltage = pv;
        data->criticalFaults = cf;
    }
}

char* toStringBmsData(const BmsData* data) {
    static char str[256];
    if (data) {
        // sprintf(str, "%f,%f,%f,%f,%f,%f,%f,%f,%d",
                // data->chargeCurrentLimit,
                // data->dischargeCurrentLimit,
                // data->temperature,
                // data->stateOfCharge,
                // data->cellResistance,
                // data->packResistance,
                // data->cellVoltage,
                // data->packVoltage,
                // data->criticalFaults);
    }
    return str;
}
