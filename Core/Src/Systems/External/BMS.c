#include "../../../Inc/Systems/External/BMS.h"

#include <stdio.h>  // For printf
#include "../../../Inc/Systems/External/Can/Can.h"

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
    bms->dbc = &dbc;

    Message* messageMap[MAX_MESSAGES] = {NULL};

    int size = dbc.messageCount;
    for (int i = 0; i < size; i++) {
        int index = dbc.messages[i].id % MAX_MESSAGES;
        messageMap[index] = &dbc.messages[i];
    }

    bms->dbcMessageMap = (Message**)messageMap;
}

void assignBmsValue(Bms* bms, int id, float value) {
    switch (id) {
        case 100:
            bms->packVoltage = value;
            break;
        case 200:
            bms->packCurrent = value;
            break;
        case 300:
            bms->stateOfCharge = value;
            break;
        case 400:
            bms->cellVoltageMin = value;
            break;
        case 500:
            bms->cellVoltageMax = value;
            break;
        case 600:
            bms->cellTemperatureMin = value;
            break;
        case 700:
            bms->cellTemperatureMax = value;
            break;
        case 800:
            bms->totalPackCapacity = value;
            break;
        case 900:
            bms->remainingPackCapacity = value;
            break;
        case 1000:
            bms->packHealth = value;
            break;
        case 1100:
            bms->chargeStatus = (int)value;
            break;
        default:
            // Handle invalid signal case if needed
            break;
    }
}

bool bmsTransferFunction(Bms* bms, CanMessage* canData) {
    int index = canData->messageId;
    printf("Id %d\n",  index);
    Message* message = bms->dbcMessageMap[index];

    printf("%d\n", message->id);

    // Check if the message ID matches the expected one
    if (message != NULL && message->id == canData->messageId) {
        // Decode signals within the message
        for (int i = 0; i < message->signal_count; i++) {
            Signal* signal = &message->signals[i];
            float value = extractSignalValue(signal, canData->data);
            assignBmsValue(bms, message->id, value);
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
    CanMessage canData;
    parseCanData(&canData, canDataFn);

    // Call the transfer function to decode the message
    if (!bmsTransferFunction(myBms, &canData)) {
        printf("Update Test Error: Transfer function failed.\n");
    }
}
