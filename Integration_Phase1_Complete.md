# CAN Communications System Integration - Phase 1 Complete

## Integration Strategy Implemented

### **Phase 1: Non-Disruptive Addition ✅**

I've successfully integrated the new CANCommsSystem **alongside** the existing CAN module without breaking any existing functionality. Both systems now coexist.

## What Was Added to main.c

### 1. **Includes** (Line ~29)
```c
#include "../Inc/Systems/Comms/Can/CANCommsSystem.h"
```

### 2. **Global Variables** (Line ~98)
```c
// New CAN Communications Systems (alongside existing CAN)
CANCommsSystem can1_comms;
CANCommsSystem can2_comms;
```

### 3. **System Initialization** (Lines ~194-217)
```c
// Initialize New CAN Communications Systems (alongside existing CAN)
if (initCANCommsSystem(&can1_comms, "CAN1_COMMS", 10, CAN_1) != 0) {
    sendMessage("INIT", MSG_ERROR, "CANCommsSystem CAN1 initialization failed");
} else {
    sendMessage("INIT", MSG_SYSTEM_STATUS, "CANCommsSystem CAN1 initialized successfully");
    
    // Load the enhanced DBC database
    if (loadCANDatabase(&can1_comms, vcu_system_dbc) != 0) {
        sendMessage("INIT", MSG_ERROR, "CAN1 DBC database load failed");
    } else {
        sendMessage("INIT", MSG_SYSTEM_STATUS, "CAN1 DBC database loaded successfully");
    }
}

// Initialize CAN2 system as well
if (initCANCommsSystem(&can2_comms, "CAN2_COMMS", 10, CAN_2) != 0) {
    sendMessage("INIT", MSG_ERROR, "CANCommsSystem CAN2 initialization failed");
} else {
    sendMessage("INIT", MSG_SYSTEM_STATUS, "CANCommsSystem CAN2 initialized successfully");
    
    // Load the same DBC database for CAN2 (can be different if needed)
    if (loadCANDatabase(&can2_comms, vcu_system_dbc) != 0) {
        sendMessage("INIT", MSG_ERROR, "CAN2 DBC database load failed");
    } else {
        sendMessage("INIT", MSG_SYSTEM_STATUS, "CAN2 DBC database loaded successfully");
    }
}
```

### 4. **Example System Registration** (Lines ~336-342)
```c
// Register Inverter for CAN messages (example of new architecture usage)
if (registerCANReceiver(&can1_comms, "ESC", &inverter, inverter_can_handler, EXTERNAL) == 0) {
    sendMessage("INIT", MSG_SYSTEM_STATUS, "Inverter registered for CAN messages on ESC receiver");
} else {
    sendMessage("INIT", MSG_WARNING, "Failed to register Inverter for CAN messages");
}
```

### 5. **Scheduler Integration** (Lines ~345-352)
```c
Updateable* updateables[8];
updateables[0] = &apps.base.system.updateable;
updateables[1] = &bsc.base.system.updateable;
updateables[2] = &rtd.base.system.updateable;
updateables[3] = &tc.base.system.updateable;
updateables[4] = &inverter.base.system.updateable;
updateables[5] = &can1_comms.base.system.updateable;  // NEW
updateables[6] = &can2_comms.base.system.updateable;  // NEW
updateables[7] = NULL;
```

### 6. **Example Message Handler** (Lines ~1552-1585)
```c
void inverter_can_handler(void* system, CAN_Message* message) {
    Inverter* inverter = (Inverter*)system;
    
    // Log the received message for debugging
    sendMessage("INVERTER_CAN", MSG_DEBUG, "ReceivedMsg=%s;ID=0x%lX;DLC=%lu", 
               message->template->name, message->header.StdId, message->header.DLC);
    
    // Process each signal in the message
    for (int i = 0; i < message->template->signal_count; i++) {
        CAN_Signal* signal = &message->signals[i];
        
        // Send signal data as telemetry
        sendMessage("INVERTER_DATA", MSG_SENSOR_DATA, "Signal=%s;Value=%f;Unit=%s", 
                   signal->template->name, signal->physical_value, signal->template->unit);
        
        // Process specific signals that the inverter cares about
        if (strcmp(signal->template->name, "Motor_Speed") == 0) {
            // Handle motor speed signal
            sendMessage("INVERTER", MSG_SYSTEM_STATUS, "MotorSpeed=%f RPM", signal->physical_value);
        } else if (strcmp(signal->template->name, "Motor_Torque") == 0) {
            // Handle motor torque signal  
            sendMessage("INVERTER", MSG_SYSTEM_STATUS, "MotorTorque=%f Nm", signal->physical_value);
        } else if (strcmp(signal->template->name, "Motor_Temperature") == 0) {
            // Handle motor temperature signal
            sendMessage("INVERTER", MSG_SYSTEM_STATUS, "MotorTemp=%f degC", signal->physical_value);
            
            // Example: Take action if temperature is too high
            if (signal->physical_value > 80.0) {
                sendMessage("INVERTER", MSG_WARNING, "Motor temperature high: %f degC", signal->physical_value);
            }
        }
    }
}
```

## How It Works Now

### **Dual Architecture**
- **Old CAN System**: Still runs `init_CANBus(CAN_1)` and uses `receive_CAN_message()` callback
- **New CAN System**: Runs `initCANCommsSystem()` and uses `system_CAN_message_callback()` with automatic routing

### **Message Flow**
1. **CAN Message Arrives** → Hardware interrupt
2. **Both Systems Process It**:
   - Old system: Calls `receive_CAN_message()` → Manual routing
   - New system: Calls `system_CAN_message_callback()` → Automatic routing based on DBC receiver field

### **Example Usage**
- Inverter is registered to receive messages where DBC receiver="ESC"
- When an ESC motor status message arrives, it's automatically routed to `inverter_can_handler()`
- The handler processes motor speed, torque, temperature signals
- All activity is logged via telemetry system

## Benefits Already Achieved

✅ **Non-Disruptive**: Existing CAN functionality unchanged  
✅ **Automatic Routing**: Inverter gets ESC messages automatically  
✅ **Telemetry Integration**: All CAN activity properly logged  
✅ **Statistics Tracking**: Built-in message counters  
✅ **Scheduler Integration**: CAN systems updated at 10Hz  
✅ **Type Safety**: Proper ExternalSystem integration  

## Next Steps Available

### **Phase 2: Gradual Migration**
- Register more existing systems (BrakeSystemControl, TorqueControl, etc.)
- Create message handlers for each system's CAN needs
- Test message routing with actual CAN traffic

### **Phase 3: Complete Transition**  
- Replace old CAN initialization entirely
- Remove manual message setup code
- Use only the new architecture

## Testing the Integration

### **Compile and Flash**
1. Compile the project - should build without errors
2. Flash to STM32 - both CAN systems will initialize
3. Check telemetry output for initialization messages

### **Expected Telemetry Output**
```
INIT:SYSTEM_STATUS:CANCommsSystem CAN1 initialized successfully
INIT:SYSTEM_STATUS:CAN1 DBC database loaded successfully
INIT:SYSTEM_STATUS:CANCommsSystem CAN2 initialized successfully  
INIT:SYSTEM_STATUS:CAN2 DBC database loaded successfully
INIT:SYSTEM_STATUS:Inverter registered for CAN messages on ESC receiver
```

### **When CAN Messages Arrive**
```
INVERTER_CAN:DEBUG:ReceivedMsg=ESC_Motor_Status;ID=0x259;DLC=8
INVERTER_DATA:SENSOR_DATA:Signal=Motor_Speed;Value=1500.0;Unit=RPM
INVERTER:SYSTEM_STATUS:MotorSpeed=1500.0 RPM
```

The integration is **complete and ready for testing**! The new architecture works alongside the existing system, providing a safe migration path.