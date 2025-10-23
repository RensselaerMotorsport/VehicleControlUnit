# Inverter CAN Testing Guide

## 🎯 **Testing the New Inverter CAN Interface**

### **Setup Instructions**

1. **Include the test DBC**: The inverter test DBC file has been created at:
   - `Core/Inc/Files/inverter_test.dbc`
   - `Core/Inc/Files/inverter_test_dbc.h`

2. **Key Test Messages**:
   - **ESC_Motor_Status_1** (ID: 0x102): Speed, Torque, Temperature
   - **ESC_Motor_Status_2** (ID: 0x103): Voltage, Current, Power
   - **VCU_Torque_Command** (ID: 0x200): Command to motor

### **Test Procedure**

#### **Step 1: Verify Integration** 
Your main.c already has:
```c
// CAN system initialization
initCANCommsSystem(&can1_comms, "CAN1_COMMS", 10, CAN_1);

// Inverter registration  
registerCANReceiver(&can1_comms, "ESC", &inverter, inverter_can_handler, EXTERNAL);

// Enhanced handler that stores data in inverter->can_data structure
void inverter_can_handler(void* system, CAN_Message* message);
```

#### **Step 2: Send Test CAN Messages**

Use your telemetry GUI or CAN tools to send these test messages:

**ESC Motor Status 1 (ID: 0x102)**:
```
ID: 0x102 (258 decimal)
Data: [0x9C 0x09 0xF3 0x02 0x69 0x00 0x00 0x00]
Decoded:
  - Motor Speed: 2460 RPM (0x099C)
  - Motor Torque: 75.5 Nm (0x02F3 = 755 → 75.5)
  - Motor Temperature: 65°C (0x69 = 105 → 65+40)
  - Fault Status: 0x00 (No fault)
```

**ESC Motor Status 2 (ID: 0x103)**:
```
ID: 0x103 (259 decimal)  
Data: [0xE2 0x01 0x9C 0x00 0x27 0x00 0x55 0x00]
Decoded:
  - Motor Voltage: 48.2V (0x01E2 = 482 → 48.2)
  - Motor Current: 15.6A (0x009C = 156 → 15.6)
  - Motor Power: 3.9kW (0x0027 = 39 → 3.9)
  - ESC Temperature: 45°C (0x55 = 85 → 45+40)
```

#### **Step 3: Monitor Telemetry Output**

Look for these telemetry messages:

```
INVERTER_CAN:DEBUG:ReceivedMsg=ESC_Motor_Status_1;ID=0x102;DLC=8
INVERTER_DATA:SENSOR_DATA:Signal=Motor_Speed;Value=2460.0;Unit=RPM
INVERTER_DATA:SENSOR_DATA:Signal=Motor_Torque;Value=75.5;Unit=Nm
INVERTER_DATA:SENSOR_DATA:Signal=Motor_Temperature;Value=65.0;Unit=degC
INVERTER:SYSTEM_STATUS:NewMotorSpeed=2460.0 RPM
```

#### **Step 4: Test Data Access API**

In your TorqueControl or other systems, use:

```c
// Check for new data
if (hasNewInverterCANData(&inverter)) {
    float speed = getInverterMotorSpeed(&inverter);
    float torque = getInverterMotorTorque(&inverter);
    float temp = getInverterMotorTemp(&inverter);
    
    printf("Motor: %.1f RPM, %.1f Nm, %.1f°C\n", speed, torque, temp);
}

// Check communication health
if (isInverterCANDataStale(&inverter, 5000)) {  // 5 second timeout
    printf("WARNING: Inverter communication lost!\n");
}
```

#### **Step 5: Test Timeout Detection**

1. Send a few CAN messages to establish communication
2. Stop sending messages for >5 seconds  
3. Watch for timeout warning:
   ```
   INVERTER:ERROR:CAN communication timeout
   ```

### **Expected Behavior**

#### **✅ Success Indicators**:
1. **Message Reception**: `INVERTER_CAN:DEBUG` messages appear
2. **Data Storage**: Values stored in `inverter->can_data` structure
3. **Flag Setting**: `new_data_flags` bits set correctly
4. **API Access**: `getInverterMotorSpeed()` returns correct values  
5. **Timeout Detection**: Warnings after communication loss
6. **Telemetry Integration**: Proper `SENSOR_DATA` messages

#### **🔍 Debug Points**:
1. **Registration**: Check `INIT:SYSTEM_STATUS:Inverter registered for CAN messages on ESC receiver`
2. **Message Routing**: Verify DBC receiver field = "VCU" matches registration
3. **Data Persistence**: Values should persist between CAN messages
4. **Flag Clearing**: `new_data_flags` reset to 0 after processing in `updateInverter()`

### **Test Scenarios**

#### **Scenario A: Normal Operation**
1. Send Motor Status messages every 100ms
2. Verify data updates in real-time
3. Check that `updateInverter()` processes new data correctly

#### **Scenario B: High Temperature**  
1. Send Motor Temperature > 100°C
2. Verify warning: `INVERTER:WARNING:MotorTempHigh=105.0 degC (limit=100)`

#### **Scenario C: Communication Failure**
1. Send messages normally for 10 seconds
2. Stop sending messages  
3. Wait 5 seconds
4. Verify timeout error appears

#### **Scenario D: Multiple Systems**
1. Register multiple systems for different receivers
2. Send messages to different receiver names
3. Verify proper routing to each system

### **Using Your Telemetry GUI**

In your Python telemetry GUI (`main.py`), you can:

1. **Monitor CAN Messages**: Watch the CAN view for automatic message decoding
2. **Send Test Messages**: Use the raw CAN interface to send test data
3. **View Sensor Data**: Check the sensor view for processed inverter data
4. **Debug Routing**: Watch for `CAN_COMMS:DEBUG` routing messages

### **Manual CAN Message Generation**

If you have CAN tools, send these hex sequences:

```bash
# Motor Status 1 (2460 RPM, 75.5 Nm, 65°C)
cansend can0 102#9C09F30269000000

# Motor Status 2 (48.2V, 15.6A, 3.9kW, 45°C)  
cansend can0 103#E2019C002700550

# High temperature test (125°C)
cansend can0 102#9C09F302A5000000
```

This comprehensive test setup will validate:
- ✅ Real-time CAN message processing  
- ✅ Data persistence between interrupts
- ✅ New data detection via flags
- ✅ Clean API for accessing CAN data
- ✅ Timeout detection for safety
- ✅ Integration with existing systems

Ready to test your new CAN architecture! 🚀