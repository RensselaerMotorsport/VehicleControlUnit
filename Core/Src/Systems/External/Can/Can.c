#include "../../../../Inc/Systems/External/Can/Can.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int parseCanData(CanMessage* canMsg, const char* fn) {
    FILE* file = fopen(fn, "r");
    if (!file) {
        fprintf(stderr, "Failed to open CAN data file at path: %s - %s\n", fn, strerror(errno));
        exit(EXIT_FAILURE);
    }

    char line[256];
    int messageId = -1;

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
        // Little endian bit extraction
        for (int i = 0; i < sig->length; i++) {
            int bitPos = sig->start_bit + i;
            int byteIndex = bitPos / 8;
            int bitInByte = bitPos % 8;
            
            // Treating `canData` as an array of bytes
            const uint8_t* byteData = (const uint8_t*) canData;
            int bitValue = (byteData[byteIndex] >> bitInByte) & 0x1;

            // Debug prints for each step of the extraction
            // printf("Extracting bit %d:\n", i);
            // printf("  bitPos: %d\n", bitPos);
            // printf("  byteIndex: %d\n", byteIndex);
            // printf("  bitInByte: %d\n", bitInByte);
            // printf("  bitValue: %d\n", bitValue);

            // Set the extracted bit in rawValue
            rawValue |= ((uint64_t)bitValue) << i;

            // Print intermediate rawValue after setting this bit
            // printf("  rawValue (intermediate): %llu\n", rawValue);
        }
    } else {
        // Big endian bit extraction
        for (int i = 0; i < sig->length; i++) {
            int bitPos = sig->start_bit + i;
            int byteIndex = bitPos / 8;
            int bitInByte = 7 - (bitPos % 8);
            
            // Treating `canData` as an array of bytes
            const uint8_t* byteData = (const uint8_t*) canData;
            int bitValue = (byteData[byteIndex] >> bitInByte) & 0x1;

            // Debug prints for each step of the extraction
            // printf("Extracting bit %d:\n", i);
            // printf("  bitPos: %d\n", bitPos);
            // printf("  byteIndex: %d\n", byteIndex);
            // printf("  bitInByte: %d\n", bitInByte);
            // printf("  bitValue: %d\n", bitValue);

            // Set the extracted bit in rawValue
            rawValue |= ((uint64_t)bitValue) << (sig->length - i - 1);

            // Print intermediate rawValue after setting this bit
            // printf("  rawValue (intermediate): %llu\n", rawValue);
        }
    }

    int64_t signedValue = 0;
    if (sig->signd == 's') {
        // For signed values, apply sign extension if the most significant bit is set
        if (rawValue & (1ULL << (sig->length - 1))) {
            signedValue = (int64_t)(rawValue | (~0ULL << sig->length));
        } else {
            signedValue = (int64_t)rawValue;
        }
    } else {
        signedValue = (int64_t)rawValue;  // Unsigned values
    }

    // Convert to physical value using scale and offset
    float physicalValue = (float)signedValue * sig->scale + sig->offset;

    // Clamp to minimum and maximum defined in the signal
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}
