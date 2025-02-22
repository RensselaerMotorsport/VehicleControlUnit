#ifndef RENNSSELAERMOTORSPORT_INVERTER_H
#define RENNSSELAERMOTORSPORT_INVERTER_H

#include "../ExternalSystem.h"
#include "../Controller/TorqueControl.h"
#include "../../Utils/Constants.h"

typedef struct {
    ExternalSystem base;
    TorqueControl* tc;
    int maxCurrent;
    int maxTemp;
    int maxVoltage;
} Inverter;

extern uint32_t dac1_buffer[DAC1_BUFFER_SIZE];

/**
 * @brief Initializes the Inverter with initial settings.
 * 
 * @param inverter A pointer to the Inverter structure.
 * @param tc A pointer to the TorqueControl structure.
 * @param hz Rate at which the inverter is called (in hz).
 * @param maxCurrent The maximum current limit set for the inverter.
 * @param maxTemp The maximum temperature limit set for the inverter.
 * @param maxVoltage The maximum voltage limit set for the inverter.
 */
void initInverter(Inverter* inverter, TorqueControl* tc, int hz, int maxCurrent, int maxTemp, int maxVoltage);

/**
 * @brief Starts the Inverter.
 * 
 * @param inverter A pointer to the Inverter structure.
 * @return int _SUCCESS or _FAILURE.
 */
int startInverter(Inverter* inverter);

/**
 * @brief Updates the Inverter.
 * 
 * @param self A pointer to the Inverter structure.
 * @return int _SUCCESS or _FAILURE.
 */
int updateInverter(void* self);

/**
 * @brief Checks the heartbeat of the Inverter.
 * 
 * @param self A pointer to the Inverter structure.
 * @return int _SUCCESS or _FAILURE.
 */
int checkInverterHeartbeat(void* self);

#endif // RENNSSELAERMOTORSPORT_INVERTER_H
