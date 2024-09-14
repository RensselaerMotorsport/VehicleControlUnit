/**
  ******************************************************************************
  * @file           : Apps.h
  * @brief          : Contains the logic for the two apps sensors.
  ******************************************************************************
*/
#ifndef RENSSELAERMOTORSPORT_APPS_H
#define RENSSELAERMOTORSPORT_APPS_H

#include "../../Sensors/AnalogSensors/App.h"

typedef struct {
    App app1;
    App app2;
} Apps;

/**
 * @brief Initializes the APPs with the given frequency and channel.
 *
 * @param apps    Pointer to the Apps structure to initialize.
 * @param hz      The frequency in Hertz at which the APP operates.
 * @param channel1 The analog channel the first APP is connected to.
 * @param channel2 The analog channel the second APP is connected to.
 */
void initApp(Apps* apps, int hz, int channel1, int channel2);

#endif // RENSSELAERMOTORSPORT_APPS_H
