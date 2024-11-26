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
