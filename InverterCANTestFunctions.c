// Add this function to your main.c USER CODE sections

/**
 * @brief Test function to send inverter CAN messages for testing
 * Call this function to simulate receiving inverter status messages
 */
void send_test_inverter_messages(void) {
    static int test_counter = 0;
    test_counter++;
    
    // Test Message 1: ESC Motor Status 1 (ID: 0x102)
    // Speed: 2000 + test_counter RPM, Torque: 50.5 Nm, Temp: 60°C
    uint8_t motor_status_1[8];
    uint16_t speed = 2000 + (test_counter * 10); // Increasing speed
    motor_status_1[0] = speed & 0xFF;
    motor_status_1[1] = (speed >> 8) & 0xFF;
    
    int16_t torque = 505; // 50.5 Nm * 10
    motor_status_1[2] = torque & 0xFF;
    motor_status_1[3] = (torque >> 8) & 0xFF;
    
    motor_status_1[4] = 100; // 60°C + 40
    motor_status_1[5] = 0;   // No fault
    motor_status_1[6] = 0;
    motor_status_1[7] = 0;
    
    // Send via CAN1
    send_CAN_message(CAN_1, CAN_2A, 0x102, motor_status_1, 8);
    
    sendMessage("TEST", MSG_DEBUG, "SentTestMsg=ESC_Motor_Status_1;Speed=%d;Torque=50.5", speed);
    
    // Test Message 2: ESC Motor Status 2 (ID: 0x103)  
    // Voltage: 48.0V, Current: 12.5A, Power: 2.5kW, ESC Temp: 45°C
    uint8_t motor_status_2[8];
    uint16_t voltage = 480; // 48.0V * 10
    motor_status_2[0] = voltage & 0xFF;
    motor_status_2[1] = (voltage >> 8) & 0xFF;
    
    int16_t current = 125; // 12.5A * 10
    motor_status_2[2] = current & 0xFF;
    motor_status_2[3] = (current >> 8) & 0xFF;
    
    uint16_t power = 25; // 2.5kW * 10
    motor_status_2[4] = power & 0xFF;
    motor_status_2[5] = (power >> 8) & 0xFF;
    
    motor_status_2[6] = 85; // 45°C + 40
    motor_status_2[7] = 0;
    
    // Send via CAN1  
    send_CAN_message(CAN_1, CAN_2A, 0x103, motor_status_2, 8);
    
    sendMessage("TEST", MSG_DEBUG, "SentTestMsg=ESC_Motor_Status_2;Voltage=48.0;Current=12.5");
}

/**
 * @brief Test the inverter data access API
 * Call this to check if the inverter has received and stored CAN data
 */
void test_inverter_data_access(void) {
    // Check if inverter has new data
    if (hasNewInverterCANData(&inverter)) {
        sendMessage("TEST", MSG_SYSTEM_STATUS, "InverterNewDataDetected=TRUE");
        
        // Read all the data
        float speed = getInverterMotorSpeed(&inverter);
        float torque = getInverterMotorTorque(&inverter);
        float temp = getInverterMotorTemp(&inverter);
        float voltage = getInverterMotorVoltage(&inverter);
        float current = getInverterMotorCurrent(&inverter);
        
        sendMessage("INVERTER_API", MSG_SENSOR_DATA, 
                   "Speed=%f;Torque=%f;Temp=%f;Voltage=%f;Current=%f", 
                   speed, torque, temp, voltage, current);
        
        // Check data age
        uint32_t age = getInverterDataAge(&inverter);
        sendMessage("INVERTER_API", MSG_DEBUG, "DataAge=%lu ms", age);
        
        // Test timeout detection
        if (isInverterCANDataStale(&inverter, 1000)) {
            sendMessage("INVERTER_API", MSG_WARNING, "DataStale=TRUE (>1000ms)");
        } else {
            sendMessage("INVERTER_API", MSG_DEBUG, "DataFresh=TRUE (<1000ms)");
        }
        
    } else {
        sendMessage("TEST", MSG_DEBUG, "InverterNewDataDetected=FALSE");
    }
}

/**
 * @brief Add this to your main() function after all initialization
 * This will run a continuous test of the CAN system
 */
void run_inverter_can_tests(void) {
    static uint32_t last_test_time = 0;
    static int test_phase = 0;
    
    uint32_t current_time = HAL_GetTick();
    
    // Run tests every 2 seconds
    if (current_time - last_test_time > 2000) {
        last_test_time = current_time;
        
        switch (test_phase) {
            case 0:
                sendMessage("TEST", MSG_SYSTEM_STATUS, "StartingInverterCANTest=Phase1");
                send_test_inverter_messages();
                test_phase = 1;
                break;
                
            case 1:
                test_inverter_data_access();
                test_phase = 2;
                break;
                
            case 2:
                sendMessage("TEST", MSG_SYSTEM_STATUS, "InverterCANTest=Continuous");
                send_test_inverter_messages();
                test_phase = 1; // Loop back to data access test
                break;
        }
    }
}

// ============================
// INTEGRATION INSTRUCTIONS:
// ============================
//
// 1. Add these functions to your main.c in USER CODE sections
// 2. Add this call to your main() function after initialization:
//    
//    /* USER CODE BEGIN WHILE */
//    while (1)
//    {
//        run_inverter_can_tests(); // ADD THIS LINE
//        
//        /* USER CODE END WHILE */
//        /* USER CODE BEGIN 3 */
//    }
//
// 3. Compile and flash to STM32
// 4. Watch telemetry output for:
//    - TEST:DEBUG messages showing sent CAN data
//    - INVERTER_CAN:DEBUG messages showing received data  
//    - INVERTER_DATA:SENSOR_DATA showing processed signals
//    - INVERTER_API messages showing data access
//
// 5. Expected telemetry sequence:
//    TEST:SYSTEM_STATUS:StartingInverterCANTest=Phase1
//    TEST:DEBUG:SentTestMsg=ESC_Motor_Status_1;Speed=2010;Torque=50.5
//    INVERTER_CAN:DEBUG:ReceivedMsg=ESC_Motor_Status_1;ID=0x102;DLC=8
//    INVERTER_DATA:SENSOR_DATA:Signal=Motor_Speed;Value=2010.0;Unit=RPM
//    INVERTER:SYSTEM_STATUS:NewMotorSpeed=2010.0 RPM
//    TEST:SYSTEM_STATUS:InverterNewDataDetected=TRUE
//    INVERTER_API:SENSOR_DATA:Speed=2010.0;Torque=50.5;Temp=60.0;...
//