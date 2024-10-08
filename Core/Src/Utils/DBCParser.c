#include "../../Inc/Systems/DBCParser.h"
#include "../../Inc/Systems/PrintHelpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseDbcLine(char *line, DBC *dbc) {
    // static Message *current_message = NULL;

    if (strncmp(line, "BO_", 3) == 0) {
        // Check if we went over the limit of allowed messages
        if (dbc->message_count > MAX_MESSAGES) {
            fprintf(stderr, "Exceeded maximum number of messages\n");
            return 0;
        }
        // Message definition
        Message *message = &dbc->messages[dbc->message_count++];
        sscanf(line, "BO_ %d %s %d %s", &message->id, message->name, &message->dlc, message->sender);
        message->name[strlen(message->name) - 1] = '\0'; // remove colon from end of name
        message->signal_count = 0; 
    } else if (strncmp(line, " SG_", 4) == 0) {
        Message *message = &dbc->messages[dbc->message_count - 1];
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

int parseDbcFile(DBC *dbc, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open DBC file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    dbc->message_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (!parseDbcLine(line, dbc)) {
            return 0;
        }
    }

    fclose(file);
    return 1;
}

void printDbc(const DBC *dbc) {
    printf("Printing DBC file with %d messages\n", dbc->message_count);
    for (int i = 0; i < dbc->message_count; i++) {
        const Message *msg = &dbc->messages[i];
        printf(ANSI_COLOR_GREEN "Message" ANSI_COLOR_RESET ": %s (ID: %d, DLC: %d, Sender: %s, SIGs: %d)\n", msg->name, msg->id, msg->dlc, msg->sender, msg->signal_count);
        for (int j = 0; j < msg->signal_count; j++) {
            const Signal *sig = &msg->signals[j];
            printf("\t" ANSI_COLOR_BLUE "Signal" ANSI_COLOR_RESET ": %s (Start bit: %d, Length: %d, Endain: %d, Signed: %c,\n\t\tScale: %f, Offset: %f, Min: %f, Max: %f, \n\t\tUnit: %s, Reciever: %s)\n", sig->name, sig->start_bit, sig->length, sig->endian, sig->signd, sig->scale, sig->offset, sig->min, sig->max, sig->unit, sig->reciever);
        }
        printf("\n");
    }
}
