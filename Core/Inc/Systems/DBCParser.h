#ifndef RENNSSELAERMOTORSPORT_DBC_H
#define RENNSSELAERMOTORSPORT_DBC_H

#include <stdint.h>

#define MAX_LINE_LENGTH 256
#define MAX_SIGNALS 64
#define MAX_MESSAGES 256
#define MAX_SIGNAL_NAME_LENGTH 64
#define MAX_MESSAGE_NAME_LENGTH 64
#define MAX_NODE_NAME_LENGTH 64
#define MAX_UNIT_NAME_LENGTH 64

typedef struct {
    char name[MAX_SIGNAL_NAME_LENGTH];
    int start_bit;
    int length;
    int endian;
    char signd;
    float scale;
    float offset;
    float min;
    float max;
    char unit[MAX_UNIT_NAME_LENGTH];
    char reciever[MAX_NODE_NAME_LENGTH];
} Signal;

typedef struct {
    int id;
    char name[MAX_MESSAGE_NAME_LENGTH];
    int dlc;  // Data Length Code
    char sender[MAX_NODE_NAME_LENGTH];
    Signal signals[MAX_SIGNALS];
    int signal_count;
} Message;

typedef struct {
    Message messages[MAX_MESSAGES];
    int messageCount;
} DBC;

// FIXME: Organize into own classes

char* getSignalName(Signal* sig);

float extractSignalValue(Signal* sig, const uint8_t* canData);

Signal* getSignals(Message* msg);

int parseDbcFile(DBC* dbc, const char *filename);

uint8_t* getMessageData(Message* msg);

Message* getDbcMessages(DBC* dbc);

void printDbc(const DBC *dbc);

typedef struct {
    int messageId;
    uint8_t data[8];  // CAN message data, typically up to 8 bytes
    int dataLength;   // Length of the data in bytes
} CanMessage;

CanMessage parseCanData(const char* filename) {
    CanMessage canMsg;
    FILE* file = fopen(filename, "rb");

    if (!file) {
        perror("Failed to open CAN data file");
        exit(EXIT_FAILURE);
    }

    // Read CAN data (assuming the first 4 bytes are the message ID and next are the data)
    uint8_t rawData[10];  // Adjust based on your message size, max 10 bytes here
    fread(rawData, sizeof(uint8_t), sizeof(rawData), file);
    fclose(file);

    // Extract message ID from the first 4 bytes
    canMsg.messageId = (rawData[0] << 24) | (rawData[1] << 16) | (rawData[2] << 8) | rawData[3];

    // Extract message data (adjust this as per the actual message length)
    canMsg.dataLength = sizeof(rawData) - 4;
    for (int i = 0; i < canMsg.dataLength; i++) {
        canMsg.data[i] = rawData[i + 4];
    }

    return canMsg;
}

#endif  // RENNSSELAERMOTORSPORT_DBC_H