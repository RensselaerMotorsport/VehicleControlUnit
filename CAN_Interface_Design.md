# CAN System Interface Design Analysis

## Current Interface Flow (Problematic)

```
CAN Message Arrives
       ↓
HAL_CAN_RxFifo0MsgPendingCallback() [INTERRUPT CONTEXT]
       ↓
system_CAN_message_callback()
       ↓
processCANMessage()
       ↓
Find subscribers for message receiver
       ↓
Call: inverter_can_handler(inverter, message) [STILL IN INTERRUPT!]
       ↓
Handler processes signals and calls sendMessage()
       ↓ 
Handler returns... DATA IS LOST!
```

**Problem**: The CAN data exists only during the interrupt call. Once the handler returns, the parsed signals are gone!

## Improved Interface Design

### Option A: Data Storage in Systems (Recommended)

```
CAN Message Arrives
       ↓
HAL_CAN_RxFifo0MsgPendingCallback() [INTERRUPT CONTEXT]
       ↓
system_CAN_message_callback()
       ↓
processCANMessage()
       ↓
Find subscribers for message receiver
       ↓
Call: inverter_can_handler(inverter, message)
       ↓
Handler STORES data in system structure + sets "new_data" flags
       ↓
System's update() method checks flags and processes new data [MAIN LOOP CONTEXT]
```

### Option B: Message Queue (Alternative)

```
CAN Message Arrives
       ↓
HAL_CAN_RxFifo0MsgPendingCallback() [INTERRUPT CONTEXT]
       ↓
system_CAN_message_callback()
       ↓
Queue message for later processing
       ↓
System's update() method dequeues and processes messages [MAIN LOOP CONTEXT]
```

### Option C: Direct Update Calls (Simplest)

```
CAN Message Arrives
       ↓
HAL_CAN_RxFifo0MsgPendingCallback() [INTERRUPT CONTEXT]
       ↓
system_CAN_message_callback()
       ↓
processCANMessage()
       ↓
Find subscribers for message receiver
       ↓
Call: inverter->base.updateFromCAN(inverter, message) [DIRECT UPDATE]
```

## Recommended Implementation: Option A

### System Data Structure Enhancement

```c
typedef struct {
    ExternalSystem base;
    TorqueControl* tc;
    int maxCurrent;
    int maxTemp;
    int maxVoltage;
    
    // CAN Data Storage
    struct {
        float motor_speed;      // RPM
        float motor_torque;     // Nm  
        float motor_temp;       // degC
        uint8_t new_data_flags; // Bit flags for new data
        uint32_t last_update;   // Timestamp
    } can_data;
} Inverter;
```

### Enhanced Message Handler

```c
void inverter_can_handler(void* system, CAN_Message* message) {
    Inverter* inverter = (Inverter*)system;
    
    // Process each signal and STORE in system
    for (int i = 0; i < message->template->signal_count; i++) {
        CAN_Signal* signal = &message->signals[i];
        
        if (strcmp(signal->template->name, "Motor_Speed") == 0) {
            inverter->can_data.motor_speed = signal->physical_value;
            inverter->can_data.new_data_flags |= INVERTER_NEW_SPEED;
        } else if (strcmp(signal->template->name, "Motor_Torque") == 0) {
            inverter->can_data.motor_torque = signal->physical_value;
            inverter->can_data.new_data_flags |= INVERTER_NEW_TORQUE;
        } else if (strcmp(signal->template->name, "Motor_Temperature") == 0) {
            inverter->can_data.motor_temp = signal->physical_value;
            inverter->can_data.new_data_flags |= INVERTER_NEW_TEMP;
        }
    }
    
    inverter->can_data.last_update = HAL_GetTick();
}
```

### System Update Method

```c
int updateInverter(Updateable* updateable) {
    Inverter* inverter = (Inverter*)updateable->child;
    
    // Check for new CAN data
    if (inverter->can_data.new_data_flags != 0) {
        
        if (inverter->can_data.new_data_flags & INVERTER_NEW_SPEED) {
            // Process new speed data
            sendMessage("INVERTER", MSG_SYSTEM_STATUS, 
                       "NewSpeed=%f RPM", inverter->can_data.motor_speed);
        }
        
        if (inverter->can_data.new_data_flags & INVERTER_NEW_TEMP) {
            // Process new temperature data
            if (inverter->can_data.motor_temp > 80.0) {
                sendMessage("INVERTER", MSG_WARNING, 
                           "HighTemp=%f degC", inverter->can_data.motor_temp);
            }
        }
        
        // Clear the flags after processing
        inverter->can_data.new_data_flags = 0;
    }
    
    // Regular inverter update logic...
    return 0;
}
```

## Interface API

### For Control/External Systems:

```c
// Check if new CAN data is available
bool hasNewCANData(Inverter* inv) {
    return inv->can_data.new_data_flags != 0;
}

// Get latest motor speed (whether new or old)
float getMotorSpeed(Inverter* inv) {
    return inv->can_data.motor_speed;
}

// Get data age in milliseconds
uint32_t getDataAge(Inverter* inv) {
    return HAL_GetTick() - inv->can_data.last_update;
}

// Check for timeout (no data received)
bool isCANDataStale(Inverter* inv, uint32_t timeout_ms) {
    return getDataAge(inv) > timeout_ms;
}
```

## Benefits of This Design

1. **Interrupt Safety**: Minimal work in interrupt context
2. **Data Persistence**: Values stored in system structures
3. **New Data Detection**: Flags indicate when data is fresh
4. **Timeout Detection**: Can detect communication failures
5. **Thread Safety**: Main loop processes data safely
6. **Performance**: No queuing overhead

## Usage Example

```c
// In TorqueControl's update method
int updateTorqueControl(Updateable* updateable) {
    TorqueControl* tc = (TorqueControl*)updateable->child;
    
    // Check if inverter has new speed data
    if (hasNewCANData(&inverter)) {
        float current_speed = getMotorSpeed(&inverter);
        
        // Adjust torque based on current motor speed
        adjustTorqueForSpeed(tc, current_speed);
    }
    
    // Check for communication timeout
    if (isCANDataStale(&inverter, 1000)) { // 1 second timeout
        sendMessage("TORQUE", MSG_ERROR, "Inverter CAN timeout");
        // Enter safe mode
    }
    
    return 0;
}
```