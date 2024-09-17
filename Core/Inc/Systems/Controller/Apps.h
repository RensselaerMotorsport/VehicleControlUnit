/**
  ******************************************************************************
  * @file           : Apps.h
  * @brief          : Contains the logic for the two app sensors.
  ******************************************************************************
*/
#ifndef RENSSELAERMOTORSPORT_APPS_H
#define RENSSELAERMOTORSPORT_APPS_H

#include "../../Sensors/AnalogSensors/App.h"

typedef enum {
    APPS_OK,
    APPS_FAULT,
} AppsStatus;

typedef struct {
    App* app[2];
} Apps;

/**
 * @brief Initializes the APPs with the given frequency and channel.
 *
 * @param apps    Pointer to the Apps structure to initialize.
 * @param hz      The frequency in Hertz at which the APP operates.
 * @param channel1 The analog channel the first APP is connected to.
 * @param channel2 The analog channel the second APP is connected to.
 */
void initApps(Apps* apps, int hz, int channel1, int channel2);

/**
 * @brief Updates the APPS based on both sensor outputs.
 *
 * @param apps A pointer to the TorqueControl structure.
 */
void updateApps(void* apps);

#endif // RENSSELAERMOTORSPORT_APPS_H
