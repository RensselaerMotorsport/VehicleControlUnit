#include "../../../../Inc/Systems/External/Can/DBCParser.h"
#include "../../../../Inc/Systems/PrintHelpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Message* getDbcMessage(DBC* dbc) {
    return dbc->messages;
}

int parseDbcLine(CAN_MessageList *messages, char *line) {
    // Check if the line is a message
    if (strstr(line, "BO_") != NULL) {
        // Parse the message
        Message msg;
        if (sscanf(line, "BO_ %d %s: %d %s", &msg.id, msg.name, &msg.dlc, msg.sender) != 4) {
            return 0;
        }
        messages->messages[messages->num_messages] = msg;
        messages->num_messages++;
    } else if (strstr(line, "SG_") != NULL) {
        // Parse the signal
        Signal sig;
        if (sscanf(line, "SG_ %s %d|%d@%d%c%d(%f,%f) [%f|%f] \"%[^\"]\" %s", sig.name, &sig.start_bit, &sig.length, &sig.endian, &sig.isSigned, &sig.scale, &sig.offset, &sig.min, &sig.max, sig.unit, sig.reciever) != 11) {
            return 0;
        }
        messages->messages[messages->num_messages - 1].signals[messages->messages[messages->num_messages - 1].signal_count] = sig;
        messages->messages[messages->num_messages - 1].signal_count++;
    }
    return 1;
}

int parseDbcFile(CAN_MessageList *messages, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!parseDbcLine(messages, line)) {
            fprintf(stderr, "Failed to parse line: %s\n", line);
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

void printDbc(Message *dbc) {
    printf("Printing DBC file with %d messages\n", dbc->messageCount);
    for (int i = 0; i < dbc->messageCount; i++) {
        const Message *msg = &dbc->messages[i];
        printf(ANSI_COLOR_GREEN "Message" ANSI_COLOR_RESET ": %s (ID: %d, DLC: %d, Sender: %s, SIGs: %d)\n", msg->name, msg->id, msg->dlc, msg->sender, msg->signal_count);
        for (int j = 0; j < msg->signal_count; j++) {
            const Signal *sig = &msg->signals[j];
            printf("\t" ANSI_COLOR_BLUE "Signal" ANSI_COLOR_RESET ": %s (Start bit: %d, Length: %d, Endain: %d, Signed: %c,\n\t\tScale: %f, Offset: %f, Min: %f, Max: %f, \n\t\tUnit: %s, Reciever: %s)\n", sig->name, sig->start_bit, sig->length, sig->endian, sig->isSigned, sig->scale, sig->offset, sig->min, sig->max, sig->unit, sig->reciever);
        }
        printf("\n");
    }
}
