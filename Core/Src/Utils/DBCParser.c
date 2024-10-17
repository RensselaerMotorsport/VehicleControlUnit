#include "../../Inc/Systems/DBCParser.h"
#include "../../Inc/Systems/PrintHelpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Message* getDbcMessage(DBC* dbc) {
    return dbc->messages;
}

Signal* getSignals(Message* msg) {
    return msg->signals;
}

char* getSignalName(Signal* sig) {
    return sig->name;
}

int parseDbcLine(char *line, DBC *dbc) {
    // static Message *current_message = NULL;

    if (strncmp(line, "BO_", 3) == 0) {
        // Check if we went over the limit of allowed messages
        if (dbc->messageCount > MAX_MESSAGES) {
            fprintf(stderr, "Exceeded maximum number of messages\n");
            return 0;
        }
        // Message definition
        Message *message = &dbc->messages[dbc->messageCount++];
        sscanf(line, "BO_ %d %s %d %s", &message->id, message->name, &message->dlc, message->sender);
        message->name[strlen(message->name) - 1] = '\0'; // remove colon from end of name
        message->signal_count = 0; 
    } else if (strncmp(line, " SG_", 4) == 0) {
        Message *message = &dbc->messages[dbc->messageCount - 1];
        // Check if we went over the limit of allowed signals
        if (message->signal_count > MAX_SIGNALS) {
            fprintf(stderr, "Exceeded maximum number of signals\n");
            return 0;
        }
        // Signal definition
        Signal *signal = &message->signals[message->signal_count++];
        sscanf(line, " SG_ %s : %d|%d@%d%c (%f,%f) [%f|%f] %s %s", signal->name, &signal->start_bit, &signal->length, &signal->endian, &signal->signd, &signal->scale, &signal->offset, &signal->min, &signal->max, signal->unit, signal->reciever);
    }
}

float extractSignalValue(Signal* sig, const uint8_t* canData) {
    // Extract raw bits from CAN data based on start_bit and length
    uint64_t rawValue = 0;

    // Extract the raw value bitwise, handle endianess
    for (int i = 0; i < sig->length; i++) {
        int bitPos = sig->start_bit + i;
        int bytePos = bitPos / 8;
        int bitInByte = bitPos % 8;

        if (sig->endian == 0) {  // Assuming 0 is little endian, 1 is big endian
            rawValue |= ((canData[bytePos] >> bitInByte) & 0x01) << i;
        } else {
            // Handle big-endian differently (adjust byte order, bit shift accordingly)
            // Big endian handling code here
        }
    }

    // Apply sign conversion if necessary
    if (sig->signd && (rawValue & (1ULL << (sig->length - 1)))) {
        // Extend sign if signal is signed and the sign bit is set
        rawValue |= ~((1ULL << sig->length) - 1);
    }

    // Apply scaling and offset
    float physicalValue = rawValue * sig->scale + sig->offset;

    // Optionally, you can clamp it to min and max
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}

int parseDbcFile(DBC *dbc, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open DBC file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    dbc->messageCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (!parseDbcLine(line, dbc)) {
            return 0;
        }
    }

    fclose(file);
    return 1;
}

void printDbc(const DBC *dbc) {
    printf("Printing DBC file with %d messages\n", dbc->messageCount);
    for (int i = 0; i < dbc->messageCount; i++) {
        const Message *msg = &dbc->messages[i];
        printf(ANSI_COLOR_GREEN "Message" ANSI_COLOR_RESET ": %s (ID: %d, DLC: %d, Sender: %s, SIGs: %d)\n", msg->name, msg->id, msg->dlc, msg->sender, msg->signal_count);
        for (int j = 0; j < msg->signal_count; j++) {
            const Signal *sig = &msg->signals[j];
            printf("\t" ANSI_COLOR_BLUE "Signal" ANSI_COLOR_RESET ": %s (Start bit: %d, Length: %d, Endain: %d, Signed: %c,\n\t\tScale: %f, Offset: %f, Min: %f, Max: %f, \n\t\tUnit: %s, Reciever: %s)\n", sig->name, sig->start_bit, sig->length, sig->endian, sig->signd, sig->scale, sig->offset, sig->min, sig->max, sig->unit, sig->reciever);
        }
        printf("\n");
    }
}


int parseCanData(CanMessage* canMsg, const DBC* dbc, const char* filename) {
    FILE* file = fopen(filename, "r");
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

            //FIXME: Check that message is known
            /*for (int i = 0; i < dbc->messageCount; i++) {*/
            /*    if (dbc->messages[i].id != messageId) {*/
            /*        printf("Message ID %d not found in DBC\n", messageId);*/
            /*        continue;*/
            /*    }*/
            /*}*/
        }

        // Line contains signal info (Second Line)
        if (strncmp(line, "SG_", 3) == 0) {
            sscanf(line, "SG_ %*s : %*s (%f,%f)", &scale, &offset);
        }

        // Line contains raw CAN data
        if (strncmp(line, "Raw CAN Data", 3) == 0) {
            unsigned char rawData[8];
            sscanf(line, "Raw CAN Data (in Hex): 0x%X", &rawData);

            // Interpret the value based on scale and offset
            canMsg->data = rawData;
        }
    }

    fclose(file);
    return 1;
}
