#include "../../../../Inc/Systems/Comms/Can/DBCParser.h"
#include "../../../../Inc/Utils/Common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseDbcLine(CAN_MessageList *messages, char *line) {
    // Check if the line is a message
    if (strstr(line, "BO_") != NULL) {
        // Parse the message
    	CAN_Message_Template msg;
        if (sscanf(line, "BO_ %d %s: %d %s", &msg.id, msg.name, &msg.dlc, msg.sender) != 4) {
            return 0;
        }
        messages->messages[messages->num_messages] = msg;
        messages->num_messages++;
    } else if (strstr(line, "SG_") != NULL) {
        // Parse the signal
        CAN_Signal_Template sig;
        char* signedness = 0;
        if (sscanf(line, "SG_ %s %d|%d@%d%c(%f,%f) [%f|%f] \"%[^\"]\" %s", sig.name, &sig.start_bit, &sig.length, &sig.endian, signedness, &sig.scale, &sig.offset, &sig.min, &sig.max, sig.unit, sig.reciever) != 11) {
            return 0;
        }
        sig.isSigned = signedness[0] == '-';
        messages->messages[messages->num_messages - 1].signals[messages->messages[messages->num_messages - 1].signal_count] = sig;
        messages->messages[messages->num_messages - 1].signal_count++;
    }
    return 1;
}

int parseDbcFile(CAN_MessageList *messages, const unsigned char *dbc_contents) {
    // dbc contents is a string of the file
    char* line = strtok(dbc_contents, "\n");
    while (line != NULL) {
        printf("Parsing line: %s\n", line);
        if (!parseDbcLine(messages, line)) {
            return 0;
        }
        line = strtok(NULL, "\n");
    }
    return 1;
}

void print_CAN_MessageList(const CAN_MessageList *messages) {
    printf("Printing CAN Message List with %d messages\n", messages->num_messages);
    for (int i = 0; i < messages->num_messages; i++) {
        const CAN_Message_Template *msg = &messages->messages[i];
        printf(ANSI_COLOR_GREEN "Message" ANSI_COLOR_RESET ": %s (ID: %d, DLC: %d, Sender: %s, SIGs: %d)\n", msg->name, msg->id, msg->dlc, msg->sender, msg->signal_count);
        for (int j = 0; j < msg->signal_count; j++) {
            const CAN_Signal_Template *sig = &msg->signals[j];
            printf("\t" ANSI_COLOR_BLUE "Signal" ANSI_COLOR_RESET ": %s (Start bit: %d, Length: %d, Endain: %d, Signed: %c,\n\t\tScale: %f, Offset: %f, Min: %f, Max: %f, \n\t\tUnit: %s, Reciever: %s)\n", sig->name, sig->start_bit, sig->length, sig->endian, sig->isSigned, sig->scale, sig->offset, sig->min, sig->max, sig->unit, sig->reciever);
        }
        printf("\n");
    }
}
