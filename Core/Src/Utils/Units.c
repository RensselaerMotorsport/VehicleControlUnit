#include "../../Inc/Utils/Units.h"
#include "../../Inc/Utils/MessageFormat.h"
#include <string.h>
#include <math.h>

static const UnitDefinition unit_definitions[UNIT_COUNT] = {
    // Voltage units
    [UNIT_VOLTS] = {"Volts", "Volts", UNIT_TYPE_FLOAT, UNIT_CATEGORY_VOLTAGE, 
                    -1000.0f, 1000.0f, -50.0f, 50.0f, 3, 1.0f},
    [UNIT_MILLIVOLTS] = {"Millivolts", "Millivolts", UNIT_TYPE_FLOAT, UNIT_CATEGORY_VOLTAGE, 
                         -1000000.0f, 1000000.0f, -50000.0f, 50000.0f, 1, 0.001f},
    
    // Current units
    [UNIT_AMPERES] = {"Amperes", "Amperes", UNIT_TYPE_FLOAT, UNIT_CATEGORY_CURRENT, 
                      -1000.0f, 1000.0f, -100.0f, 100.0f, 3, 1.0f},
    [UNIT_MILLIAMPERES] = {"Milliamperes", "Milliamperes", UNIT_TYPE_FLOAT, UNIT_CATEGORY_CURRENT, 
                           -1000000.0f, 1000000.0f, -100000.0f, 100000.0f, 1, 0.001f},
    
    // Pressure units
    [UNIT_PSI] = {"PSI", "PSI", UNIT_TYPE_FLOAT, UNIT_CATEGORY_PRESSURE, 
                  -1.0f, 10000.0f, 0.0f, 3000.0f, 1, 6894.76f},  // Scale to Pascal
    [UNIT_BAR] = {"bar", "Bar", UNIT_TYPE_FLOAT, UNIT_CATEGORY_PRESSURE, 
                  -1.0f, 1000.0f, 0.0f, 300.0f, 2, 100000.0f},   // Scale to Pascal
    [UNIT_PASCAL] = {"Pa", "Pascal", UNIT_TYPE_FLOAT, UNIT_CATEGORY_PRESSURE, 
                     -1.0f, 100000000.0f, 0.0f, 30000000.0f, 0, 1.0f},
    
    // Temperature units
    [UNIT_CELSIUS] = {"°C", "Celsius", UNIT_TYPE_FLOAT, UNIT_CATEGORY_TEMPERATURE, 
                      -273.15f, 3000.0f, -50.0f, 200.0f, 1, 1.0f},
    [UNIT_FAHRENHEIT] = {"°F", "Fahrenheit", UNIT_TYPE_FLOAT, UNIT_CATEGORY_TEMPERATURE, 
                         -459.67f, 5432.0f, -58.0f, 392.0f, 1, 1.0f},
    [UNIT_KELVIN] = {"K", "Kelvin", UNIT_TYPE_FLOAT, UNIT_CATEGORY_TEMPERATURE, 
                     0.0f, 3273.15f, 223.15f, 473.15f, 2, 1.0f},
    
    // Position/angle units
    [UNIT_PERCENT] = {"%", "Percent", UNIT_TYPE_FLOAT, UNIT_CATEGORY_POSITION, 
                      -1000.0f, 1000.0f, 0.0f, 100.0f, 1, 1.0f},
    [UNIT_DEGREES] = {"°", "Degrees", UNIT_TYPE_FLOAT, UNIT_CATEGORY_POSITION, 
                      -3600.0f, 3600.0f, -180.0f, 180.0f, 1, 0.017453f},  // Scale to radians
    [UNIT_RADIANS] = {"rad", "Radians", UNIT_TYPE_FLOAT, UNIT_CATEGORY_POSITION, 
                      -62.83f, 62.83f, -3.14159f, 3.14159f, 3, 1.0f},
    
    // Speed units
    [UNIT_RPM] = {"RPM", "RPM", UNIT_TYPE_FLOAT, UNIT_CATEGORY_SPEED, 
                  0.0f, 50000.0f, 0.0f, 10000.0f, 0, 0.10472f},  // Scale to rad/s
    [UNIT_HZ] = {"Hz", "Hertz", UNIT_TYPE_FLOAT, UNIT_CATEGORY_FREQUENCY, 
                 0.0f, 1000000.0f, 0.0f, 10000.0f, 2, 1.0f},
    [UNIT_KMH] = {"km/h", "KMH", UNIT_TYPE_FLOAT, UNIT_CATEGORY_SPEED, 
                  0.0f, 1000.0f, 0.0f, 300.0f, 1, 0.27778f},     // Scale to m/s
    [UNIT_MPH] = {"mph", "MPH", UNIT_TYPE_FLOAT, UNIT_CATEGORY_SPEED, 
                  0.0f, 600.0f, 0.0f, 200.0f, 1, 0.44704f},      // Scale to m/s
    
    // Time units
    [UNIT_SECONDS] = {"s", "Seconds", UNIT_TYPE_FLOAT, UNIT_CATEGORY_TIME, 
                      0.0f, 86400.0f, 0.0f, 3600.0f, 3, 1.0f},
    [UNIT_MILLISECONDS] = {"ms", "Milliseconds", UNIT_TYPE_UINT32, UNIT_CATEGORY_TIME, 
                           0.0f, 86400000.0f, 0.0f, 10000.0f, 0, 0.001f},
    [UNIT_MICROSECONDS] = {"μs", "Microseconds", UNIT_TYPE_UINT32, UNIT_CATEGORY_TIME, 
                           0.0f, 86400000000.0f, 0.0f, 10000000.0f, 0, 0.000001f},
    
    // Status units
    [UNIT_BOOLEAN] = {"bool", "Boolean", UNIT_TYPE_BOOL, UNIT_CATEGORY_STATUS, 
                      0.0f, 1.0f, 0.0f, 1.0f, 0, 1.0f},
    [UNIT_ENUM] = {"enum", "Enum", UNIT_TYPE_UINT16, UNIT_CATEGORY_STATUS, 
                   0.0f, 65535.0f, 0.0f, 100.0f, 0, 1.0f},
    
    // Additional units
    [UNIT_PERCENTAGE] = {"%", "Percentage", UNIT_TYPE_FLOAT, UNIT_CATEGORY_DIMENSIONLESS, 
                         0.0f, 100.0f, 0.0f, 100.0f, 1, 1.0f},
    [UNIT_DIMENSIONLESS] = {"", "Dimensionless", UNIT_TYPE_FLOAT, UNIT_CATEGORY_DIMENSIONLESS, 
                            -1000000.0f, 1000000.0f, -1000.0f, 1000.0f, 3, 1.0f}
};

void initUnits(void) {
    sendMessage("Units", MSG_DEBUG, "Units system initialized with %d unit types", UNIT_COUNT);
}

const UnitDefinition* getUnitDefinition(UnitId unit_id) {
    if (unit_id >= UNIT_COUNT) {
        return &unit_definitions[UNIT_DIMENSIONLESS];  // Fallback
    }
    return &unit_definitions[unit_id];
}

UnitId getUnitIdBySymbol(const char* symbol) {
    for (UnitId i = 0; i < UNIT_COUNT; i++) {
        if (strcmp(unit_definitions[i].symbol, symbol) == 0) {
            return i;
        }
    }
    return UNIT_DIMENSIONLESS;  // Fallback
}

bool validateUnitValue(UnitId unit_id, float value) {
    const UnitDefinition* unit = getUnitDefinition(unit_id);
    return (value >= unit->absolute_min && value <= unit->absolute_max);
}

float convertUnits(float value, UnitId from_unit, UnitId to_unit) {
    const UnitDefinition* from_def = getUnitDefinition(from_unit);
    const UnitDefinition* to_def = getUnitDefinition(to_unit);
    
    // Only convert within same category
    if (from_def->category != to_def->category) {
        return value;  // No conversion possible
    }
    
    // Convert to base unit, then to target unit
    float base_value = value * from_def->scale_factor;
    return base_value / to_def->scale_factor;
}