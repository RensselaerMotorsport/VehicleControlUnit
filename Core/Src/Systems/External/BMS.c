#include "../../../Inc/Systems/External/BMS.h"

#include <stdio.h>
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
}

void assignBmsValue(Bms* bms, int id, float value, const char* name) {
    switch (id) {
        // TODO: Remember to update +1712
        case 0:
            if (strcmp(name, "Pack_Current") == 0) {
                bms->packCurrent = value;
            } else if (strcmp(name, "Pack_Inst_Voltage") == 0) {
                bms->packVoltage = value;
            } else if (strcmp(name, "Pack_SOC") == 0) {
                bms->stateOfCharge = value;
            } else if (strcmp(name, "Relay_State") == 0) {
                /*bms->relayState = (int)value;*/
            } else if (strcmp(name, "CRC_Checksum") == 0) {
                // TODO: Deal with this

                /*bms->crcChecksum = (int)value;*/
            }
            break;
        case 1:
            if (strcmp(name, "Pack_DCL") == 0) {
                /*bms->packDCL = value;*/
            } else if (strcmp(name, "Pack_CCL") == 0) {
                /*bms->packCCL = value;*/
            } else if (strcmp(name, "High_Temperature") == 0) {
                /*bms->highTemperature = value;*/
            } else if (strcmp(name, "Low_Temperature") == 0) {
                /*bms->lowTemperature = value;*/
            } else if (strcmp(name, "CRC_Checksum") == 0) {
                /*bms->crcChecksum = (int)value;*/
            }
            break;
        default:
            // Handle invalid signal case if needed
            break;
    }
}

// TODO: Put in signal class
const unsigned char* extractSignalBytes(const unsigned char* data, int startBit,
                                        int len) {
    static unsigned char result[MAX_CAN_DATA_LENGTH];
    memset(result, 0, MAX_CAN_DATA_LENGTH);

    int posAfterData = 7;
    int bitOffset = (startBit - posAfterData);
    int startByte = bitOffset / 8;
    int endByte = (bitOffset + len - 1) / 8;
    int resultIndex = 0;

    // Copy relevant bytes to the result array
    for (int i = startByte * 2; i <= endByte * 2; i+=2) {
        result[resultIndex++] = data[i];
        result[resultIndex++] = data[i+1];
    }

    return result;
}

bool bmsTransferFunction(Bms* bms, CanMessage* canData) {
    int index = canData->messageId;
    Message* message = &bms->dbc->messages[index-1712];

    // Check if the message ID matches the expected one
    if (message == NULL && message->id != canData->messageId) {
        return false;  // Message ID not found or doesn't match
    }

    // Decode signals within the message
    for (int i = 0; i < message->signal_count; i++) {
        Signal* signal = &message->signals[i];

        // Extract the relevant data bits
        const unsigned char* extracted = extractSignalBytes(
            (const unsigned char*)canData->data,
            signal->start_bit,
            signal->length
        );

        // Pass the extracted signal data to the extractSignalValue function
        float value = extractSignalValue(signal, extracted);
        // TODO: check that this works
        assignBmsValue(bms, message->id, value, signal->name);
    }
    return true;
}

// TODO: Implemented this
void updateBms(void* bms) {
    Bms* myBms = (Bms*) bms;
    printf("Not implemented\n");
}

// TODO: make this as similar to the normal update as possible.
// Add doxy comments
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
