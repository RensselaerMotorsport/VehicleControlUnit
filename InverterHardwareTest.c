/**
 * @brief Test functions for Rinehart PM100DX inverter CAN communication
 * These functions simulate inverter responses and test the CAN interface
 */

#include "main.h"
#include "stm32f7xx_hal.h"
#include <string.h>
#include <stdio.h>

// External references (assuming these are defined in main.c)
extern CANCommsSystem can1_comms;
extern Inverter inverter;

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

    // Simulate M161 - Temperatures 2 & Motor Position (ID: 0x0A1)
    uint8_t temp2_msg[8] = {0};
    // Control Board Temp: 45°C
    int16_t ctrl_temp = 450;
    temp2_msg[0] = ctrl_temp & 0xFF;
    temp2_msg[1] = (ctrl_temp >> 8) & 0xFF;
    // RTD1 Temp: 70°C
    int16_t rtd1_temp = 700;
    temp2_msg[2] = rtd1_temp & 0xFF;
    temp2_msg[3] = (rtd1_temp >> 8) & 0xFF;
    // RTD2 Temp: 75°C
    int16_t rtd2_temp = 750;
    temp2_msg[4] = rtd2_temp & 0xFF;
    temp2_msg[5] = (rtd2_temp >> 8) & 0xFF;
    // Motor Temp: 80°C
    int16_t motor_temp = 800;
    temp2_msg[6] = motor_temp & 0xFF;
    temp2_msg[7] = (motor_temp >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0A1, temp2_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M161 Temperatures 2");

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

    // Simulate M163 - Current Information (ID: 0x0A3)
    uint8_t current_msg[8] = {0};
    // Phase A Current: 15.5A (155 * 0.1 = 15.5)
    int16_t phase_a = 155;
    current_msg[0] = phase_a & 0xFF;
    current_msg[1] = (phase_a >> 8) & 0xFF;
    // Phase B Current: -15.5A
    int16_t phase_b = -155;
    current_msg[2] = phase_b & 0xFF;
    current_msg[3] = (phase_b >> 8) & 0xFF;
    // Phase C Current: 15.5A
    int16_t phase_c = 155;
    current_msg[4] = phase_c & 0xFF;
    current_msg[5] = (phase_c >> 8) & 0xFF;
    // DC Bus Current: 12.5A (125 * 0.1 = 12.5)
    int16_t dc_current = 125;
    current_msg[6] = dc_current & 0xFF;
    current_msg[7] = (dc_current >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0A3, current_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M163 Current Info");

    // Simulate M164 - Voltage Information (ID: 0x0A4)
    uint8_t voltage_msg[8] = {0};
    // DC Bus Voltage: 320.0V
    voltage_msg[0] = voltage & 0xFF;
    voltage_msg[1] = (voltage >> 8) & 0xFF;
    // Output Voltage: 240.0V (2400 * 0.1 = 240)
    int16_t out_voltage = 2400;
    voltage_msg[2] = out_voltage & 0xFF;
    voltage_msg[3] = (out_voltage >> 8) & 0xFF;
    // VAB_Vd Voltage: 180.0V (1800 * 0.1 = 180)
    int16_t vab_vd = 1800;
    voltage_msg[4] = vab_vd & 0xFF;
    voltage_msg[5] = (vab_vd >> 8) & 0xFF;
    // VBC_Vq Voltage: 180.0V
    int16_t vbc_vq = 1800;
    voltage_msg[6] = vbc_vq & 0xFF;
    voltage_msg[7] = (vbc_vq >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0A4, voltage_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M164 Voltage Info");

    // Simulate M165 - Flux Information (ID: 0x0A5)
    uint8_t flux_msg[8] = {0};
    // Flux Command: 1.5 Wb (15 * 0.1 = 1.5)
    int16_t flux_cmd = 15;
    flux_msg[0] = flux_cmd & 0xFF;
    flux_msg[1] = (flux_cmd >> 8) & 0xFF;
    // Flux Feedback: 1.4 Wb
    int16_t flux_fb = 14;
    flux_msg[2] = flux_fb & 0xFF;
    flux_msg[3] = (flux_fb >> 8) & 0xFF;
    // Id: 5.0A (50 * 0.1 = 5.0)
    int16_t id = 50;
    flux_msg[4] = id & 0xFF;
    flux_msg[5] = (id >> 8) & 0xFF;
    // Iq: 10.0A (100 * 0.1 = 10.0)
    int16_t iq = 100;
    flux_msg[6] = iq & 0xFF;
    flux_msg[7] = (iq >> 8) & 0xFF;

    sendCANMessage(&can1_comms, 0x0A5, flux_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M165 Flux Info");

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

    // Simulate M167 - Fault Codes (ID: 0x0A7)
    uint8_t fault_msg[8] = {0};
    // Run Fault Lo: 0 (no faults)
    fault_msg[0] = 0;
    fault_msg[1] = 0;
    // Run Fault Hi: 0
    fault_msg[2] = 0;
    fault_msg[3] = 0;
    // Post Fault Lo: 0
    fault_msg[4] = 0;
    fault_msg[5] = 0;
    // Post Fault Hi: 0
    fault_msg[6] = 0;
    fault_msg[7] = 0;

    sendCANMessage(&can1_comms, 0x0A7, fault_msg, 8);
    sendMessage("TEST", MSG_DEBUG, "Sent simulated M167 Fault Codes");

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

/**
 * @brief Test the inverter data processing and command transmission
 */
void test_inverter_system(void) {
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
        float torque_fb = inverterGetTorqueFeedback(&inverter);
        float motor_speed = inverterGetMotorSpeed(&inverter);
        float dc_voltage = inverterGetDCBusVoltage(&inverter);
        float motor_temp = inverterGetMotorTemperature(&inverter);

        sendMessage("INVERTER_TEST", MSG_SENSOR_DATA,
                   "TorqueFB=%.1f Nm;Speed=%.0f RPM;Voltage=%.1f V;Temp=%.1f C",
                   torque_fb, motor_speed, dc_voltage, motor_temp);

        // Test state getters
        InverterState state = inverterGetState(&inverter);
        VSMState vsm_state = inverterGetVSMState(&inverter);
        uint8_t has_fault = inverterHasRunFault(&inverter);

        sendMessage("INVERTER_TEST", MSG_SYSTEM_STATUS,
                   "State=%d;VSMState=%d;HasFault=%d",
                   state, vsm_state, has_fault);

        // Test command transmission (send a test torque command)
        static float test_torque = 10.0f;
        inverterSendTorqueCommand(&inverter, test_torque, 1);
        sendMessage("INVERTER_TEST", MSG_DEBUG, "Sent torque command: %.1f Nm", test_torque);

        // Cycle test torque between 10, 20, 30 Nm
        test_torque += 10.0f;
        if (test_torque > 30.0f) test_torque = 10.0f;
    }
}

/**
 * @brief Hardware test function - call this in main loop for testing
 * This function simulates inverter CAN communication for development/testing
 */
void run_inverter_hardware_test(void) {
    test_inverter_system();
}