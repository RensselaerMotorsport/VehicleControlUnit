#ifndef RENSSELAERMOTORSPORT_APP_H
#define RENSSELAERMOTORSPORT_APP_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    float position;
} App;

/**
 * @brief Initializes the APP with the given frequency and channel.
 *
 * @param app     Pointer to the App structure to initialize.
 * @param hz      The frequency in Hertz at which the APP operates.
 * @param channel The analog channel the APP is connected to.
 */
void initApp(App* app, int hz, int channel);

/**
 * @brief Gets the current position of the APP.
 *
 * @param app Pointer to the App structure to query.
 * @return    The current position of the APP.
 */
float getAppPosition(App* app);

/**
 * @brief Updates the APP data.
 *
 * @param app Pointer to the App structure to update.
 */
void updateApp(void* app);

/**
 * @brief Retrieves updateable portion of APP system
 *
 * @param apps A pointer to the APP structure.
 * @return Pointer to updateable structure
 */
Updateable* GetUpdateableApp(App* app);

/**
 * @brief Converts raw APP data to a meaningful position value.
 *
 * @param rawVal The raw data from the APP.
 * @return       The converted APP position value.
 */
float transferFunctionApp(float rawVal);

/**
 * @brief Sets pedal position in app sensor.
 * @note For testing only
 *
 * @param app Pointer to app sensor.
 * @param pos Position to set on sensor.
 */
void setAppPos(App* app, float pos);

#endif // RENSSELAERMOTORSPORT_APP_H
