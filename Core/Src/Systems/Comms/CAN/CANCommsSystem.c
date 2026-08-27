#ifndef TEST_MODE
#include "stm32f7xx_it.h"
#include "stm32f7xx_hal.h"
#endif

#include "../../../../Inc/Systems/Comms/Can/CANCommsSystem.h"
#include "../../../../Inc/Utils/MessageFormat.h"
#include <string.h>
#include <stdio.h>

// CAN RX Queue Item - lightweight message storage for interrupt context
typedef struct {
    uint32_t id;       // CAN ID
    uint32_t dlc;      // Data length
    uint8_t data[8];   // Message data
    CANBus bus;        // Bus number
} CAN_RX_Queue_Item;

// Forward declare the functions defined in this file
int initCANCommsSystem(CANCommsSystem* can_comms, const char* name, int hz, CANBus bus);
int loadCANDatabase(CANCommsSystem* can_comms, const unsigned char* dbc_data);
int registerCANReceiver(CANCommsSystem* can_comms, const char* receiver_name, 
                       void* system, CAN_MessageHandler handler, SystemType system_type);
int sendCANMessage(CANCommsSystem* can_comms, uint32_t id, uint8_t* data, uint8_t len);
void processCANMessage(CANCommsSystem* can_comms, CAN_Message* message);
int updateCANCommsSystem(Updateable* updateable);
void processCANQueue(CANCommsSystem* can_comms);
                     
// Forward declare the callback that replaces receive_CAN_message
void system_CAN_message_callback(void* RxHeader, uint8_t* RxData, CANBus bus);

// Global pointer to active CAN comms systems (one per bus)
static CANCommsSystem* active_can_systems[MAX_BUS] = {NULL, NULL};

int initCANCommsSystem(CANCommsSystem* can_comms, const char* name, int hz, CANBus bus) {
    if (!can_comms || bus >= MAX_BUS) {
        return -1;
    }
    
    // Initialize base CommsSystem  
    initCommsSystem(&can_comms->base, name, hz, CAN);
    
    // Set our update function and enable
    can_comms->base.system.updateable.update = updateCANCommsSystem;
    can_comms->base.system.updateable.enabled = ENABLED;
    can_comms->base.system.child = can_comms;
    
    // Initialize CAN specific fields
    can_comms->bus = bus;
    can_comms->message_list = &can_messages[bus];
    can_comms->subscriber_count = 0;
    
    // Clear statistics
    can_comms->messages_received = 0;
    can_comms->messages_sent = 0;
    can_comms->messages_dropped = 0;
    
    // Initialize message queue
    can_comms->queue_head = 0;
    can_comms->queue_tail = 0;
    
    // Initialize the underlying CAN bus
    if (init_CANBus(bus) != 0) {
        return -1;
    }
    
    // Register this system globally for callback routing
    active_can_systems[bus] = can_comms;
    
    // Enable the system
    ENABLE(can_comms->base.system);
    
    return 0;
}

int loadCANDatabase(CANCommsSystem* can_comms, const unsigned char* dbc_data) {
    if (!can_comms || !dbc_data) {
        return -1;
    }
    
    return load_dbc_file(can_comms->bus, dbc_data);
}

int registerCANReceiver(CANCommsSystem* can_comms, const char* receiver_name, 
                       void* system, CAN_MessageHandler handler, SystemType system_type) {
    if (!can_comms || !receiver_name || !system || !handler) {
        return -1;
    }
    
    // Check if we have space
    if (can_comms->subscriber_count >= MAX_CAN_SUBSCRIBERS) {
        sendMessage("CAN_COMMS", MSG_ERROR, "MaxSubscribers=%d", MAX_CAN_SUBSCRIBERS);
        return -1;
    }
    
    // Check if already registered
    for (int i = 0; i < can_comms->subscriber_count; i++) {
        if (strcmp(can_comms->subscribers[i].receiver_name, receiver_name) == 0) {
            sendMessage("CAN_COMMS", MSG_WARNING, "ReceiverAlreadyRegistered=%s", receiver_name);
            return -1;
        }
    }
    
    // Add new subscriber
    CAN_Subscriber* sub = &can_comms->subscribers[can_comms->subscriber_count];
    strncpy(sub->receiver_name, receiver_name, MAX_NODE_NAME_LENGTH - 1);
    sub->receiver_name[MAX_NODE_NAME_LENGTH - 1] = '\0';
    sub->system = system;
    sub->handler = handler;
    sub->system_type = system_type;
    
    can_comms->subscriber_count++;
    
    sendMessage("CAN_COMMS", MSG_DEBUG, "RegisteredReceiver=%s;Type=%d", receiver_name, system_type);
    return 0;
}



int sendCANMessage(CANCommsSystem* can_comms, uint32_t id, uint8_t* data, uint8_t len) {
    if (!can_comms || !data || len > 8) {
        return -1;
    }
    
    int result = send_CAN_message(can_comms->bus, CAN_2A, id, data, len);
    
    if (result == 0) {
        can_comms->messages_sent++;
    }
    
    return result;
}

void processCANMessage(CANCommsSystem* can_comms, CAN_Message* message) {
    if (!can_comms || !message) {
        return;
    }
    
    can_comms->messages_received++;
    
    // Send telemetry for the received message (even if template is NULL)
    char data_hex[17];
    for (int i = 0; i < message->header.DLC && i < 8; i++) {
        sprintf(&data_hex[i*2], "%02X", message->data[i]);
    }
    data_hex[message->header.DLC * 2] = '\0';
    
    // Use template name if available, otherwise "UNKNOWN"
    const char* msg_name = message->template ? message->template->name : "UNKNOWN";
    
    sendMessage("CAN", MSG_CAN_RX, "ID:0x%lX;DLC:%lu;Data:%s;Msg:%s", 
                message->header.StdId, message->header.DLC, data_hex, msg_name);
    
    // Only route to subscribers if we have a valid template
    if (!message->template) {
        sendMessage("CAN_COMMS", MSG_WARNING, "UnknownMessageID=0x%lX", message->header.StdId);
        return;
    }

    // Route message to subscribers - find any subscriber that matches any signal receiver
    for (int sub_idx = 0; sub_idx < can_comms->subscriber_count; sub_idx++) {
        CAN_Subscriber* subscriber = &can_comms->subscribers[sub_idx];
        
        // Check if this subscriber matches any signal receiver for this message
        for (int sig_idx = 0; sig_idx < message->template->signal_count; sig_idx++) {
            CAN_Signal_Template* sig_template = &message->template->signals[sig_idx];
            
            if (strcmp(subscriber->receiver_name, sig_template->reciever) == 0) {
                // Call the subscriber's handler once per message
                subscriber->handler(subscriber->system, message);
                
                sendMessage("CAN_COMMS", MSG_DEBUG, "RoutedMsg=%s;To=%s", 
                           message->template->name, subscriber->receiver_name);
                break; // Found match, don't check other signals for this subscriber
            }
        }
    }
}

int updateCANCommsSystem(Updateable* updateable) {
    // TEMPORARY: Comment out all processing to test scheduler overflow
    if (!updateable) {
        return -1;
    }
    
    // Get the System first, then the child  
    System* system = (System*)updateable->child;
    if (!system) {
        return -1;
    }
    
    CANCommsSystem* can_comms = (CANCommsSystem*)system->child;
    if (!can_comms) {
        return -1;
    }
    
    // Process queued messages every cycle
    int queue_depth = (can_comms->queue_tail - can_comms->queue_head + CAN_RX_QUEUE_SIZE) % CAN_RX_QUEUE_SIZE;
    
    if (queue_depth > 0) {
        processCANQueue(can_comms);
    }
    
    // Emergency queue management - prevent total overflow
    if (queue_depth > (CAN_RX_QUEUE_SIZE - 5)) {
        // Queue is almost full - drop oldest messages
        int messages_to_drop = 10;
        can_comms->queue_head = (can_comms->queue_head + messages_to_drop) % CAN_RX_QUEUE_SIZE;
        can_comms->messages_dropped += messages_to_drop;
        
        sendMessage("CAN_EMERGENCY", MSG_WARNING, "DroppedMessages=%d;QueueDepth=%d", 
                   messages_to_drop, queue_depth);
    }
    
    return 0;
}

// Heavy message processing - called from scheduler context
void processCANQueue(CANCommsSystem* can_comms) {
    if (!can_comms) return;
    
    int processed = 0;
    
    // Safety: Limit processing to prevent infinite loops
    int max_iterations = CAN_RX_QUEUE_SIZE; // Maximum possible messages
    
    // Process ALL queued messages with safety limit
    while (can_comms->queue_head != can_comms->queue_tail && processed < max_iterations) {
        // Simple array indexing instead of complex byte offset calculation
        CAN_RX_Queue_Item* queue_item = (CAN_RX_Queue_Item*)&can_comms->rx_queue_storage[can_comms->queue_head * 32];
        
        // Use stack buffer to avoid malloc
        uint8_t message_buffer[sizeof(CAN_Message) + sizeof(CAN_Signal) * 8];
        CAN_Message* can_message = (CAN_Message*)message_buffer;
        
        // Fill in the message structure
        can_message->header.StdId = queue_item->id;
        can_message->header.DLC = queue_item->dlc;
        memcpy(can_message->data, queue_item->data, 8);
        can_message->template = NULL;
        
        // Do the heavy lifting: DBC parsing and message routing
        parseMessage(can_comms->message_list, can_message);
        processCANMessage(can_comms, can_message);
        
        processed++;
        can_comms->messages_received++;
        
        // Move to next queued item
        can_comms->queue_head = (can_comms->queue_head + 1) % CAN_RX_QUEUE_SIZE;
        
        // Sanity check to prevent infinite loops
        if (processed > 0 && can_comms->queue_head == can_comms->queue_tail) {
            break; // Queue is now empty
        }
    }
    
    // Debug if we hit the safety limit
    if (processed >= max_iterations) {
        sendMessage("CAN_ERROR", MSG_ERROR, "InfiniteLoopProtection_Bus%d", can_comms->bus);
    }
}



// Queue a CAN message for deferred processing (interrupt-safe)
int queueCANMessage(CANCommsSystem* can_comms, void* header, uint8_t* data, CANBus bus) {
    if (!can_comms || !header || !data) {
        return -1;
    }
    
    // Extract header fields using correct structure layout:
    // struct { uint32_t StdId; uint32_t ExtId; uint32_t IDE; uint32_t RTR; uint32_t DLC; ... }
    uint32_t* header_ptr = (uint32_t*)header;
    uint32_t std_id = header_ptr[0];  // StdId at offset 0
    uint32_t dlc = header_ptr[4];     // DLC at offset 4 (16 bytes / 4 = offset 4)
    
    // Simple array indexing instead of complex byte offset calculation
    CAN_RX_Queue_Item* queue_item = (CAN_RX_Queue_Item*)&can_comms->rx_queue_storage[can_comms->queue_tail * 32];
    
    // Check if queue is full
    int next_tail = (can_comms->queue_tail + 1) % CAN_RX_QUEUE_SIZE;
    if (next_tail == can_comms->queue_head) {
        return -1; // Queue full
    }
    
    // Store message data
    queue_item->id = std_id;
    queue_item->dlc = dlc;
    memcpy(queue_item->data, data, 8);
    queue_item->bus = bus;
    
    // Update tail pointer (atomic on single-core systems)
    can_comms->queue_tail = next_tail;
    
    return 0;
}



// System-level callback to replace receive_CAN_message
void system_CAN_message_callback(void* RxHeader, uint8_t* RxData, CANBus bus) {
    // Fast path: queue the message for deferred processing
    if (bus >= MAX_BUS) {
        return; // Invalid bus
    }

    CANCommsSystem* can_comms = active_can_systems[bus];
    if (!can_comms) {
        return; // No system registered for this bus
    }

    // Queue the message for deferred processing (interrupt-safe)
    if (queueCANMessage(can_comms, RxHeader, RxData, bus) != 0) {
        can_comms->messages_dropped++;
    }
}

// Notify of a transmit completion - emit telemetry directly
void notifyCANTxEvent(CANBus bus, uint32_t id, uint8_t dlc, const uint8_t* data) {
    if (bus >= MAX_BUS) return;
    CANCommsSystem* can_comms = active_can_systems[bus];
    if (!can_comms) return;

    // Emit telemetry directly
    char data_hex[17];
    for (int i = 0; i < dlc && i < 8; i++) {
        sprintf(&data_hex[i*2], "%02X", data ? data[i] : 0);
    }
    data_hex[dlc * 2] = '\0';
    
    sendMessage("CAN", MSG_CAN_TX, "ID:0x%lX;DLC:%d;Data:%s", id, dlc, data_hex);
    can_comms->messages_sent++;
}
