/**
  ******************************************************************************
  * @file           : Apps.h
  * @brief          : Contains the logic for the two app sensors.
  ******************************************************************************
*/

#ifndef RENSSELAERMOTORSPORT_APPS_H
#define RENSSELAERMOTORSPORT_APPS_H

#include "../ControllerSystem.h"
#include "../../Sensors/AnalogSensors/App.h"

#define APPS_DIFFERENCE 0.1 // 10% as rules required

typedef enum {
    APPS_OK,
    APPS_FAULT,
} AppsStatus;

typedef struct {
    ControllerSystem base;
    AppsStatus status;
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
 * @param apps A pointer to the APPs structure.
 */
void updateApps(void* apps);

/**
 * @brief Averages app sensors to get pedal position.
 *
 * @param apps Pointer to the APPS controller.
 */
 float getAppsPosition(Apps* apps);

/**
 * @brief Checks the safety of Apps.
 *
 * @param apps A pointer to the APPs controller.
 */
int appsSafetyCheck(void* apps);

#endif // RENSSELAERMOTORSPORT_APPS_H
