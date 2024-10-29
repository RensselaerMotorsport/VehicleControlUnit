#include "../../../../Inc/Systems/External/Can/Can.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

int parseCanData(CanMessage* canMsg, const char* fn) {
    FILE* file = fopen(fn, "r");
    if (!file) {
        perror("Failed to open CAN data file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int messageId = -1;
    float scale = 1.0, offset = 0.0;

    while (fgets(line, sizeof(line), file)) {
        // Line contains the message ID (First Line)
        if (strncmp(line, "BO_", 3) == 0) {
            sscanf(line, "BO_ %d", &messageId);
            canMsg->messageId = messageId;
        }

        // Line contains raw CAN data
        if (strncmp(line, "Raw CAN Data", 3) == 0) {
            unsigned int rawData;
            sscanf(line, "Raw CAN Data (in Hex): 0x%x", &rawData);

            // Interpret the value based on scale and offset
            canMsg->data = rawData;
        }
    }

    fclose(file);
    return 1;
}

float extractSignalValue(Signal* sig, const unsigned int* canData) {
    uint64_t rawValue = 0;

    if (sig->endian == ENDIAN_LITTLE) {
        // Little endian
        for (int i = 0; i < sig->length; i++) {
            int bitPos = sig->start_bit + i;
            int byteIndex = bitPos / 8;
            int bitInByte = bitPos % 8;
            int bitValue = (canData[byteIndex] >> bitInByte) & 0x1;
            rawValue |= ((uint64_t)bitValue) << i;
        }
    } else {
        // Big endian
        for (int i = 0; i < sig->length; i++) {
            int bitPos = sig->start_bit - i;
            int byteIndex = bitPos / 8;
            int bitInByte = 7 - (bitPos % 8);
            int bitValue = (canData[byteIndex] >> bitInByte) & 0x1;
            rawValue |= ((uint64_t)bitValue) << (sig->length - i - 1);
        }   
    }

    int64_t signedValue = 0;
    if (sig->signd == 's') {
        // Signed signal
        if (rawValue & (1ULL << (sig->length - 1))) {
            // Negative number, sign-extend
            signedValue = (int64_t)(rawValue | (~0ULL << sig->length));
        } else {
            signedValue = (int64_t)rawValue;
        }
    } else {
        // Unsigned signal
        signedValue = (int64_t)rawValue;
    }

    // Apply scaling and offset
    float physicalValue = (float)signedValue * sig->scale + sig->offset;

    // Clamp to min and max
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}
