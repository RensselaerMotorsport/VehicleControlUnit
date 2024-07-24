#include "../../includes/MonitorSystem.h"

/* 
 * Initializes the Monitor System with initial settings.
 *
 * @param monitor A pointer to the MonitorSystem structure.
 * @param name The name of the monitor.
 * @param hz Rate at which the monitor is called (in hz).
 * @param type The type of monitor (per MonitorType).
 * @param fault The type of fault to execute when monitor trips (per FaultType).
*/
void initMonitorSystem(MonitorSystem* monitor, const char* name, int hz, MonitorType type, FaultType fault) {
    initSystem(&monitor->system, name, hz, MONITOR);
    monitor->type = type;
    monitor->fault = fault;
}