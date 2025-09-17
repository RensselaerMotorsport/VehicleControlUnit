#ifndef RENSSELAERMOTORSPORT_TELEMETRY_H
#define RENSSELAERMOTORSPORT_TELEMETRY_H

#include <stdint.h>
#include <stdbool.h>
#include "Units.h"

typedef enum {
    TELEMETRY_SENSOR,     // Traditional sensor readings
    TELEMETRY_OUTPUT,     // Digital/analog outputs  
    TELEMETRY_CAN_TX,     // CAN messages being sent
    TELEMETRY_CAN_RX,     // CAN messages being received
    TELEMETRY_STATUS,     // System status values
    TELEMETRY_DEBUG       // Debug/diagnostic values
} TelemetryType;

typedef struct {
    char name[32];
    TelemetryType type;           // NEW: What kind of data this is
    UnitId unit_id;               // Use unit ID instead of string
    uint32_t expected_rate_ms;
    uint32_t last_update;
    float custom_min;             // Custom limits (overrides unit defaults)
    float custom_max;
    bool use_custom_limits;       // Whether to use custom or unit default limits
    bool enabled;
    
    // Type-specific data
    union {
        struct {
            // For CAN messages
            uint32_t can_id;
            uint8_t dlc;
        } can_info;
        
        struct {
            // For outputs
            bool is_pwm;          // true for PWM outputs, false for digital
            uint16_t pwm_frequency;
        } output_info;
    };
} TelemetrySignal;

// Initialize telemetry utility
void initTelemetry(void);

// Register different types of telemetry signals
TelemetrySignal* registerTelemetrySignal(const char* name, TelemetryType type, UnitId unit_id, 
                                        uint32_t expected_rate_ms, 
                                        float custom_min, float custom_max);

// Universal telemetry send - handles all types!
void sendTelemetryValue(TelemetrySignal* signal, float value);
void sendCANTelemetryData(TelemetrySignal* signal, uint32_t can_id, uint8_t* data, uint8_t dlc);

// Fallback send by name (avoid if possible)
void sendTelemetryValueByName(const char* name, float value, TelemetryType type);

// Send configuration and health functions
void checkTelemetryHealth(void);
void handleTelemetryConfigRequest(void);
const char* getCategoryName(UnitCategory category);
const char* getTypeName(TelemetryType type);

#endif // RENSSELAERMOTORSPORT_TELEMETRY_H