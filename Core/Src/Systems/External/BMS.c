#include "../../../Inc/Systems/External/BMS.h"

#include <stdio.h>  // For printf

void initBms(Bms* bms, int hz, const char* dbcFn) {
    if (bms == NULL) return;

    initExternalSystem(&bms->extSystem, "Bms", hz, 0);

    bms->packVoltage = 0.0f;
    bms->packCurrent = 0.0f;
    bms->stateOfCharge = 0.0f;
    bms->cellVoltageMin = 0.0f;
    bms->cellVoltageMax = 0.0f;
    bms->cellTemperatureMin = 0.0f;
    bms->cellTemperatureMax = 0.0f;
    bms->totalPackCapacity = 0.0f;
    bms->remainingPackCapacity = 0.0f;
    bms->packHealth = 0.0f;
    bms->chargeStatus = IDLE;
    bms->extSystem.system.updateable.update = updateBms;

    DBC dbc;
    if(!parseDbcFile(&dbc, dbcFn)) {
        printf("Error: Couldn't parse dbc.\n");
        return;
    }

    Message* messageMap[MAX_MESSAGES] = {NULL};

    int size = dbc.messageCount;
    for (int i = 0; i < size; i++) {
        int index = dbc.messages[i].id / 100;
        messageMap[index] = &dbc.messages[i];
    }

    bms->dbcMessageMap = (Message**)messageMap;
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

bool bmsTransferFunction(Bms* bms, CanMessage* canData) {
    int index = canData->messageId / 100;
    Message* message = bms->dbcMessageMap[index];

    // Check if the message ID matches the expected one
    if (message != NULL && message->id == canData->messageId) {
        // Decode signals within the message
        for (int i = 0; i < message->signal_count; i++) {
            Signal* signal = &message->signals[i];
            float value = extractSignalValue(signal, canData->data);
            // assignBmsValue(bms, signal->type, value);  // Assign value to Bms
        }
        return true;
    }
    return false;  // Message ID not found or doesn't match
}


void updateBms(void* bms) {
    Bms* myBms = (Bms*) bms;
    printf("Not implemented\n");
}

// TODO: make this as similar to the normal update as possible.
//
// @warning For testing and debugging purposes only
void updateBmsTest(void* bms, const char* canDataFn) {
    Bms* myBms = (Bms*) bms;
    CanMessage canData = parseCanData(canDataFn);

    // Call the transfer function to decode the message
    if (!bmsTransferFunction(myBms, &canData)) {
        printf("Update Test Error: Transfer function failed.\n");
    }
}
