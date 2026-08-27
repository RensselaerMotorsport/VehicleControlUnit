#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Utils/MessageFormat.h"
#include "../../../Inc/Systems/External/Inverter.h"
#include "../../../Inc/Systems/Comms/CAN/Can.h"
#include "../../../Inc/Systems/Comms/CAN/CANCommsSystem.h"
#include "../../../Inc/Utils/Telemetry.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

// Forward declaration
extern uint32_t HAL_GetTick(void);

void initInverter(Inverter* inverter, CANCommsSystem* can_comms, int hz, uint8_t can_bus, uint32_t heartbeat_timeout_ms) {
    // Initialize base external system
    initExternalSystem(&inverter->base, "Rinehart PM100DX Inverter", hz, e_INVERTER, 
                       updateInverter, inverterCheckHeartbeat, inverter);
    
    // Initialize inverter-specific fields
    inverter->can_comms = can_comms;
    inverter->can_bus = can_bus;
    inverter->update_hz = hz;
    inverter->heartbeat_timeout_ms = heartbeat_timeout_ms;
    inverter->heartbeat_active = 0;
    inverter->last_message_timestamp = 0;
    
    // Initialize RX and TX data structures
    memset(&inverter->rx_data, 0, sizeof(InverterRXData));
    memset(&inverter->tx_command, 0, sizeof(InverterTXCommand));
    
    // Set sensible defaults for TX
    inverter->tx_command.torque_command_raw = 0;
    inverter->tx_command.speed_command_raw = 0;
    inverter->tx_command.torque_limit_command_raw = 0;
    inverter->tx_command.direction_command = 0;  // Reverse
    inverter->tx_command.speed_mode_enable = 0;  // Torque mode
    inverter->tx_command.inverter_enable = 0;    // Disabled
    inverter->tx_command.inverter_discharge = 0; // Disabled
    inverter->tx_command.rolling_counter = 0;
    
    // Register telemetry signals
    inverter->telem_torque_feedback = registerTelemetrySignal("Inverter_Torque_Feedback", TELEMETRY_SENSOR, UNIT_DIMENSIONLESS, 100, -1000.0f, 1000.0f);
    inverter->telem_motor_speed = registerTelemetrySignal("Inverter_Motor_Speed", TELEMETRY_SENSOR, UNIT_RPM, 100, -10000.0f, 10000.0f);
    inverter->telem_dc_bus_voltage = registerTelemetrySignal("Inverter_DC_Bus_Voltage", TELEMETRY_SENSOR, UNIT_VOLTS, 100, 0.0f, 1000.0f);
    inverter->telem_dc_bus_current = registerTelemetrySignal("Inverter_DC_Bus_Current", TELEMETRY_SENSOR, UNIT_AMPERES, 100, -500.0f, 500.0f);
    inverter->telem_motor_temp = registerTelemetrySignal("Inverter_Motor_Temp", TELEMETRY_SENSOR, UNIT_CELSIUS, 1000, -50.0f, 200.0f);
    inverter->telem_inverter_state = registerTelemetrySignal("Inverter_State", TELEMETRY_STATUS, UNIT_ENUM, 1000, 0.0f, 15.0f);
    inverter->telem_vsm_state = registerTelemetrySignal("Inverter_VSM_State", TELEMETRY_STATUS, UNIT_ENUM, 1000, 0.0f, 15.0f);
    inverter->telem_torque_command = registerTelemetrySignal("Inverter_Torque_Command", TELEMETRY_OUTPUT, UNIT_DIMENSIONLESS, 100, -1000.0f, 1000.0f);
}

int updateInverter(ExternalSystem* ext) {
    Inverter* inverter = (Inverter*)ext->child;
    
    // Hardware testing mode - simulate inverter messages
    #ifdef INVERTER_HARDWARE_TEST
    static uint32_t last_test_time = 0;
    uint32_t current_time = HAL_GetTick();
    
    // Send simulated messages every 1 second
    if (current_time - last_test_time > 1000) {
        last_test_time = current_time;
        send_simulated_inverter_messages();
    }
    
    // Test data access every 2 seconds
    static uint32_t last_data_test = 0;
    if (current_time - last_data_test > 2000) {
        last_data_test = current_time;
        
        // Test getter functions
        float torque_fb = inverterGetTorqueFeedback(inverter);
        float motor_speed = inverterGetMotorSpeed(inverter);
        float dc_voltage = inverterGetDCBusVoltage(inverter);
        float motor_temp = inverterGetMotorTemperature(inverter);
        
        sendMessage("INVERTER_TEST", MSG_SENSOR_DATA,
                   "TorqueFB=%.1f Nm;Speed=%.0f RPM;Voltage=%.1f V;Temp=%.1f C",
                   torque_fb, motor_speed, dc_voltage, motor_temp);
        
        // Test state getters
        InverterState state = inverterGetState(inverter);
        VSMState vsm_state = inverterGetVSMState(inverter);
        uint8_t has_fault = inverterHasRunFault(inverter);
        
        sendMessage("INVERTER_TEST", MSG_SYSTEM_STATUS,
                   "State=%d;VSMState=%d;HasFault=%d",
                   state, vsm_state, has_fault);
    }
    #endif
    
    // Check heartbeat
    if (inverter->heartbeat_active == 0) {
        sendMessage("INVERTER", MSG_WARNING, "Inverter heartbeat inactive");
    }
    
    // Check for faults
    if (inverter->rx_data.run_fault_lo || inverter->rx_data.run_fault_hi) {
        sendMessage("INVERTER", MSG_WARNING, 
                   "Run faults detected: Lo=0x%04X Hi=0x%04X", 
                   inverter->rx_data.run_fault_lo, inverter->rx_data.run_fault_hi);
    }
    
    if (inverter->rx_data.post_fault_lo || inverter->rx_data.post_fault_hi) {
        sendMessage("INVERTER", MSG_WARNING, 
                   "Post faults detected: Lo=0x%04X Hi=0x%04X", 
                   inverter->rx_data.post_fault_lo, inverter->rx_data.post_fault_hi);
    }
    
    // Check temperature limits (example: 80°C warning, 100°C error)
    float motor_temp = inverterGetMotorTemperature(inverter);
    if (motor_temp > 100.0f) {
        sendMessage("INVERTER", MSG_ERROR, "Motor temperature CRITICAL: %.1f°C", motor_temp);
    } else if (motor_temp > 80.0f) {
        sendMessage("INVERTER", MSG_WARNING, "Motor temperature HIGH: %.1f°C", motor_temp);
    }
    
    // Send telemetry data
    sendTelemetryValue(inverter->telem_torque_feedback, inverterGetTorqueFeedback(inverter));
    sendTelemetryValue(inverter->telem_motor_speed, inverterGetMotorSpeed(inverter));
    sendTelemetryValue(inverter->telem_dc_bus_voltage, inverterGetDCBusVoltage(inverter));
    sendTelemetryValue(inverter->telem_dc_bus_current, inverterGetDCBusCurrent(inverter));
    sendTelemetryValue(inverter->telem_motor_temp, motor_temp);
    sendTelemetryValue(inverter->telem_inverter_state, (float)inverterGetState(inverter));
    sendTelemetryValue(inverter->telem_vsm_state, (float)inverterGetVSMState(inverter));
    sendTelemetryValue(inverter->telem_torque_command, inverter->tx_command.torque_command_raw * 0.1f);
    
    // Send periodic command message to keep inverter updated
    inverterSendCommandMessage(inverter);
    
    return _SUCCESS;
}

int inverterCheckHeartbeat(void* inv) {
    Inverter* inverter = (Inverter*)inv;
    uint32_t now = HAL_GetTick();
    
    if (inverter->last_message_timestamp == 0) {
        inverter->heartbeat_active = 0;
        return _FAILURE;
    }
    
    uint32_t time_since_last = now - inverter->last_message_timestamp;
    if (time_since_last > inverter->heartbeat_timeout_ms) {
        inverter->heartbeat_active = 0;
        return _FAILURE;
    }
    
    inverter->heartbeat_active = 1;
    return _SUCCESS;
}

// CAN message handler - called by CANCommsSystem when messages arrive for "VCU_Inverter_Ext"
void inverterCANMessageHandler(void* system, CAN_Message* message) {
    Inverter* inverter = (Inverter*)system;
    
    // Update heartbeat timestamp
    inverter->last_message_timestamp = HAL_GetTick();
    
    // Process each signal in the message
    for (int i = 0; i < message->template->signal_count; i++) {
        CAN_Signal* signal = &message->signals[i];
        const char* signal_name = signal->template->name;
        
        // Map signal names to InverterRXData fields
        // Note: signal->value already has scale/offset applied by CANCommsSystem
        
        if (strcmp(signal_name, "INV_Commanded_Torque") == 0) {
            inverter->rx_data.commanded_torque_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Torque_Feedback") == 0) {
            inverter->rx_data.torque_feedback_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Power_On_Timer") == 0) {
            inverter->rx_data.power_on_timer = (uint32_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Inverter_State") == 0) {
            inverter->rx_data.inverter_state = (InverterState)signal->value;
        }
        else if (strcmp(signal_name, "INV_VSM_State") == 0) {
            inverter->rx_data.vsm_state = (VSMState)signal->value;
        }
        else if (strcmp(signal_name, "INV_PWM_Frequency") == 0) {
            inverter->rx_data.pwm_frequency = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Rolling_Counter") == 0) {
            inverter->rx_data.rolling_counter = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Inverter_Enable_State") == 0) {
            inverter->rx_data.inverter_enabled = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Direction_Command") == 0) {
            inverter->rx_data.direction_command = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Inverter_Run_Mode") == 0) {
            inverter->rx_data.run_mode = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Inverter_Command_Mode") == 0) {
            inverter->rx_data.command_mode = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Inverter_Enable_Lockout") == 0) {
            inverter->rx_data.enable_lockout = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Start_Mode_Active") == 0) {
            inverter->rx_data.start_mode_active = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Relay_1_Status") == 0) {
            inverter->rx_data.relay_1_status = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Relay_2_Status") == 0) {
            inverter->rx_data.relay_2_status = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Relay_3_Status") == 0) {
            inverter->rx_data.relay_3_status = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Relay_4_Status") == 0) {
            inverter->rx_data.relay_4_status = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Relay_5_Status") == 0) {
            inverter->rx_data.relay_5_status = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Relay_6_Status") == 0) {
            inverter->rx_data.relay_6_status = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Inverter_Discharge_State") == 0) {
            inverter->rx_data.discharge_state = (DischargeState)signal->value;
        }
        else if (strcmp(signal_name, "INV_BMS_Active") == 0) {
            inverter->rx_data.bms_active = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_BMS_Torque_Limiting") == 0) {
            inverter->rx_data.bms_torque_limiting = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Max_Speed_Limiting") == 0) {
            inverter->rx_data.max_speed_limiting = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Low_Speed_Limiting") == 0) {
            inverter->rx_data.low_speed_limiting = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Run_Fault_Lo") == 0) {
            inverter->rx_data.run_fault_lo = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Run_Fault_Hi") == 0) {
            inverter->rx_data.run_fault_hi = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Post_Fault_Lo") == 0) {
            inverter->rx_data.post_fault_lo = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Post_Fault_Hi") == 0) {
            inverter->rx_data.post_fault_hi = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Fast_Torque_Command") == 0) {
            inverter->rx_data.fast_torque_command_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Fast_Torque_Feedback") == 0) {
            inverter->rx_data.fast_torque_feedback_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Fast_Motor_Speed") == 0) {
            inverter->rx_data.fast_motor_speed_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Fast_DC_Bus_Voltage") == 0) {
            inverter->rx_data.fast_dc_bus_voltage_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Phase_A_Current") == 0) {
            inverter->rx_data.phase_a_current_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Phase_B_Current") == 0) {
            inverter->rx_data.phase_b_current_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Phase_C_Current") == 0) {
            inverter->rx_data.phase_c_current_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_DC_Bus_Current") == 0) {
            inverter->rx_data.dc_bus_current_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_DC_Bus_Voltage") == 0) {
            inverter->rx_data.dc_bus_voltage_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Output_Voltage") == 0) {
            inverter->rx_data.output_voltage_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_VAB_Vd_Voltage") == 0) {
            inverter->rx_data.vab_Vd_voltage_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_VBC_Vq_Voltage") == 0) {
            inverter->rx_data.vbc_Vq_voltage_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Motor_Speed") == 0) {
            inverter->rx_data.motor_speed_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Motor_Angle_Electrical") == 0) {
            inverter->rx_data.motor_angle_electrical_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Electrical_Output_Frequency") == 0) {
            inverter->rx_data.electrical_output_frequency_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Delta_Resolver_Filtered") == 0) {
            inverter->rx_data.delta_resolver_filtered_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Flux_Command") == 0) {
            inverter->rx_data.flux_command_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Flux_Feedback") == 0) {
            inverter->rx_data.flux_feedback_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Id") == 0) {
            inverter->rx_data.id_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Iq") == 0) {
            inverter->rx_data.iq_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Module_A") == 0) {
            inverter->rx_data.module_a_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Module_B") == 0) {
            inverter->rx_data.module_b_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Module_C") == 0) {
            inverter->rx_data.module_c_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Gate_Driver_Board") == 0) {
            inverter->rx_data.gate_driver_board_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Control_Board_Temperature") == 0) {
            inverter->rx_data.control_board_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_RTD1_Temperature") == 0) {
            inverter->rx_data.rtd1_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_RTD2_Temperature") == 0) {
            inverter->rx_data.rtd2_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_RTD3_Temperature") == 0) {
            inverter->rx_data.rtd3_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_RTD4_Temperature") == 0) {
            inverter->rx_data.rtd4_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_RTD5_Temperature") == 0) {
            inverter->rx_data.rtd5_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Motor_Temperature") == 0) {
            inverter->rx_data.motor_temp_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Torque_Shudder") == 0) {
            // This is part of M162, store in appropriate field if needed
        }
        else if (strcmp(signal_name, "INV_Analog_Input_1") == 0) {
            inverter->rx_data.analog_input_1_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Analog_Input_2") == 0) {
            inverter->rx_data.analog_input_2_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Analog_Input_3") == 0) {
            inverter->rx_data.analog_input_3_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Analog_Input_4") == 0) {
            inverter->rx_data.analog_input_4_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Analog_Input_5") == 0) {
            inverter->rx_data.analog_input_5_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Analog_Input_6") == 0) {
            inverter->rx_data.analog_input_6_raw = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_1") == 0) {
            inverter->rx_data.digital_input_1 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_2") == 0) {
            inverter->rx_data.digital_input_2 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_3") == 0) {
            inverter->rx_data.digital_input_3 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_4") == 0) {
            inverter->rx_data.digital_input_4 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_5") == 0) {
            inverter->rx_data.digital_input_5 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_6") == 0) {
            inverter->rx_data.digital_input_6 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_7") == 0) {
            inverter->rx_data.digital_input_7 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Digital_Input_8") == 0) {
            inverter->rx_data.digital_input_8 = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Reference_Voltage_1_5") == 0) {
            inverter->rx_data.ref_voltage_1_5_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Reference_Voltage_2_5") == 0) {
            inverter->rx_data.ref_voltage_2_5_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Reference_Voltage_5_0") == 0) {
            inverter->rx_data.ref_voltage_5_0_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Reference_Voltage_12_0") == 0) {
            inverter->rx_data.ref_voltage_12_0_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Modulation_Index") == 0) {
            inverter->rx_data.modulation_index_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Flux_Weakening_Output") == 0) {
            inverter->rx_data.flux_weakening_output_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Id_Command") == 0) {
            inverter->rx_data.id_command_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Iq_Command") == 0) {
            inverter->rx_data.iq_command_raw = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_SW_Version") == 0) {
            inverter->rx_data.sw_version = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Project_Code_EEP_Ver") == 0) {
            inverter->rx_data.project_code_eep_ver = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_DateCode_MMDD") == 0) {
            inverter->rx_data.date_code_mmdd = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_DateCode_YYYY") == 0) {
            inverter->rx_data.date_code_yyyy = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Write_Success") == 0) {
            inverter->rx_data.write_success = (uint8_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Data_Response") == 0) {
            inverter->rx_data.data_response = (int16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Parameter_Address_Response") == 0) {
            inverter->rx_data.param_address_response = (uint16_t)signal->value;
        }
        else if (strcmp(signal_name, "INV_Max_Discharge_Current") == 0) {
            // BMS current limit - could be stored separately if needed
        }
        else if (strcmp(signal_name, "INV_Max_Charge_Current") == 0) {
            // BMS current limit - could be stored separately if needed
        }
    }
}

// CAN message RX callbacks (called from CAN interrupt handlers)
void inverterProcessM176(uint8_t* data) {
    // M176 - Fast Info (5ms): Torque Command/Feedback, Motor Speed, DC Bus Voltage
    int16_t torque_cmd = (data[1] << 8) | data[0];
    int16_t torque_fb = (data[3] << 8) | data[2];
    int16_t speed = (data[5] << 8) | data[4];
    int16_t voltage = (data[7] << 8) | data[6];
    
    // Store raw values - will be converted to physical units during getter calls
    // (Implementation would hook into actual inverter instance)
}

// Getter functions for physical values
float inverterGetTorqueFeedback(Inverter* inv) {
    return inv->rx_data.torque_feedback_raw * 0.1f;  // 0.1 Nm scale
}

float inverterGetMotorSpeed(Inverter* inv) {
    return (float)inv->rx_data.motor_speed_raw;  // 1 rpm scale
}

float inverterGetDCBusVoltage(Inverter* inv) {
    return inv->rx_data.dc_bus_voltage_raw * 0.1f;  // 0.1 V scale
}

float inverterGetDCBusCurrent(Inverter* inv) {
    return inv->rx_data.dc_bus_current_raw * 0.1f;  // 0.1 A scale
}

float inverterGetMotorTemperature(Inverter* inv) {
    return inv->rx_data.motor_temp_raw * 0.1f;  // 0.1 C scale
}

float inverterGetModuleATemp(Inverter* inv) {
    return inv->rx_data.module_a_temp_raw * 0.1f;  // 0.1 C scale
}

float inverterGetModuleBTemp(Inverter* inv) {
    return inv->rx_data.module_b_temp_raw * 0.1f;  // 0.1 C scale
}

float inverterGetModuleCTemp(Inverter* inv) {
    return inv->rx_data.module_c_temp_raw * 0.1f;  // 0.1 C scale
}

// CAN message transmission functions
void inverterSendCommandMessage(Inverter* inv) {
    if (!inv->can_comms) return;
    
    // M192 - Command Message (ID 0x180)
    uint8_t data[8] = {0};
    
    // Pack signals according to DBC (little-endian, specific bit positions)
    // Torque Command: bits 0-15 (0.1 Nm scale)
    data[0] = (inv->tx_command.torque_command_raw >> 0) & 0xFF;
    data[1] = (inv->tx_command.torque_command_raw >> 8) & 0xFF;
    
    // Speed Command: bits 16-31 (1 rpm scale)
    data[2] = (inv->tx_command.speed_command_raw >> 0) & 0xFF;
    data[3] = (inv->tx_command.speed_command_raw >> 8) & 0xFF;
    
    // Direction Command: bit 32
    data[4] |= (inv->tx_command.direction_command & 0x01) << 0;
    
    // Inverter Enable: bit 40
    data[5] |= (inv->tx_command.inverter_enable & 0x01) << 0;
    
    // Speed Mode Enable: bit 42
    data[5] |= (inv->tx_command.speed_mode_enable & 0x01) << 2;
    
    // Inverter Discharge: bit 41
    data[5] |= (inv->tx_command.inverter_discharge & 0x01) << 1;
    
    // Torque Limit Command: bits 48-63 (0.1 Nm scale)
    data[6] = (inv->tx_command.torque_limit_command_raw >> 0) & 0xFF;
    data[7] = (inv->tx_command.torque_limit_command_raw >> 8) & 0xFF;
    
    // Rolling Counter: bits 44-47 (increment each message)
    data[5] |= ((inv->tx_command.rolling_counter & 0x0F) << 4);
    
    // Increment rolling counter for next message
    inv->tx_command.rolling_counter = (inv->tx_command.rolling_counter + 1) & 0x0F;
    
    // Send the message
    sendCANMessage(inv->can_comms, 0x180, data, 8);
}

void inverterSendParamCommand(Inverter* inv, uint16_t param_addr, int16_t data_value, uint8_t read_write) {
    if (!inv->can_comms) return;
    
    // M193 - Read/Write Param Command (ID 0x181)
    uint8_t data[8] = {0};
    
    // Data Command: bits 32-47
    data[4] = (data_value >> 0) & 0xFF;
    data[5] = (data_value >> 8) & 0xFF;
    
    // Read/Write Command: bit 16
    data[2] |= (read_write & 0x01) << 0;
    
    // Parameter Address Command: bits 0-15
    data[0] = (param_addr >> 0) & 0xFF;
    data[1] = (param_addr >> 8) & 0xFF;
    
    // Send the message
    sendCANMessage(inv->can_comms, 0x181, data, 8);
}

// Command functions (now with actual transmission)
void inverterSendTorqueCommand(Inverter* inv, float torque_nm, uint8_t enable) {
    inv->tx_command.torque_command_raw = (int16_t)(torque_nm / 0.1f);  // Convert to raw value
    inv->tx_command.inverter_enable = enable;
    inv->tx_command.speed_mode_enable = 0;  // Torque mode
    
    // Send the command message
    inverterSendCommandMessage(inv);
}

void inverterSendSpeedCommand(Inverter* inv, int16_t speed_rpm, uint8_t enable) {
    inv->tx_command.speed_command_raw = speed_rpm;  // Direct RPM value
    inv->tx_command.inverter_enable = enable;
    inv->tx_command.speed_mode_enable = 1;  // Speed mode
    
    // Send the command message
    inverterSendCommandMessage(inv);
}

void inverterSetDirection(Inverter* inv, uint8_t forward) {
    inv->tx_command.direction_command = forward ? 1 : 0;
    
    // Send updated command message
    inverterSendCommandMessage(inv);
}

void inverterSetEnable(Inverter* inv, uint8_t enabled) {
    inv->tx_command.inverter_enable = enabled;
    
    // Send updated command message
    inverterSendCommandMessage(inv);
}

void inverterSetDischarge(Inverter* inv, uint8_t discharge_enabled) {
    inv->tx_command.inverter_discharge = discharge_enabled;
    
    // Send updated command message
    inverterSendCommandMessage(inv);
}

void inverterSetTorqueLimit(Inverter* inv, float torque_limit_nm) {
    inv->tx_command.torque_limit_command_raw = (int16_t)(torque_limit_nm / 0.1f);
    
    // Send updated command message
    inverterSendCommandMessage(inv);
}

// Fault checking functions
uint8_t inverterHasRunFault(Inverter* inv) {
    return (inv->rx_data.run_fault_lo != 0) || (inv->rx_data.run_fault_hi != 0);
}

uint8_t inverterHasPostFault(Inverter* inv) {
    return (inv->rx_data.post_fault_lo != 0) || (inv->rx_data.post_fault_hi != 0);
}

// State getters
InverterState inverterGetState(Inverter* inv) {
    return inv->rx_data.inverter_state;
}

VSMState inverterGetVSMState(Inverter* inv) {
    return inv->rx_data.vsm_state;
}

// ============================================================================
// HARDWARE TESTING FUNCTIONS
// ============================================================================

#ifdef INVERTER_HARDWARE_TEST

/**
 * @brief Send simulated inverter status messages for testing
 * This simulates the Rinehart PM100DX sending status messages to VCU
 */
void send_simulated_inverter_messages(void) {
    static uint8_t rolling_counter = 0;

    // Simulate M160 - Temperatures 1 (ID: 0x0A0)
    uint8_t temp_msg[8] = {0};
    // Module A Temp: 60°C (600 * 0.1 = 60)
    int16_t mod_a_temp = 600;
    temp_msg[0] = mod_a_temp & 0xFF;
    temp_msg[1] = (mod_a_temp >> 8) & 0xFF;
    // Module B Temp: 65°C
    int16_t mod_b_temp = 650;
    temp_msg[2] = mod_b_temp & 0xFF;
    temp_msg[3] = (mod_b_temp >> 8) & 0xFF;
    // Module C Temp: 55°C
    int16_t mod_c_temp = 550;
    temp_msg[4] = mod_c_temp & 0xFF;
    temp_msg[5] = (mod_c_temp >> 8) & 0xFF;
    // Gate Driver Temp: 50°C
    int16_t gate_temp = 500;
    temp_msg[6] = gate_temp & 0xFF;
    temp_msg[7] = (gate_temp >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0A0, temp_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M160 Temperatures");

    // Simulate M176 - Fast Info (ID: 0x0B0) - This is the most important one
    uint8_t fast_msg[8] = {0};
    // Torque Feedback: 25.5 Nm (255 * 0.1 = 25.5)
    int16_t torque_fb = 255;
    fast_msg[0] = torque_fb & 0xFF;
    fast_msg[1] = (torque_fb >> 8) & 0xFF;
    // Torque Command: 30.0 Nm
    int16_t torque_cmd = 300;
    fast_msg[2] = torque_cmd & 0xFF;
    fast_msg[3] = (torque_cmd >> 8) & 0xFF;
    // Motor Speed: 1500 RPM
    int16_t speed = 1500;
    fast_msg[4] = speed & 0xFF;
    fast_msg[5] = (speed >> 8) & 0xFF;
    // DC Bus Voltage: 320.0V (3200 * 0.1 = 320)
    int16_t voltage = 3200;
    fast_msg[6] = voltage & 0xFF;
    fast_msg[7] = (voltage >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0B0, fast_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M176 Fast Info");

    // Simulate M162 - Torque & Speed (ID: 0x0A2)
    uint8_t torque_msg[8] = {0};
    // Commanded Torque: 30.0 Nm
    torque_msg[0] = torque_cmd & 0xFF;
    torque_msg[1] = (torque_cmd >> 8) & 0xFF;
    // Torque Feedback: 25.5 Nm
    torque_msg[2] = torque_fb & 0xFF;
    torque_msg[3] = (torque_fb >> 8) & 0xFF;
    // Motor Speed: 1500 RPM
    torque_msg[4] = speed & 0xFF;
    torque_msg[5] = (speed >> 8) & 0xFF;
    // Electrical Output Frequency: 50.0 Hz (500 * 0.1 = 50)
    int16_t freq = 500;
    torque_msg[6] = freq & 0xFF;
    torque_msg[7] = (freq >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0A2, torque_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M162 Torque & Speed");

    // Simulate M166 - Internal States (ID: 0x0A6)
    uint8_t state_msg[8] = {0};
    // VSM State: 1 (Startup)
    state_msg[0] = 1;
    // Inverter State: 2 (Ready)
    state_msg[1] = 2;
    // Relay Status: All on
    state_msg[2] = 1; // Relay 1
    state_msg[3] = 1; // Relay 2
    state_msg[4] = 1; // Relay 3
    state_msg[5] = 1; // Relay 4
    state_msg[6] = 1; // Relay 5
    state_msg[7] = 1; // Relay 6

    sendCANMessage(&can1_comms, 0x0A6, state_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M166 Internal States");

    // Simulate M168 - Torque Timer (ID: 0x0A8)
    uint8_t timer_msg[8] = {0};
    // Power On Timer: 3600 seconds (1 hour)
    uint32_t power_timer = 3600;
    timer_msg[0] = power_timer & 0xFF;
    timer_msg[1] = (power_timer >> 8) & 0xFF;
    timer_msg[2] = (power_timer >> 16) & 0xFF;
    timer_msg[3] = (power_timer >> 24) & 0xFF;
    // Rolling Counter: increment each message
    timer_msg[4] = rolling_counter;
    // Inverter Enable State: 1 (enabled)
    timer_msg[5] = 1;
    // Direction Command: 1 (forward)
    timer_msg[6] = 1;
    // Inverter Run Mode: 1 (enabled)
    timer_msg[7] = 1;

    sendCANMessage(&can1_comms, 0x0A8, timer_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M168 Torque Timer");

    rolling_counter = (rolling_counter + 1) & 0x0F;
}

#endif // INVERTER_HARDWARE_TEST
