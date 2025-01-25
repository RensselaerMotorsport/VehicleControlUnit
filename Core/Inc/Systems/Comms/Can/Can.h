#ifndef RENSSELAERMOTORSPORT_CAN_H
#define RENSSELAERMOTORSPORT_CAN_H

#include "DBCParser.h"
#include "Message.h"

/*******************************************************************/

// Signals
#define MAX_SIGNALS 64
#define MAX_SIGNAL_NAME_LENGTH 64
#define MAX_UNIT_NAME_LENGTH 64
#define MAX_NODE_NAME_LENGTH 64

typedef struct {
    char name[MAX_SIGNAL_NAME_LENGTH];
    int start_bit;
    int length;
    int endian;
    int isSigned;
    float scale;
    float offset;
    float min;
    float max;
    char unit[MAX_UNIT_NAME_LENGTH];
    char reciever[MAX_NODE_NAME_LENGTH];
} CAN_Signal_Template;

typedef struct {
    int value;
    CAN_Signal_Template* template;
} CAN_Signal;

/*******************************************************************/

// Messages
#define MAX_MESSAGE_NAME_LENGTH 64

typedef struct {
    int id;
    int dlc; // Data Length Code
    int ide; // Identifier Extension
    int rtr; // Remote Transmission Request
    char name[MAX_MESSAGE_NAME_LENGTH];
    char sender[MAX_NODE_NAME_LENGTH];
    Signal signals[MAX_SIGNALS];
    int signal_count;
    int payload_length;
} CAN_Message_Template;

typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    CAN_Message_Template* template;
    CAN_Signal signals[template->signal_count];
} CAN_Message;

/*******************************************************************/

// CAN
#define MAX_BUS 3
#define MAX_MESSAGES 1024

// CANBus enum
typedef enum {
    CAN_1 = 0,
    CAN_2 = 1,
    CAN_3 = 2
} CANBus;

// CAN protocol enum
typedef enum {
    CAN_2A = 0, // Standard CAN w/ 11-bit ID
    CAN_2B = 1, // Standard CAN w/ 29-bit ID
    //CAN_FD = 2, // CAN FD w/ 11-bit ID
    //CAN_FD_BRS = 3, // CAN FD w/ 29-bit ID
} CANProtocol;

// Baudrate enum
typedef enum {
    CAN_100KBPS = 100000,
    CAN_125KBPS = 125000,
    CAN_250KBPS = 250000,
    CAN_500KBPS = 500000,
    CAN_1MBPS = 1000000,
} CANBaudrate;

// CAN message list per length of CANBus enum
typedef struct {
    CANBus bus;
    CAN_Message_Template messages[MAX_MESSAGES];
    int num_messages;
} CAN_MessageList;

// CAN handle
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
CAN_HandleTypeDef hcan3;

// CAN message list
CAN_MessageList can_messages[MAX_BUS];

// Receive message buffers
CAN_RxHeaderTypeDef RxHeader1;
CAN_RxHeaderTypeDef RxHeader2;
CAN_RxHeaderTypeDef RxHeader3;
uint8_t RxData1[8];
uint8_t RxData2[8];
uint8_t RxData3[8];

#endif // RENSSELAERMOTORSPORT_CAN_H
