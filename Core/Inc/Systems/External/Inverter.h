#ifndef RENSSELAERMOTORSPORT_INVERTER_H
#define RENSSELAERMOTORSPORT_INVERTER_H

#include "../ExternalSystem.h"
#include "../../Utils/Telemetry.h"
#include <stdint.h>

// Inverter states from M170 message
typedef enum {
    INV_STATE_POWER_UP = 0,
    INV_STATE_STOP = 1,
    INV_STATE_OPEN_LOOP = 2,
    INV_STATE_CLOSED_LOOP = 3,
    INV_STATE_IDLE_RUN = 8,
    INV_STATE_IDLE_STOP = 9
} InverterState;

// VSM states from M170 message
typedef enum {
    VSM_STATE_START = 0,
    VSM_STATE_PRECHARGE_INIT = 1,
    VSM_STATE_PRECHARGE_ACTIVE = 2,
    VSM_STATE_PRECHARGE_COMPLETE = 3,
    VSM_STATE_WAIT = 4,
    VSM_STATE_READY = 5,
    VSM_STATE_MOTOR_RUNNING = 6,
    VSM_STATE_FAULT = 7,
    VSM_STATE_SHUTDOWN = 14,
    VSM_STATE_RESET = 15
} VSMState;

// Discharge states
typedef enum {
    DISCHARGE_DISABLED = 0,
    DISCHARGE_ENABLED = 1,
    DISCHARGE_SPEED_CHECK = 2,
    DISCHARGE_ACTIVE = 3,
    DISCHARGE_COMPLETE = 4
} DischargeState;

// Inverter RX data structure (from inverter messages)
typedef struct {
    // M172 - Torque and Timer Info
    int16_t commanded_torque_raw;               // 0.1 Nm scale
    int16_t torque_feedback_raw;                // 0.1 Nm scale
    uint32_t power_on_timer;                    // 0.003 sec
    
    // M170 - Internal States
    InverterState inverter_state;
    VSMState vsm_state;
    uint8_t pwm_frequency;                      // kHz
    uint8_t rolling_counter;
    
    // Flags from M170
    uint8_t inverter_enabled;                   // 1 bit
    uint8_t direction_command;                  // 1 bit (0=Reverse, 1=Forward)
    uint8_t run_mode;                           // 0=Torque, 1=Speed
    uint8_t command_mode;                       // 0=CAN, 1=VSM
    uint8_t enable_lockout;                     // Lockout active
    uint8_t start_mode_active;                  // Key switch mode indicator
    
    // Relay status
    uint8_t relay_1_status;
    uint8_t relay_2_status;
    uint8_t relay_3_status;
    uint8_t relay_4_status;
    uint8_t relay_5_status;
    uint8_t relay_6_status;
    
    // Discharge and limiting info
    DischargeState discharge_state;
    uint8_t bms_active;
    uint8_t bms_torque_limiting;
    uint8_t max_speed_limiting;
    uint8_t low_speed_limiting;
    
    // M171 - Fault Codes
    uint16_t run_fault_lo;
    uint16_t run_fault_hi;
    uint16_t post_fault_lo;
    uint16_t post_fault_hi;
    
    // M176 - Fast Info (5ms)
    int16_t fast_torque_command_raw;            // 0.1 Nm scale
    int16_t fast_torque_feedback_raw;           // 0.1 Nm scale
    int16_t fast_motor_speed_raw;               // 1 rpm scale
    int16_t fast_dc_bus_voltage_raw;            // 0.1 V scale
    
    // M166 - Current Info (10ms)
    int16_t phase_a_current_raw;                // 0.1 A scale
    int16_t phase_b_current_raw;                // 0.1 A scale
    int16_t phase_c_current_raw;                // 0.1 A scale
    int16_t dc_bus_current_raw;                 // 0.1 A scale
    
    // M167 - Voltage Info (10ms)
    int16_t dc_bus_voltage_raw;                 // 0.1 V scale
    int16_t output_voltage_raw;                 // 0.1 V scale
    int16_t vab_Vd_voltage_raw;                 // 0.1 V scale
    int16_t vbc_Vq_voltage_raw;                 // 0.1 V scale
    
    // M165 - Motor Position Info (10ms)
    int16_t motor_speed_raw;                    // 1 rpm scale
    uint16_t motor_angle_electrical_raw;        // 0.1 deg scale
    int16_t electrical_output_frequency_raw;    // 0.1 Hz scale
    int16_t delta_resolver_filtered_raw;        // 0.1 deg scale
    
    // M168 - Flux and current info (10ms)
    int16_t flux_command_raw;                   // 0.001 Wb scale
    int16_t flux_feedback_raw;                  // 0.001 Wb scale
    int16_t id_raw;                             // 0.1 A scale
    int16_t iq_raw;                             // 0.1 A scale
    
    // M160-M162 - Temperature data (100ms)
    int16_t module_a_temp_raw;                  // 0.1 C scale
    int16_t module_b_temp_raw;                  // 0.1 C scale
    int16_t module_c_temp_raw;                  // 0.1 C scale
    int16_t gate_driver_board_temp_raw;         // 0.1 C scale
    int16_t control_board_temp_raw;             // 0.1 C scale
    int16_t rtd1_temp_raw;                      // 0.1 C scale
    int16_t rtd2_temp_raw;                      // 0.1 C scale
    int16_t rtd3_temp_raw;                      // 0.1 C scale
    int16_t rtd4_temp_raw;                      // 0.1 C scale
    int16_t rtd5_temp_raw;                      // 0.1 C scale
    int16_t motor_temp_raw;                     // 0.1 C scale
    
    // M163 - Analog inputs (100ms, 0-10V)
    uint16_t analog_input_1_raw;                // 0.01 V scale
    uint16_t analog_input_2_raw;                // 0.01 V scale
    uint16_t analog_input_3_raw;                // 0.01 V scale
    uint16_t analog_input_4_raw;                // 0.01 V scale
    uint16_t analog_input_5_raw;                // 0.01 V scale
    uint16_t analog_input_6_raw;                // 0.01 V scale
    
    // M164 - Digital inputs
    uint8_t digital_input_1;
    uint8_t digital_input_2;
    uint8_t digital_input_3;
    uint8_t digital_input_4;
    uint8_t digital_input_5;
    uint8_t digital_input_6;
    uint8_t digital_input_7;
    uint8_t digital_input_8;
    
    // M169 - Reference voltages (100ms)
    int16_t ref_voltage_1_5_raw;                // 0.01 V scale
    int16_t ref_voltage_2_5_raw;                // 0.01 V scale
    int16_t ref_voltage_5_0_raw;                // 0.01 V scale
    int16_t ref_voltage_12_0_raw;               // 0.01 V scale
    
    // M173 - Modulation and Flux
    int16_t modulation_index_raw;               // 0.0001 scale
    int16_t flux_weakening_output_raw;          // 0.1 A scale
    int16_t id_command_raw;                     // 0.1 A scale
    int16_t iq_command_raw;                     // 0.1 A scale
    
    // Firmware info
    uint16_t sw_version;
    uint16_t project_code_eep_ver;
    uint16_t date_code_mmdd;
    uint16_t date_code_yyyy;
    
    // Parameter response
    uint16_t param_address_response;
    int16_t data_response;
    uint8_t write_success;
    
} InverterRXData;

// Inverter TX command structure (to inverter)
typedef struct {
    int16_t torque_command_raw;                 // 0.1 Nm scale
    int16_t speed_command_raw;                  // 1 rpm scale
    int16_t torque_limit_command_raw;           // 0.1 Nm scale
    uint8_t direction_command;                  // 0=Reverse, 1=Forward
    uint8_t speed_mode_enable;                  // 0=Torque mode, 1=Speed mode
    uint8_t inverter_enable;                    // 0=Disabled, 1=Enabled
    uint8_t inverter_discharge;                 // 0=Normal, 1=Discharge
    uint8_t rolling_counter;                    // 4 bits, increment each message
} InverterTXCommand;

// Forward declarations
typedef struct CANCommsSystem CANCommsSystem;

// Main Inverter structure
typedef struct {
    ExternalSystem base;
    InverterRXData rx_data;
    InverterTXCommand tx_command;
    
    // Heartbeat tracking
    uint32_t last_message_timestamp;
    uint32_t heartbeat_timeout_ms;              // Timeout threshold
    uint8_t heartbeat_active;
    
    // CAN system reference for sending messages
    CANCommsSystem* can_comms;
    
    // CAN bus reference
    uint8_t can_bus;                            // CAN_1 or CAN_2
    
    // Update frequency (Hz)
    int update_hz;
    
    // Telemetry signals
    TelemetrySignal* telem_torque_feedback;
    TelemetrySignal* telem_motor_speed;
    TelemetrySignal* telem_dc_bus_voltage;
    TelemetrySignal* telem_dc_bus_current;
    TelemetrySignal* telem_motor_temp;
    TelemetrySignal* telem_inverter_state;
    TelemetrySignal* telem_vsm_state;
    TelemetrySignal* telem_torque_command;
    
} Inverter;

/**
 * Initialize the Inverter external system
 * @param inverter Pointer to the Inverter structure
 * @param can_comms Pointer to the CANCommsSystem for sending messages
 * @param hz Update frequency in Hz
 * @param can_bus CAN bus to use (CAN_1 or CAN_2)
 * @param heartbeat_timeout_ms Timeout in ms for heartbeat check
 */
void initInverter(Inverter* inverter, CANCommsSystem* can_comms, int hz, uint8_t can_bus, uint32_t heartbeat_timeout_ms);

/**
 * Update the inverter system - process RX data, check heartbeat, send TX commands
 */
int updateInverter(ExternalSystem* ext);

/**
 * Check inverter heartbeat
 */
int inverterCheckHeartbeat(void* inverter);

/**
 * Send torque command to inverter
 */
void inverterSendTorqueCommand(Inverter* inv, float torque_nm, uint8_t enable);

/**
 * Send speed command to inverter (speed mode)
 */
void inverterSendSpeedCommand(Inverter* inv, int16_t speed_rpm, uint8_t enable);

/**
 * Set direction (forward/reverse)
 */
void inverterSetDirection(Inverter* inv, uint8_t forward);

/**
 * Enable/disable inverter
 */
void inverterSetEnable(Inverter* inv, uint8_t enabled);

/**
 * Send parameter read/write command to inverter
 */
void inverterSendParamCommand(Inverter* inv, uint16_t param_addr, int16_t data_value, uint8_t read_write);

/**
 * Set torque limit for inverter
 */
void inverterSetTorqueLimit(Inverter* inv, float torque_limit_nm);

/**
 * Get physical values from raw data
 */
float inverterGetTorqueFeedback(Inverter* inv);
float inverterGetMotorSpeed(Inverter* inv);
float inverterGetDCBusVoltage(Inverter* inv);
float inverterGetDCBusCurrent(Inverter* inv);
float inverterGetMotorTemperature(Inverter* inv);
float inverterGetModuleATemp(Inverter* inv);
float inverterGetModuleBTemp(Inverter* inv);
float inverterGetModuleCTemp(Inverter* inv);

/**
 * Get fault status
 */
uint8_t inverterHasRunFault(Inverter* inv);
uint8_t inverterHasPostFault(Inverter* inv);

/**
 * Get inverter state
 */
InverterState inverterGetState(Inverter* inv);
VSMState inverterGetVSMState(Inverter* inv);

#endif // RENSSELAERMOTORSPORT_INVERTER_H
