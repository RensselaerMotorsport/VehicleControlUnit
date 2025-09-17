#include "../../Inc/Utils/Telemetry.h"
#include "../../Inc/Utils/MessageFormat.h"
#include "../../Inc/Utils/Common.h"
#include <string.h>

#define MAX_TELEMETRY_SIGNALS 100

static TelemetrySignal signals[MAX_TELEMETRY_SIGNALS];
static uint16_t num_signals = 0;
static bool telemetry_initialized = false;

void initTelemetry(void) {
    memset(signals, 0, sizeof(signals));
    num_signals = 0;
    telemetry_initialized = true;
    
    // Initialize units system
    initUnits();
    
    sendMessage("Telemetry", MSG_DEBUG, "Telemetry utility initialized");
}

TelemetrySignal* registerTelemetrySignal(const char* name, TelemetryType type, UnitId unit_id, 
                                        uint32_t expected_rate_ms, 
                                        float custom_min, float custom_max) {
    if (!telemetry_initialized || num_signals >= MAX_TELEMETRY_SIGNALS) {
        return NULL;
    }
    
    TelemetrySignal* sig = &signals[num_signals];
    const UnitDefinition* unit = getUnitDefinition(unit_id);
    
    strncpy(sig->name, name, sizeof(sig->name) - 1);
    sig->name[sizeof(sig->name) - 1] = '\0';
    sig->type = type;
    sig->unit_id = unit_id;
    sig->expected_rate_ms = expected_rate_ms;
    sig->last_update = HAL_GetTick();
    sig->enabled = true;
    
    // Use custom limits if provided, otherwise use unit defaults
    if (custom_min != custom_max) {
        sig->custom_min = custom_min;
        sig->custom_max = custom_max;
        sig->use_custom_limits = true;
    } else {
        sig->custom_min = unit->default_warning_min;
        sig->custom_max = unit->default_warning_max;
        sig->use_custom_limits = false;
    }
    
    num_signals++;
    
    sendMessage("Telemetry", MSG_DEBUG, "Registered: %s (%s) [%s] - %dms", 
               name, unit->symbol, getTypeName(type), expected_rate_ms);
    return sig;
}

void sendTelemetryValue(TelemetrySignal* signal, float value) {
    if (signal == NULL || !signal->enabled) {
        return;
    }
    
    const UnitDefinition* unit = getUnitDefinition(signal->unit_id);
    
    // Update timestamp
    signal->last_update = HAL_GetTick();
    
    // Handle different telemetry types using the generic sendMessage()
    switch (signal->type) {
        case TELEMETRY_SENSOR:
            // Validate and send sensor data
            if (!validateUnitValue(signal->unit_id, value)) {
                sendMessage(signal->name, MSG_ERROR, "Value exceeds physical limits: %.3f %s", 
                           value, unit->symbol);
                return;
            }
            
            // Check warning limits
            float min_limit = signal->use_custom_limits ? signal->custom_min : unit->default_warning_min;
            float max_limit = signal->use_custom_limits ? signal->custom_max : unit->default_warning_max;
            
            if (value < min_limit || value > max_limit) {
                sendMessage(signal->name, MSG_WARNING, "Value out of range: %.3f %s (range: %.1f-%.1f)", 
                           value, unit->symbol, min_limit, max_limit);
            }
            
            // Send using generic sendMessage - no special sendSensorValue() needed!
            sendMessage(signal->name, MSG_SENSOR_VALUE, "Value:%.3f;Unit:%s", value, unit->symbol);
            break;
            
        case TELEMETRY_OUTPUT:
            sendMessage(signal->name, MSG_OUTPUT_VALUE, "Value:%.0f;Unit:%s", value, unit->symbol);
            break;
            
        case TELEMETRY_STATUS:
            sendMessage(signal->name, MSG_SYSTEM_STATUS, "Value:%.3f;Unit:%s", value, unit->symbol);
            break;
            
        case TELEMETRY_DEBUG:
            sendMessage(signal->name, MSG_DEBUG, "Value:%.3f;Unit:%s", value, unit->symbol);
            break;
            
        case TELEMETRY_CAN_TX:
        case TELEMETRY_CAN_RX:
            // CAN data handled separately through sendCANTelemetryData()
            break;
    }
}

void checkTelemetryHealth(void) {
    uint32_t now = HAL_GetTick();
    
    for (uint16_t i = 0; i < num_signals; i++) {
        TelemetrySignal* sig = &signals[i];
        if (!sig->enabled) continue;
        
        uint32_t time_since_update = now - sig->last_update;
        uint32_t stale_threshold = sig->expected_rate_ms * 3;
        
        if (time_since_update > stale_threshold) {
            const UnitDefinition* unit = getUnitDefinition(sig->unit_id);
            sendMessage("TelemetryHealth", MSG_WARNING, 
                       "Signal stale: %s (last update %dms ago, expected every %dms)",
                       sig->name, time_since_update, sig->expected_rate_ms);
        }
    }
}

void handleTelemetryConfigRequest(void) {
    sendMessage("Telemetry", MSG_DEBUG, "Configuration requested by GUI");
    
    // Send total count first
    sendMessage("TelemetryConfig", MSG_DEBUG, "TotalSignals:%d", num_signals);
    
    // Send each signal configuration
    for (uint16_t i = 0; i < num_signals; i++) {
        TelemetrySignal* sig = &signals[i];
        const UnitDefinition* unit = getUnitDefinition(sig->unit_id);
        
        // Send comprehensive configuration
        sendMessage("TelemetryConfig", MSG_CONFIG, 
                   "Signal:%s;Unit:%s;Category:%s;Type:%d;Rate:%d;Min:%.2f;Max:%.2f;Decimals:%d;AbsMin:%.2f;AbsMax:%.2f",
                   sig->name, 
                   unit->symbol,
                   getCategoryName(unit->category),  // Helper function to get category name
                   unit->data_type,
                   sig->expected_rate_ms,
                   sig->custom_min, 
                   sig->custom_max,
                   unit->decimal_places,
                   unit->absolute_min,
                   unit->absolute_max);
    }
    
    // Send end marker
    sendMessage("TelemetryConfig", MSG_DEBUG, "ConfigComplete:1");
}

// Helper function to get category names
const char* getCategoryName(UnitCategory category) {
    switch (category) {
        case UNIT_CATEGORY_VOLTAGE: return "Voltage";
        case UNIT_CATEGORY_CURRENT: return "Current";
        case UNIT_CATEGORY_PRESSURE: return "Pressure";
        case UNIT_CATEGORY_TEMPERATURE: return "Temperature";
        case UNIT_CATEGORY_POSITION: return "Position";
        case UNIT_CATEGORY_SPEED: return "Speed";
        case UNIT_CATEGORY_TIME: return "Time";
        case UNIT_CATEGORY_FREQUENCY: return "Frequency";
        case UNIT_CATEGORY_STATUS: return "Status";
        default: return "Other";
    }
}

// Helper function to get type names
const char* getTypeName(TelemetryType type) {
    switch (type) {
        case TELEMETRY_SENSOR: return "SENSOR";
        case TELEMETRY_OUTPUT: return "OUTPUT";
        case TELEMETRY_CAN_TX: return "CAN_TX";
        case TELEMETRY_CAN_RX: return "CAN_RX";
        case TELEMETRY_STATUS: return "STATUS";
        case TELEMETRY_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

// Enhanced CAN handling
void sendCANTelemetryData(TelemetrySignal* signal, uint32_t can_id, uint8_t* data, uint8_t dlc) {
    if (signal == NULL || !signal->enabled || 
        (signal->type != TELEMETRY_CAN_TX && signal->type != TELEMETRY_CAN_RX)) {
        return;
    }
    
    // Update timestamp
    signal->last_update = HAL_GetTick();
    
    // Format data as hex string
    char hex_data[17] = {0}; // 8 bytes * 2 chars + null terminator
    for (int i = 0; i < dlc && i < 8; i++) {
        sprintf(&hex_data[i*2], "%02X", data[i]);
    }
    
    MessageType msg_type = (signal->type == TELEMETRY_CAN_TX) ? MSG_CAN_TX : MSG_CAN_RX;
    sendMessage("CAN", msg_type, "ID:0x%03X;DLC:%d;Data:%s", can_id, dlc, hex_data);
}