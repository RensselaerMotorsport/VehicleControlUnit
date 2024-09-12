#ifndef RENSSELAERMOTORSPORT_SHOCKPOT_H
#define RENSSELAERMOTORSPORT_SHOCKPOT_H

#include "../AnalogSensor.h"
#include "../../Utils/WheelLocation.h"

typedef struct {
    AnalogSensor base;
    float distance;
    WHEEL_LOCATION wheel_location;
} ShockPot;

/**
 * @brief Initializes the ShockPot sensor with the given frequency, channel,
 * and wheel location.
 *
 * @param shockPot Pointer to the ShockPot structure to initialize.
 * @param hz       The frequency in Hertz at which the sensor operates.
 * @param channel  The analog channel the sensor is connected to.
 * @param location The wheel location as defined by the WHEEL_LOCATION enum.
 */
void initShockPot(ShockPot* shockPot, int hz, int channel, WHEEL_LOCATION location);

/**
 * @brief Gets the current distance measured by the ShockPot.
 *
 * @param shockPot Pointer to the ShockPot structure to query.
 * @return         The current distance measured by the ShockPot.
 */
float getShockPotDistance(ShockPot* shockPot);

/**
 * @brief Updates the ShockPot data.
 *
 * @param shockPot Pointer to the ShockPot structure to update.
 */
void updateShockPot(void* shockPot);

/**
 * @brief Converts raw ShockPot data to a meaningful distance value.
 *
 * @param rawVal The raw data from the ShockPot sensor.
 * @return       The converted distance value.
 */
float transferFunctionShockPot(float rawVal);

#endif // RENSSELAERMOTORSPORT_SHOCKPOT_H
