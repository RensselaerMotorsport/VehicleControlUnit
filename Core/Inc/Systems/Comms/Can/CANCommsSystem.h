#ifndef RENSSELAERMOTORSPORT_CAN_COMMS_SYSTEM_H
#define RENSSELAERMOTORSPORT_CAN_COMMS_SYSTEM_H

#include "../../CommsSystem.h"
#include "Can.h"
#include "../../ExternalSystem.h"
#include "../../ControllerSystem.h"

// Maximum number of systems that can register for CAN messages
#define MAX_CAN_SUBSCRIBERS 16

// CAN Message Queue Configuration
#define CAN_RX_QUEUE_SIZE 64  // Must be power of 2 for efficient modulo

// Forward declarations
typedef struct CANCommsSystem CANCommsSystem;
typedef struct ExternalSystem ExternalSystem;
typedef struct ControllerSystem ControllerSystem;

// CAN Message Handler function pointer
// Called when a CAN message is received for a specific receiver
typedef void (*CAN_MessageHandler)(void* system, CAN_Message* message);

// CAN Subscriber - represents a system registered to receive CAN messages
typedef struct {
    char receiver_name[MAX_NODE_NAME_LENGTH];  // Name of the receiver (from DBC)
    void* system;                              // Pointer to the system (Controller/External/etc)
    CAN_MessageHandler handler;                // Function to call when message received
    SystemType system_type;                    // Type of system for validation
} CAN_Subscriber;

// Enhanced CAN Communications System
struct CANCommsSystem {
    CommsSystem base;                          // Inherits from CommsSystem
    
    // CAN Hardware Configuration
    CANBus bus;                               // Which CAN bus (CAN_1, CAN_2)
    CAN_MessageList* message_list;            // Pointer to loaded DBC messages
    
    // Subscriber Management
    CAN_Subscriber subscribers[MAX_CAN_SUBSCRIBERS];
    int subscriber_count;
    
    // Basic Statistics
    uint32_t messages_received;
    uint32_t messages_sent;
    uint32_t messages_dropped;
    
    // Message Queue for Deferred Processing
    uint8_t rx_queue_storage[CAN_RX_QUEUE_SIZE * 32]; // 32 bytes per queue item
    volatile int queue_head;
    volatile int queue_tail;
};

/**
 * @brief Initialize a CAN Communications System
 * 
 * @param can_comms Pointer to CANCommsSystem to initialize
 * @param name Name of the CAN system
 * @param hz Update frequency
 * @param bus Which CAN bus to use (CAN_1, CAN_2)
 * @return int 0 on success, -1 on failure
 */
int initCANCommsSystem(CANCommsSystem* can_comms, const char* name, int hz, CANBus bus);

/**
 * @brief Load DBC file and initialize message definitions
 * 
 * @param can_comms Pointer to CANCommsSystem
 * @param dbc_data Pointer to DBC file data
 * @return int 0 on success, -1 on failure
 */
int loadCANDatabase(CANCommsSystem* can_comms, const unsigned char* dbc_data);

/**
 * @brief Register a system to receive CAN messages for specific receivers
 * 
 * @param can_comms Pointer to CANCommsSystem
 * @param receiver_name Name from DBC file (e.g., "VCU", "BMS", "ESC")
 * @param system Pointer to the system (ExternalSystem*, ControllerSystem*, etc)
 * @param handler Function to call when messages are received
 * @param system_type Type of system for validation
 * @return int 0 on success, -1 on failure
 */
int registerCANReceiver(CANCommsSystem* can_comms, const char* receiver_name, 
                       void* system, CAN_MessageHandler handler, SystemType system_type);



/**
 * @brief Send a CAN message (wrapper for existing send_CAN_message)
 * 
 * @param can_comms Pointer to CANCommsSystem
 * @param id CAN message ID
 * @param data Pointer to data bytes
 * @param len Data length (0-8)
 * @return int 0 on success, negative on failure
 */
int sendCANMessage(CANCommsSystem* can_comms, uint32_t id, uint8_t* data, uint8_t len);

/**
 * @brief Process a received CAN message and route to appropriate systems
 * This function replaces the current receive_CAN_message for system integration
 * 
 * @param can_comms Pointer to CANCommsSystem
 * @param message Parsed CAN message with signals
 */
void processCANMessage(CANCommsSystem* can_comms, CAN_Message* message);

/**
 * @brief Default update function for CAN Communications System
 * Handles heartbeat checking and statistics
 * 
 * @param updateable Pointer to the updateable (cast from CANCommsSystem)
 * @return int 0 on success
 */
int updateCANCommsSystem(Updateable* updateable);



/**
 * @brief Queue a CAN message for deferred processing (called from interrupt context)
 * 
 * @param can_comms Pointer to CANCommsSystem
 * @param header CAN message header (STM32 HAL type)
 * @param data CAN message data (8 bytes)
 * @param bus CAN bus number
 * @return int 0 on success, -1 if queue full
 */
int queueCANMessage(CANCommsSystem* can_comms, void* header, uint8_t* data, CANBus bus);





/**
 * @brief Notify the CAN system of a transmit completion event from interrupt context.
 * This is interrupt-safe and will defer any telemetry emission to the scheduler.
 */
void notifyCANTxEvent(CANBus bus, uint32_t id, uint8_t dlc, const uint8_t* data);

#endif // RENSSELAERMOTORSPORT_CAN_COMMS_SYSTEM_H