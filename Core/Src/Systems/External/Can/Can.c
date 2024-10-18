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

        // Line contains signal info (Second Line)
        // FIXME: Maybe not needed
        if (strncmp(line, "SG_", 3) == 0) {
            sscanf(line, "SG_ %*s : %*s (%f,%f)", &scale, &offset);
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
    // Apply scaling and offset directly to the raw CAN data
    float physicalValue = *canData * sig->scale + sig->offset;

    // Optionally clamp it to min and max
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}
