#include "../../../../Inc/Systems/External/Can/Can.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int parseCanData(CanMessage* canMsg, const char* fn) {
    FILE* file = fopen(fn, "r");
    if (!canMsg || !file) {
        fprintf(stderr, "Failed to open CAN data file at path: %s - %s\n",
                fn, strerror(errno));
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int messageId = -1;

    while (fgets(line, sizeof(line), file)) {
        // Line contains the message ID (First Line)
        if (strncmp(line, "BO_", 3) == 0) {
            sscanf(line, "BO_ %d", &messageId);
            canMsg->messageId = messageId;
        }

        // Line contains raw CAN data
        if (strncmp(line, "Raw CAN Data", 12) == 0) {
            unsigned int rawData;
            sscanf(line, "Raw CAN Data (in Hex): 0x%x", &rawData);

            // Interpret the value based on scale and offset
            canMsg->data = rawData;
        }
    }

    fclose(file);
    return 1;
}

/**
 * @brief Decodes a raw signal value from little-endian formatted CAN data.
 *
 * @param sig The signal configuration, defining bit length and start position.
 * @param byteData Pointer to the CAN data interpreted as a byte array.
 * @return uint64_t The raw extracted signal value.
 */
uint64_t decodeLittleEndian(const Signal* sig, const uint8_t* byteData) {
    uint64_t rawValue = 0;
    int byteLength = (sig->length + 7) / 8;

    for (int i = 0; i < byteLength; i++) {
        uint8_t currentByte = byteData[i];

        // Add the byte to rawValue at the correct position in rawValue
        rawValue |= ((uint64_t)currentByte) << (8 * i);
    }

    return rawValue;
}

/**
 * @brief Decodes a raw signal value from big-endian formatted CAN data.
 *
 * @param sig The signal configuration, defining bit length and start position.
 * @param byteData Pointer to the CAN data interpreted as a byte array.
 * @return uint64_t The raw extracted signal value.
 */
uint64_t decodeBigEndian(const Signal* sig, const uint8_t* byteData) {
    uint64_t rawValue = 0;
    int byteLength = (sig->length + 7) / 8;

    for (int i = byteLength - 1; i >= 0; i--) {
        uint8_t currentByte = byteData[i];

        // Shift the byte into the correct position in rawValue
        rawValue = (rawValue << 8) | currentByte;
    }

    return rawValue;
}

/**
 * @brief Applies sign extension to a raw value if the signal is signed.
 *
 * @param rawValue The extracted raw signal value.
 * @param sig The signal configuration, specifying if the signal is signed.
 * @return int64_t The signed (or unsigned) interpreted value.
 */
int64_t applySignExtension(uint64_t rawValue, const Signal* sig) {
    if (sig->isSigned == 's') {
        // Check the most significant bit for sign extension
        if (rawValue & (1ULL << (sig->length - 1))) {
            return (int64_t)(rawValue | (~0ULL << sig->length));
        }
    }
    return (int64_t)rawValue;
}

float extractSignalValue(Signal* sig, const unsigned int* canData) {
    uint64_t rawValue;
    const uint8_t* byteData = (const uint8_t*) canData;

    // Determine the decoding method based on endian type
    if (sig->endian == ENDIAN_LITTLE) {
        rawValue = decodeLittleEndian(sig, byteData);
    } else {
        rawValue = decodeBigEndian(sig, byteData);
    }

    // Apply sign extension if the signal is signed
    int64_t signedValue = applySignExtension(rawValue, sig);

    // Convert to physical value using scale and offset
    float physicalValue = (float)signedValue * sig->scale + sig->offset;

    // Clamp to minimum and maximum defined in the signal
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}
