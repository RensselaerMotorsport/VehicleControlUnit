#ifndef __RENSSELAERMOTORSPORT_CAN_H__
#define __RENSSELAERMOTORSPORT_CAN_H__

#include "stm32f7xx_hal.h"


/*******************************************************************/

// Signals
#define MAX_SIGNALS 8
#define MAX_SIGNAL_NAME_LENGTH 32
#define MAX_UNIT_NAME_LENGTH 32
#define MAX_NODE_NAME_LENGTH 32

typedef struct CAN_Signal_Template {
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

typedef struct CAN_Signal {
    int value;
    CAN_Signal_Template* template;
} CAN_Signal;

/*******************************************************************/

// Messages
#define MAX_MESSAGE_NAME_LENGTH 32

typedef struct CAN_Message_Template {
    int id;
    int dlc; // Data Length Code
    int ide; // Identifier Extension
    int rtr; // Remote Transmission Request
    char name[MAX_MESSAGE_NAME_LENGTH];
    char sender[MAX_NODE_NAME_LENGTH];
    CAN_Signal_Template signals[MAX_SIGNALS];
    int signal_count;
    int payload_length;
} CAN_Message_Template;

typedef struct CAN_Message {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    CAN_Message_Template* template;
    CAN_Signal signals[]; // TODO:This should not be hardcoded, find way to dynamically allocate
} CAN_Message;

/*******************************************************************/

// CAN
#define MAX_BUS 2
#define MAX_MESSAGES 128

// CANBus enum
typedef enum CANBus {
    CAN_1 = 0,
    CAN_2 = 1
} CANBus;

// CAN protocol enum
typedef enum CANProtocol {
    CAN_2A = 0, // Standard CAN w/ 11-bit ID
    CAN_2B = 1, // Standard CAN w/ 29-bit ID
    //CAN_FD = 2, // CAN FD w/ 11-bit ID
    //CAN_FD_BRS = 3, // CAN FD w/ 29-bit ID
} CANProtocol;

// Baudrate enum
typedef enum CANBaudrate {
    CAN_100KBPS = 100000,
    CAN_125KBPS = 125000,
    CAN_250KBPS = 250000,
    CAN_500KBPS = 500000,
    CAN_1MBPS = 1000000,
} CANBaudrate;

// CAN message list per length of CANBus enum
typedef struct CAN_MessageList{
    CANBus bus;
    CAN_Message_Template messages[MAX_MESSAGES];
    int num_messages;
} CAN_MessageList;

// function prototypes

/**
 * @brief Initializes the CANBus struct
 *
 * @param bus The CAN bus to initialize
 * @return int 0 if the CAN bus was initialized successfully, -1 if the bus is invalid
 */
int init_CANBus(CANBus bus);

/**
* @brief Loads a DBC file into the CAN database. Makes the messages and signals and stores them in the list
* 
* @param bus The CAN bus to load the DBC file into
* @param filename The name of the DBC file to load
* @return int 0 if the DBC file was loaded successfully, -1 if the file could not be opened
*/
int load_dbc_file(CANBus bus, const unsigned char* filename);

/**
 * @brief Adds a message to the CAN message list
 *
 * @param bus The CAN bus to add the message to
 * @param message The message to add
 * @return int 0 if the message was added successfully, -1 if the bus is invalid
 */
int add_message(CANBus bus, CAN_Message_Template message);

/**
 * @brief Adds a message to the CAN message list by passing in the individual message parameters
 * hence "LOP" (List of Parameters)
 *
 * @param bus The CAN bus to add the message to
 * @param id The ID of the message
 * @param dlc The Data Length Code of the message
 * @param ide The Identifier Extension of the message
 * @param rtr The Remote Transmission Request of the message
 * @param name The name of the message
 * @param sender The sender of the message
 * @param signal_count The number of signals in the message
 * @param signals The signals in the message
 * @return int 0 if the message was added successfully, -1 if the bus is invalid
 */
int add_message_lop(CANBus bus, int id, int dlc, int ide, int rtr, const char* name, const char* sender, int signal_count, CAN_Signal_Template* signals);

/**
 * @brief Sends a CAN message
 *
 * @param bus The CAN bus to send the message on
 * @param protocol The CAN protocol to use
 * @param id The ID of the message
 * @param data The data to send
 * @param len The length of the data
 * @return int 0 if the message was sent successfully, -1 if the bus is invalid
 */
int send_CAN_message(CANBus bus, CANProtocol protocol, uint32_t id, uint8_t* data, uint8_t len);

/**
 * @brief Receives a CAN message
 *
 * @param RxHeader The CAN Rx header
 * @param RxData The CAN Rx data
 * @param bus The CAN bus to receive the message on
 */
void receive_CAN_message(CAN_RxHeaderTypeDef* header, uint8_t* data, CANBus bus);

/**
 * @brief Parses a CAN message
 *
 * @param messages The CAN message list
 * @param can_message The CAN message to parse
 */
void parseMessage(CAN_MessageList* messages, CAN_Message* can_message);

/**
 * @brief Parses signals in a CAN message
 *
 * @param message The CAN message template
 * @param can_message The CAN message to parse
 */
void parseSignals(CAN_Message_Template* message, CAN_Message* can_message);

/**
 * @brief Parses a CAN signal
 *
 * @param signal The CAN signal template
 * @param can_signal The CAN signal to parse
 * @param can_message The CAN message to parse
 */
void parseSignal(CAN_Signal_Template* signal, CAN_Signal* can_signal, CAN_Message* can_message);


/**
 * @brief Prints the CAN message list
 */
void print_CAN_Messages_Lists();
#endif // RENSSELAERMOTORSPORT_CAN_H
