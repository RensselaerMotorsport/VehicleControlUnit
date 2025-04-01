#ifndef __RENNSSELAERMOTORSPORT_RTD_H
#define __RENNSSELAERMOTORSPORT_RTD_H

#include "Apps.h"
#include "BrakeSystemControl.h"
#include "../ControllerSystem.h"
#include "../../Outputs/DigitalOutput.h"
#include "../../Sensors/DigitalSensors/Button.h"
#include "../../Utils/Common.h"

typedef struct {
    ControllerSystem base;
    uint8_t readyToDrive;
    uint8_t precharged;
    uint8_t counter;
    Button* button;
    DigitalOutput* piezo;
    Apps* apps;
    BrakeSystemControl* bsc;
} RTD;

/**
 * @brief Initializes the RTD Actuator with initial settings.
 *
 * @param rtd A pointer to the RTD structure.
 * @param hz Rate at which the sensor is called (in hz).
 */
void initRTD(RTD* rtd, Apps* apps, BrakeSystemControl* bsc, int hz, int buttonPort, int piezoPort);

/**
 * @brief Starts the RTD Actuator.
 *
 * @param rtd A pointer to the RTD structure.
 * @return _SUCCESS if the actuator was started, _FAILURE otherwise.
 */
int startRTD(RTD* rtd);

/**
 * @brief Updates the RTD Actuator.
 *
 * @param controller A pointer to the RTD ControllerSystem.
 * @return _SUCCESS if the actuator was updated, _FAILURE otherwise.
 */
int updateRTD(ControllerSystem* controller);

/**
 * @brief Checks if RTD is in ready to drive state.
 * 
 * @param rtd A pointer to the RTD structure.
 * @return _SUCCESS if the RTD is ready to drive, _FAILURE otherwise.
 */
int checkRTD(void* rtd);
#endif // __RENNSSELAERMOTORSPORT_RTD_H