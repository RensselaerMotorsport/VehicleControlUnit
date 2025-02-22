#ifndef __RENNSSELAERMOTORSPORT_RTD_MONITOR_H
#define __RENNSSELAERMOTORSPORT_RTD_MONITOR_H

#include "../MonitorSystem.h"
#include "../Controller/RTD.h"

typedef struct {
    MonitorSystem base;
    RTD* rtd;
} RTDMonitor;

/**
 * @brief Initializes the RTD Monitor with initial settings.
 * 
 * @param rtdm A pointer to the RTDMonitor structure.
 * @param rtd A pointer to the RTD structure.
 * @param hz Rate at which the sensor is called (in hz).
 * @return void
 */
void initRTDMonitor(RTDMonitor* rtdm, RTD* rtd, int hz);

/**
 * @brief Starts the RTD Monitor.
 * 
 * @param rtdm A pointer to the RTDMonitor structure.
 * @return _SUCCESS if the monitor was started, _FAILURE otherwise.
 */
int startRTDMonitor(RTDMonitor* rtdm);

/**
 * @brief Checks the RTD Monitor.
 * 
 * @param rtdm A pointer to the RTDMonitor structure.
 * @return _SUCCESS if the monitor was checked, _FAILURE otherwise.
 */
int checkRTDMonitor(void* rtdm);

#endif // __RENNSSELAERMOTORSPORT_RTD_MONITOR_H