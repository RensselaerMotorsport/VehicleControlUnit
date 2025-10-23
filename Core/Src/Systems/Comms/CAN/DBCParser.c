#include "../../../../Inc/Systems/Comms/Can/DBCParser.h"
#include "../../../../Inc/Utils/Common.h"
#include "../../../../Inc/Utils/MessageFormat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseDbcLine(CAN_MessageList *messages, char *line) {
    // Check if the line is a message
    if (strstr(line, "BO_") != NULL) {
        // Parse the message: "BO_ 258 ESC_Motor_Status_1: 8 ESC"
    	CAN_Message_Template msg;
        memset(&msg, 0, sizeof(msg));  // Initialize to zero
        
        if (sscanf(line, "BO_ %d %[^:]: %d %s", &msg.id, msg.name, &msg.dlc, msg.sender) == 4) {
            sendMessage("DBC_DEBUG", MSG_DEBUG, "ParsedMsg=ID:%d;Name:%s;DLC:%d;Sender:%s", msg.id, msg.name, msg.dlc, msg.sender);
            messages->messages[messages->num_messages] = msg;
            messages->num_messages++;
        } else {
            sendMessage("DBC_DEBUG", MSG_DEBUG, "FailedMsgParse=%s", line);
        }
    } else if (strstr(line, " SG_") != NULL) {
        // Parse the signal: " SG_ Motor_Speed : 0|16@1+ (1,0) [0|65535] "RPM" VCU"
        CAN_Signal_Template sig;
        memset(&sig, 0, sizeof(sig));
        char sign_and_endian[10];
        
        if (sscanf(line, " SG_ %s : %d|%d@%s (%f,%f) [%f|%f] \"%[^\"]\" %s", 
                   sig.name, &sig.start_bit, &sig.length, sign_and_endian, 
                   &sig.scale, &sig.offset, &sig.min, &sig.max, sig.unit, sig.reciever) == 10) {
            
            // Parse endian and sign from something like "1+" or "1-"
            sig.endian = sign_and_endian[0] - '0';  // Convert '1' to 1
            sig.isSigned = (sign_and_endian[1] == '-');
            
            sendMessage("DBC_DEBUG", MSG_DEBUG, "ParsedSig=%s;StartBit:%d;Len:%d", sig.name, sig.start_bit, sig.length);
            
            if (messages->num_messages > 0) {
                CAN_Message_Template* current_msg = &messages->messages[messages->num_messages - 1];
                current_msg->signals[current_msg->signal_count] = sig;
                current_msg->signal_count++;
            }
        } else {
            sendMessage("DBC_DEBUG", MSG_DEBUG, "FailedSigParse=%s", line);
        }
    }
    return 1;
}

int parseDbcFile(CAN_MessageList *messages, const unsigned char *dbc_contents) {
    // dbc contents is a string of the file - make a copy since strtok modifies it
    // Use the external length variable since dbc_contents may contain null bytes
    extern const unsigned int inverter_test_dbc_len;
    size_t len = inverter_test_dbc_len;
    
    // Debug the length and first few bytes
    char first_bytes[20];
    for (int i = 0; i < 19 && i < len; i++) {
        first_bytes[i] = dbc_contents[i];
    }
    first_bytes[19] = '\0';
    sendMessage("DBC_DEBUG", MSG_DEBUG, "Length=%d;FirstBytes=%s", len, first_bytes);
    
    char* dbc_copy = malloc(len + 1);
    if (!dbc_copy) return 0;
    memcpy(dbc_copy, dbc_contents, len);
    dbc_copy[len] = '\0';  // Ensure null termination
    
    char* line = strtok(dbc_copy, "\n");
    while (line != NULL) {
        //printf("Parsing line: %s\n", line);
        if (!parseDbcLine(messages, line)) {
            free(dbc_copy);
            return 0;
        }
        line = strtok(NULL, "\n");
    }
    
    free(dbc_copy);
    return 1;
}

void print_CAN_MessageList(const CAN_MessageList *messages) {
    //printf("Printing CAN Message List with %d messages\n", messages->num_messages);
    for (int i = 0; i < messages->num_messages; i++) {
        const CAN_Message_Template *msg = &messages->messages[i];
        //printf(ANSI_COLOR_GREEN "Message" ANSI_COLOR_RESET ": %s (ID: %d, DLC: %d, Sender: %s, SIGs: %d)\n", msg->name, msg->id, msg->dlc, msg->sender, msg->signal_count);
        for (int j = 0; j < msg->signal_count; j++) {
            (void)j;  // Unused in commented debug code
            //const CAN_Signal_Template *sig = &msg->signals[j];
            //printf("\t" ANSI_COLOR_BLUE "Signal" ANSI_COLOR_RESET ": %s (Start bit: %d, Length: %d, Endain: %d, Signed: %c,\n\t\tScale: %f, Offset: %f, Min: %f, Max: %f, \n\t\tUnit: %s, Reciever: %s)\n", sig->name, sig->start_bit, sig->length, sig->endian, sig->isSigned, sig->scale, sig->offset, sig->min, sig->max, sig->unit, sig->reciever);
        }
        //printf("\n");
    }
}
