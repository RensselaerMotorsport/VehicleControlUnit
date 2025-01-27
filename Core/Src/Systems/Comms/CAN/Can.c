#include "../../../../Inc/Systems/Comms/Can/Can.h"
#include "../../../../Inc/Systems/Comms/Can/DBCParser.h"
#include "../../../../Inc/stm32f7xx_it.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

// CAN message list
CAN_MessageList can_messages[MAX_BUS];

int init_CANBus(CANBus bus) {
    // Initialize the CAN message list
    can_messages[bus].bus = bus;
    can_messages[bus].num_messages = 0;

    return 0;
}

int load_dbc_file(CANBus bus, const char* filename)
{
    // Load the DBC file
    FILE* dbc_file = fopen(filename, "r");
    if (dbc_file == NULL) {
        return -1;
    }
    // Parse the DBC file
    parseDbcFile(&can_messages[bus], filename);
    return 0;
}

int add_message(CANBus bus, CAN_Message_Template message) {
    // Add the message to the list
    can_messages[bus].messages[can_messages[bus].num_messages] = message;
    can_messages[bus].num_messages++;
    return 0;
}

int add_message(CANBus bus, int id, int dlc, int ide, int rtr, const char* name, const char* sender, int signal_count, CAN_Signal_Template* signals)
{
    // Create a CAN message template
    CAN_Message_Template message;
    message.id = id;
    message.dlc = dlc;
    message.ide = ide;
    message.rtr = rtr;
    strcpy(message.name, name);
    strcpy(message.sender, sender);
    message.signal_count = signal_count;
    message.payload_length = 0;

    // Copy the signals
    for (int i = 0; i < signal_count; i++) {
        message.signals[i] = signals[i];
        message.payload_length += signals[i].length;
    }

    // Add the message to the list
    can_messages[bus].messages[can_messages[bus].num_messages] = message;
    can_messages[bus].num_messages++;
    return 0;
}

// Send a CAN message
int send_CAN_message(CANBus bus, CANProtocol protocol, uint32_t id, uint8_t* data, uint8_t len) {

    CAN_TxHeaderTypeDef TxHeader;
    if (protocol == CAN_2A) {
        TxHeader.StdId = id;
        TxHeader.IDE = CAN_ID_STD;
    } else if (protocol == CAN_2B) {
        TxHeader.ExtId = id;
        TxHeader.IDE = CAN_ID_EXT;
    } else {
        printf("Invalid CAN protocol\r\n");
        return -1;
    }

    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = len;
    TxHeader.TransmitGlobalTime = DISABLE;

    // Send the CAN message
    if (send_CAN_message_helper(bus, &TxHeader, data) != 0) {
        return -1;
    }
    return 0;
}

// Receive a CAN message from the Callback
void receive_CAN_message(CAN_RxHeaderTypeDef* RxHeader, uint8_t* RxData, CANBus bus) {
    // Create a CAN message
    CAN_Message* can_message = malloc(sizeof(CAN_Message) + sizeof(CAN_Signal) * 8);
    can_message->header = *RxHeader;
    memcpy(can_message->data, RxData, 8);

    // Parse the message
    parseMessage(&can_messages[bus], can_message);

    // Print out the contents of the message
    printf("Received CAN message on bus %d with ID %" PRIu32" and data: ", bus, can_message->header.StdId);
    for (int i = 0; i < can_message->header.DLC; i++) {
        printf("%d ", can_message->data[i]);
    }
    printf("\r\n");
    
    // Print out the signals
    for (int i = 0; i < can_message->template->signal_count; i++) {
        CAN_Signal* signal = &can_message->signals[i];
        printf("\t%s: %u \r\n", signal->template->name, signal->value);
        free(signal);
    }
    printf("\r\n");

    // Free the message
    free(can_message);
}

// Parse a CAN message
void parseMessage(CAN_MessageList* messages, CAN_Message* can_message) {
    // Find the message in the list
    // TODO: Implement a hash table or map for faster lookup
    for (int i = 0; i < messages->num_messages; i++) {
        CAN_Message_Template* msg = &messages->messages[i];
        if (msg->id == can_message->header.StdId || msg->id == can_message->header.ExtId) {
            // Parse the signals of the message
            can_message->template = msg;
            parseSignals(msg, can_message);
            return;
        }
    }
}

// Parse the signals of a CAN message
void parseSignals(CAN_Message_Template* message, CAN_Message* can_message) {
    // Find the signals in the message
    for (int i = 0; i < message->signal_count; i++) {
        CAN_Signal_Template* signal = &message->signals[i];
        // Parse the signal
        parseSignal(signal, &can_message->signals[i], can_message);
    }
}

// Parse a CAN signal
void parseSignal(CAN_Signal_Template* signal, CAN_Signal* can_signal, CAN_Message* can_message) {
    can_signal->template = signal;

    // Extract the raw data from the message
    uint64_t raw_data = 0;
    for (int i = signal->start_bit; i < signal->start_bit + signal->length; i++) {
        raw_data |= ((can_message->data[i / 8] >> (i % 8)) & 1) << (i - signal->start_bit);
    }

    // Check if the signal is big endian
    if (signal->endian == 1) {
        // Reverse the bits
        uint64_t reversed_data = 0;
        for (int i = 0; i < signal->length; i++) {
            reversed_data |= ((raw_data >> i) & 1) << (signal->length - i - 1);
        }
        raw_data = reversed_data;
    }

    // Check if the signal is signed
    if (signal->isSigned) {
        // Sign extend the data
        raw_data = (raw_data << (64 - signal->length)) >> (64 - signal->length);
    }

    // Scale and offset the data
    can_signal->value = (raw_data * signal->scale) + signal->offset;

}