#include "../../../../Inc/Systems/External/Can/Can.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Reads a single line from a file into the provided buffer.
 * 
 * @param fileName Path to the file to be read.
 * @param lineBuffer Pointer to the buffer where the line will be stored.
 * @param bufferSize Size of the buffer to ensure no overflow.
 * @return int 1 on success, 0 on failure.
 */
int readFileLine(const char* fileName, char* lineBuffer, size_t bufferSize) {
    if (!fileName || !lineBuffer) {
        fprintf(stderr, "Invalid input to readFileLine\n");
        return 0;
    }

    FILE* file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file at path: %s\n", fileName);
        return 0;
    }

    if (!fgets(lineBuffer, bufferSize, file)) {
        fprintf(stderr, "Failed to read line from file: %s\n", fileName);
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

/**
 * @brief Extracts a hexadecimal substring from a source string and converts it 
 *        to an integer.
 * 
 * @param source The source string to extract from.
 * @param start The starting position in the source string.
 * @param length The number of characters to extract.
 * @return unsigned int The extracted value as an integer.
 */
unsigned int extractHexValue(const char* source, int start, int length) {
    char temp[9] = {0}; // Max size: 8 characters + null terminator
    strncpy(temp, source + start, length);
    temp[length] = '\0';
    // Base 16 conversion
    return (unsigned int)strtol(temp, NULL, 16);
}

int parseCanData(CanMessage* canMsg, const char* fn) {
    if (!canMsg || !fn) {
        fprintf(stderr, "Invalid input to parseCanData\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    if (!readFileLine(fn, line, sizeof(line))) {
        return 0;
    }

    // Extract CAN ID (first 3 characters)
    canMsg->messageId = extractHexValue(line, 0, 3);

    // Extract DLC (next 2 characters)
    canMsg->dataLength = (unsigned char)extractHexValue(line, 3, 2);

    // Extract data (Remaining characters)
    strncpy((char*)canMsg->data, line + 5, canMsg->dataLength * 2);
    canMsg->data[canMsg->dataLength * 2] = '\0'; // Null-terminate for safety

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

float extractSignalValue(Signal* sig, const unsigned char* canData) {
    uint64_t rawValue;
    // printf("CAN: %s\n", canData);
    const uint8_t* byteData = (const uint8_t*) canData;
    // printf("byte : %d\n", byteData);

    // Determine the decoding method based on endian type
    if (sig->endian == ENDIAN_LITTLE) {
        rawValue = decodeLittleEndian(sig, byteData);
    } else {
        rawValue = decodeBigEndian(sig, byteData);
    }

    // printf("Raw: %d\n", rawValue);

    // Apply sign extension if the signal is signed
    int64_t signedValue = applySignExtension(rawValue, sig);

    // Convert to physical value using scale and offset
    float physicalValue = (float)signedValue * sig->scale + sig->offset;
    // printf("%f\n", physicalValue);

    // Clamp to minimum and maximum defined in the signal
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}
