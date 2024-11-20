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

// TODO: Put in signal class
uint64_t extractSignalBits(const char* sourceData, int start_bit, int length) {
    // Convert the hex string to a 64-bit integer
    uint64_t data = 0;
    sscanf(sourceData, "%llx", &data);

    // Shift to isolate the desired bits
    uint64_t mask = (1ULL << length) - 1;   // Create a mask for the desired bit length
    uint64_t extracted = (data >> (64 - start_bit - length)) & mask;

    printf("Converted Data: 0x%016llX\n", data);
    printf("Start Bit: %d, Length: %d, Extracted: 0x%llX\n", start_bit, length, extracted);

    return extracted;
}

bool bmsTransferFunction(Bms* bms, CanMessage* canData) {
    int index = canData->messageId;
    printf("ID: %d\n", index);
    Message* message = &bms->dbc->messages[index-1712];

    // Check if the message ID matches the expected one
    if (message == NULL && message->id != canData->messageId) {
        printf("Message ID not found\n");
        return false;  // Message ID not found or doesn't match
    }

    printf("Message ID found: %d\n", message->id);
    printf("Signal Count: %d\n", message->signal_count);
    // Decode signals within the message
    for (int i = 0; i < message->signal_count; i++) {
        Signal* signal = &message->signals[i];

        // Extract the relevant data bits
        const unsigned char* extracted = extractSignalBits((const char*)canData->data, signal->start_bit, signal->length);
        printf("Temp data: %s\n", extracted);

        // Pass the extracted signal data to the extractSignalValue function
        // float value = extractSignalValue(signal, extracted);
        // printf("Value: %f\n", value);
        // assignBmsValue(bms, message->id, value);
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

    // DEBUG: printing can info
    printf("CAN: ID: %d, Len: %d, Data: %s\n", canData.messageId, canData.dataLength, canData.data);

    // Call the transfer function to decode the message
    if (!bmsTransferFunction(myBms, &canData)) {
        printf("Update Test Error: Transfer function failed.\n");
    }
}
