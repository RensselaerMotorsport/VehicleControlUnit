#ifndef INVERTER_TEST_AUTO_GENERATED_H
#define INVERTER_TEST_AUTO_GENERATED_H

#include <stdint.h>

// Auto-generated from DBC file
// This shows what CAN be automatically generated

// ========== MESSAGE IDs (Auto-Generatable) ==========
#define ESC_MOTOR_STATUS_1_ID               0x102  // 258
#define ESC_MOTOR_STATUS_2_ID               0x103  // 259
#define ESC_CONTROL_STATUS_ID               0x104  // 260
#define VCU_TORQUE_COMMAND_ID               0x200  // 512
#define VCU_SYSTEM_STATUS_ID                0x201  // 513

// ========== SIGNAL DEFINITIONS (Auto-Generatable) ==========
// Message: ESC_Motor_Status_1 (ID: 0x102, Size: 8 bytes)
//   Motor_Speed: bit 0|16 (unsigned, receivers=VCU)
//   Motor_Torque: bit 16|16 (scale=0.1, signed, receivers=VCU)
//   Motor_Temperature: bit 32|8 (offset=-40.0, unsigned, receivers=VCU)
//   Motor_Fault_Status: bit 40|8 (unsigned, receivers=VCU)

// Message: ESC_Motor_Status_2 (ID: 0x103, Size: 8 bytes)
//   Motor_Voltage: bit 0|16 (scale=0.1, unsigned, receivers=VCU)
//   Motor_Current: bit 16|16 (scale=0.1, signed, receivers=VCU)
//   Motor_Power: bit 32|16 (scale=0.1, unsigned, receivers=VCU)
//   ESC_Temperature: bit 48|8 (offset=-40.0, unsigned, receivers=VCU)

// Message: ESC_Control_Status (ID: 0x104, Size: 8 bytes)
//   ESC_State: bit 0|8 (unsigned, receivers=VCU)
//   Inverter_Enable: bit 8|1 (unsigned, receivers=VCU)
//   Direction_Command: bit 9|2 (unsigned, receivers=VCU)
//   Torque_Limit: bit 16|16 (scale=0.1, unsigned, receivers=VCU)
//   Speed_Limit: bit 32|16 (unsigned, receivers=VCU)

// Message: VCU_Torque_Command (ID: 0x200, Size: 8 bytes)
//   Requested_Torque: bit 0|16 (scale=0.1, signed, receivers=ESC)
//   Torque_Enable: bit 16|1 (unsigned, receivers=ESC)
//   Direction: bit 17|1 (unsigned, receivers=ESC)
//   Speed_Command: bit 24|16 (unsigned, receivers=ESC)
//   Emergency_Stop: bit 40|1 (unsigned, receivers=ESC)

// Message: VCU_System_Status (ID: 0x201, Size: 8 bytes)
//   VCU_State: bit 0|8 (unsigned, receivers=ESC,BMS,DASH)
//   Apps_1: bit 8|16 (scale=0.1, unsigned, receivers=ESC,DASH)
//   Apps_2: bit 24|16 (scale=0.1, unsigned, receivers=ESC,DASH)
//   Brake_Pressure: bit 40|16 (scale=0.1, unsigned, receivers=ESC,DASH)

// ========== RECEIVER ROUTING (Auto-Generatable) ==========
// BMS should receive:
//   - VCU_System_Status (0x201)

// DASH should receive:
//   - VCU_System_Status (0x201)

// ESC should receive:
//   - VCU_Torque_Command (0x200)
//   - VCU_System_Status (0x201)

// VCU should receive:
//   - ESC_Motor_Status_1 (0x102)
//   - ESC_Motor_Status_2 (0x103)
//   - ESC_Control_Status (0x104)


// ========== WHAT REQUIRES MANUAL WORK ==========
// The following CANNOT be auto-generated and needs manual decisions:

// 1. STRUCTURE DEFINITIONS (Manual decision on grouping/packing):
/*
typedef struct {
    uint16_t motor_speed;        // Manual choice: uint16_t vs int16_t
    int16_t motor_torque;        // Manual choice: how to group signals
    uint8_t motor_temperature;   // Manual choice: bit fields vs separate vars
    uint8_t inverter_enable;     // Manual choice: bool vs uint8_t
} ESC_Motor_Status_1_t;
*/

// 2. DATA TYPE CHOICES (Manual engineering decisions):
/*
// DBC says: "16@1+" but doesn't specify signed/unsigned in C
// Engineer must decide: uint16_t vs int16_t vs bit fields
*/

// 3. PROGRAMMING CONVENIENCES (Manual additions):
/*
#define MOTOR_FAULT_OVERCURRENT    0x01  // Bit flag interpretation
#define MOTOR_FAULT_OVERVOLTAGE    0x02  // Not directly in DBC
#define MOTOR_FAULT_OVERTEMP       0x04  // Manual convenience
*/

// 4. API FUNCTIONS (Manual system integration):
/*
bool hasNewMotorData(void);              // System-specific
uint16_t getMotorSpeed_RPM(void);        // Convenience functions
void clearMotorNewDataFlag(void);       // State management
*/

#endif // INVERTER_TEST_AUTO_GENERATED_H
