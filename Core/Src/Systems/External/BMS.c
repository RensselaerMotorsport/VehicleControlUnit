#include "../../../Inc/Systems/External/BMS.h"
#include "../../../Inc/Systems/DBCParser.h"


#include <stdio.h>  // For printf

void initBms(Bms* bms, int hz) {
    if (bms != NULL) {
        initExternalSystem(&bms->extSystem, "Bms", hz, 0);
        // Assume initBmsData initializes with default or zero values
        /*initBmsData(&bms->data, 0, 0, 0, 0, 0, 0, 0, 0, false);*/
        bms->extSystem.system.updateable.update = updateBms;
    }
}

BmsSignal lookupBmsSignal(const char* name) {
    for (int i = 0; bmsSignalMap[i].signalName != NULL; i++) {
        if (strcmp(bmsSignalMap[i].signalName, name) == 0) {
            return bmsSignalMap[i].signalType;
        }
    }
    return -1; // Invalid signal
}

void assignBmsValue(Bms* bms, BmsSignal type, float value) {
    // Define the array inside the function where `bms` is available
    void* bmsSignalPointers[] = {
        &bms->packVoltage,
        &bms->packCurrent,
        &bms->stateOfCharge,
        &bms->cellVoltageMin,
        &bms->cellVoltageMax,
        &bms->cellTemperatureMin,
        &bms->cellTemperatureMax,
        &bms->totalPackCapacity,
        &bms->remainingPackCapacity,
        &bms->packHealth,
        &bms->chargeStatus
    };

    // Assign values based on type
    if (type == BMS_CHARGE_STATUS) {
        *(uint8_t*)bmsSignalPointers[type] = (uint8_t)value; // Cast to uint8_t for chargeStatus
    } else {
        *(float*)bmsSignalPointers[type] = value; // Default cast to float for others
    }
}

bool bmsTransferFunction(Bms* bms, DBC* dbc) {
    Message** messages = getDbcMessages(dbc);
    for (Message** msg = messages; *msg != NULL; msg++) {
        Signal** signals = getSignals(*msg);
        for (Signal** sig = signals; *sig != NULL; sig++) {
            char* name = getSignalName(*sig);
            BmsSignal type = lookupBmsSignal(name);
            float value = extractSignalValue(*sig);
            assignBmsValue(bms, type, value);
        }
    }
    return true;
}

void updateBms(void* bms) {
    Bms* myBms = (Bms*) bms;
}

// TODO: make this as similar to the normal update as possible.
//
// @warning For testing and debugging purposes only
void updateBmsTest(void* bms, const char* dbcFilename) {
    Bms* myBms = (Bms*) bms;
    DBC dbc;
    if(!parseDbcFile(&dbc, dbcFilename)) {
        printf("Update Test Error: Couldn't parse dbc.\n");
        return;
    }
    if(!bmsTransferFunction(myBms, &dbc)) {
        printf("Update Test Error: Transfer function failed.\n");
        return;
    }
}
