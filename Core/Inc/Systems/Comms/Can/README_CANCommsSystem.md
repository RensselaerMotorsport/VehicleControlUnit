# CAN Communications System Architecture

## Overview

The new CANCommsSystem provides an integrated, subscriber-based architecture for CAN communications that replaces the standalone CAN module. This system allows controllers and external systems to automatically receive CAN messages based on DBC receiver field definitions.

## Key Features

- **Automatic Message Routing**: Messages are routed to systems based on DBC receiver fields
- **Subscriber Pattern**: Systems register for messages they need to receive
- **Type Safety**: Proper integration with ExternalSystem and ControllerSystem base classes
- **Statistics Tracking**: Built-in counters for RX/TX/dropped messages
- **Telemetry Integration**: All CAN activity is properly logged through the telemetry system

## Architecture Components

### CANCommsSystem Structure
```c
typedef struct CANCommsSystem {
    CommsSystem base;                           // Inherits from CommsSystem base class
    CANBus bus;                                // CAN bus identifier (CAN_1, CAN_2)
    CAN_MessageList* message_list;             // Reference to DBC message definitions
    CAN_Subscriber subscribers[MAX_CAN_SUBSCRIBERS]; // Registered message receivers
    int subscriber_count;                      // Number of active subscribers
    
    // Statistics
    uint32_t messages_received;
    uint32_t messages_sent; 
    uint32_t messages_dropped;
    uint32_t last_heartbeat;
    
    // Function pointers for dynamic dispatch
    int (*registerSystem)(CANCommsSystem*, const char*, void*, CAN_MessageHandler, SystemType);
    int (*unregisterSystem)(CANCommsSystem*, const char*);
    int (*sendMessage)(CANCommsSystem*, uint32_t, uint8_t*, uint8_t);
    void (*processReceivedMessage)(CANCommsSystem*, CAN_Message*);
} CANCommsSystem;
```

### CAN_Subscriber Structure
```c
typedef struct CAN_Subscriber {
    char receiver_name[MAX_NODE_NAME_LENGTH];  // Matches DBC receiver field
    void* system;                              // Pointer to ExternalSystem/ControllerSystem
    CAN_MessageHandler handler;                // Callback function for messages
    SystemType system_type;                    // EXTERNAL or CONTROLLER
} CAN_Subscriber;
```

## Migration from Old Architecture

### Old Approach (Standalone CAN Module)
```c
// Manual initialization
init_CANBus(CAN_1);

// Manual message setup
CAN_Signal_Template signals[8] = { ... };
add_message_lop(CAN_1, 0x123, 8, 0, 0, "Message 1", "AD3", 8, signals);

// Manual message routing in receive_CAN_message()
void receive_CAN_message(CAN_RxHeaderTypeDef* RxHeader, uint8_t* RxData, CANBus bus) {
    // Parse message
    // Route manually to different systems
}
```

### New Approach (Integrated CANCommsSystem)
```c
// System initialization
CANCommsSystem can1_system;
initCANCommsSystem(&can1_system, "CAN1_COMMS", 10, CAN_1);
loadCANDatabase(&can1_system, vcu_system_dbc);

// System registration
ExternalSystem bms_system;
initExternalSystem(&bms_system, "BMS", 10);
registerCANReceiver(&can1_system, "BMS", &bms_system, bms_can_handler, EXTERNAL);

// Automatic routing based on DBC receiver fields
// Messages with receiver="BMS" are automatically routed to bms_can_handler
```

## DBC Integration

The system uses the receiver field in DBC files to automatically route messages:

```dbc
BO_ 258 BMS_Battery_Status: 8 Vector__XXX
 SG_ Battery_Voltage : 0|16@1+ (0.1,0) [0|6553.5] "V" BMS
 SG_ Battery_Current : 16|16@1- (0.1,-1638.4) [-1638.4|5000] "A" BMS
 SG_ Battery_Temperature : 32|8@1+ (1,-40) [-40|215] "degC" BMS

BO_ 259 ESC_Motor_Status: 8 Vector__XXX  
 SG_ Motor_Speed : 0|16@1+ (1,0) [0|65535] "RPM" ESC
 SG_ Motor_Torque : 16|16@1- (0.1,0) [-3276.8|3276.7] "Nm" ESC
 SG_ Motor_Temperature : 32|8@1+ (1,-40) [-40|215] "degC" ESC
```

In this example:
- All signals in `BMS_Battery_Status` have receiver `BMS`
- All signals in `ESC_Motor_Status` have receiver `ESC`
- Systems registered with names "BMS" and "ESC" will automatically receive their respective messages

## Implementation Steps

### 1. System Initialization
```c
CANCommsSystem can1_system;

// Initialize the CAN communications system
if (initCANCommsSystem(&can1_system, "CAN1_COMMS", 10, CAN_1) != 0) {
    // Handle error
    return -1;
}

// Load DBC database
if (loadCANDatabase(&can1_system, vcu_system_dbc) != 0) {
    // Handle error  
    return -1;
}
```

### 2. System Registration
```c
ExternalSystem bms_system;
initExternalSystem(&bms_system, "BMS", 10);

// Register BMS to receive CAN messages
registerCANReceiver(&can1_system, "BMS", &bms_system, bms_can_handler, EXTERNAL);
```

### 3. Message Handler Implementation
```c
void bms_can_handler(void* system, CAN_Message* message) {
    ExternalSystem* bms = (ExternalSystem*)system;
    
    // Log message reception
    sendMessage("BMS_HANDLER", MSG_DEBUG, "ReceivedMsg=%s;ID=0x%lX", 
               message->template->name, message->header.StdId);
    
    // Process signals
    for (int i = 0; i < message->template->signal_count; i++) {
        CAN_Signal* signal = &message->signals[i];
        
        // Send signal data as telemetry
        sendMessage("BMS_DATA", MSG_SENSOR_DATA, "Signal=%s;Value=%f;Unit=%s", 
                   signal->template->name, signal->physical_value, signal->template->unit);
        
        // Update system state based on signal data
        if (strcmp(signal->template->name, "Battery_Voltage") == 0) {
            bms->battery_voltage = signal->physical_value;
        }
    }
}
```

### 4. Callback Integration
Replace the old `receive_CAN_message()` callback with `system_CAN_message_callback()` in the HAL interrupt handlers. This is handled automatically by the CANCommsSystem implementation.

## API Reference

### Core Functions

#### `initCANCommsSystem()`
```c
int initCANCommsSystem(CANCommsSystem* can_comms, const char* name, int hz, CANBus bus);
```
Initializes a CANCommsSystem instance with the specified parameters.

#### `loadCANDatabase()`
```c
int loadCANDatabase(CANCommsSystem* can_comms, const unsigned char* dbc_data);
```
Loads a DBC database into the CAN communications system.

#### `registerCANReceiver()`
```c
int registerCANReceiver(CANCommsSystem* can_comms, const char* receiver_name, 
                       void* system, CAN_MessageHandler handler, SystemType system_type);
```
Registers a system to receive CAN messages for a specific receiver name.

#### `sendCANMessage()`
```c
int sendCANMessage(CANCommsSystem* can_comms, uint32_t id, uint8_t* data, uint8_t len);
```
Sends a CAN message through the communications system.

### Statistics Functions

#### `getCANStatistics()`
```c
void getCANStatistics(CANCommsSystem* can_comms, uint32_t* rx_count, 
                     uint32_t* tx_count, uint32_t* dropped_count);
```
Retrieves message statistics from the CAN communications system.

## Benefits of the New Architecture

1. **Automatic Routing**: No manual message routing code needed
2. **Type Safety**: Proper integration with system base classes
3. **Maintainability**: Clear separation of concerns between systems
4. **Scalability**: Easy to add new systems and message types
5. **Debugging**: Built-in telemetry and statistics tracking
6. **Flexibility**: Systems can register/unregister dynamically

## Files Modified

- `Core/Inc/Systems/Comms/Can/CANCommsSystem.h` - New header defining the architecture
- `Core/Src/Systems/Comms/CAN/CANCommsSystem.c` - Implementation of the new system
- `Core/Inc/Systems/Comms/Can/Can.h` - Added extern declaration for can_messages
- `Core/Inc/Files/vcu_system.dbc` - Enhanced DBC with proper receiver definitions
- `Core/Src/Systems/Comms/CAN/CANIntegrationExample.c` - Example integration code

## Testing

The new architecture can be tested by:

1. Initializing CANCommsSystem instances
2. Registering test systems as message receivers  
3. Sending test messages and verifying routing
4. Checking telemetry output for proper message logging
5. Monitoring statistics for message counts

The system maintains backward compatibility with existing CAN hardware initialization (MX_CAN1_Init(), etc.) and only replaces the software architecture for message handling.