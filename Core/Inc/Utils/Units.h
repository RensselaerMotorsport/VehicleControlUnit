#ifndef RENSSELAERMOTORSPORT_UNITS_H
#define RENSSELAERMOTORSPORT_UNITS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    UNIT_TYPE_FLOAT,
    UNIT_TYPE_INT16,
    UNIT_TYPE_INT32,
    UNIT_TYPE_UINT16,
    UNIT_TYPE_UINT32,
    UNIT_TYPE_BOOL,
    UNIT_TYPE_ENUM
} UnitDataType;

typedef enum {
    UNIT_CATEGORY_VOLTAGE,
    UNIT_CATEGORY_CURRENT,
    UNIT_CATEGORY_PRESSURE,
    UNIT_CATEGORY_TEMPERATURE,
    UNIT_CATEGORY_POSITION,
    UNIT_CATEGORY_SPEED,
    UNIT_CATEGORY_TIME,
    UNIT_CATEGORY_FREQUENCY,
    UNIT_CATEGORY_DIMENSIONLESS,
    UNIT_CATEGORY_STATUS
} UnitCategory;

typedef struct {
    char symbol[8];           // "V", "PSI", "°C", etc.
    char name[16];            // "Volts", "PSI", "Celsius"
    UnitDataType data_type;   // Expected data type
    UnitCategory category;    // Unit category
    float absolute_min;       // Physical minimum possible value
    float absolute_max;       // Physical maximum possible value
    float default_warning_min;
    float default_warning_max;
    uint8_t decimal_places;   // Suggested decimal places for display
    float scale_factor;       // For unit conversions (base unit multiplier)
} UnitDefinition;

// Predefined unit IDs
typedef enum {
    UNIT_VOLTS = 0,
    UNIT_MILLIVOLTS,
    UNIT_AMPERES,
    UNIT_MILLIAMPERES,
    UNIT_PSI,
    UNIT_BAR,
    UNIT_PASCAL,
    UNIT_CELSIUS,
    UNIT_FAHRENHEIT,
    UNIT_KELVIN,
    UNIT_PERCENT,
    UNIT_DEGREES,
    UNIT_RADIANS,
    UNIT_RPM,
    UNIT_HZ,
    UNIT_KMH,
    UNIT_MPH,
    UNIT_MS,
    UNIT_SECONDS,
    UNIT_MILLISECONDS,
    UNIT_MICROSECONDS,
    UNIT_BOOLEAN,
    UNIT_ENUM,
    UNIT_PERCENTAGE,
    UNIT_DIMENSIONLESS,
    UNIT_COUNT  // Must be last
} UnitId;

// Get unit definition by ID
const UnitDefinition* getUnitDefinition(UnitId unit_id);

// Get unit ID by symbol
UnitId getUnitIdBySymbol(const char* symbol);

// Validate value against unit's absolute limits
bool validateUnitValue(UnitId unit_id, float value);

// Convert between compatible units
float convertUnits(float value, UnitId from_unit, UnitId to_unit);

// Initialize units system
void initUnits(void);

#endif // RENSSELAERMOTORSPORT_UNITS_H