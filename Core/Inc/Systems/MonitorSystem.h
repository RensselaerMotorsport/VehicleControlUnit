#ifndef RENSSELAERMOTORSPORT_MONITOR_SYSTEM_H
#define RENSSELAERMOTORSPORT_MONITOR_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"

typedef enum {
    m_TORQUE,
    m_APPS,
    m_BRAKES,
    m_TRACTION,
    m_ACCUMULATOR,
    m_RTD
} MonitorType;

typedef enum {
    NONE, // If a fault is detected, do nothing
    SYSTEM_ADJUST, // If a fault is detected, adjust the system
    SYSTEM_DISABLE, // If a fault is detected, disable the system, and adjust the vehicle
    VEHICLE_SHUTDOWN // If a fault is detected, shut down the vehicle
} FaultType;

typedef struct {
    System system;
    MonitorType type;
    FaultType fault;
    int (*runMonitor)(void* self);
} MonitorSystem;

/**
 * @brief Initializes the Monitor System with initial settings.
 *
 * @param monitor A pointer to the MonitorSystem structure.
 * @param name The name of the monitor.
 * @param hz Rate at which the monitor is called (in hz).
 * @param type The type of monitor (per MonitorType).
 * @param fault The type of fault to execute when monitor trips (per FaultType).
*/
void initMonitorSystem(MonitorSystem* monitor, const char* name, int hz,
                       MonitorType type, FaultType fault, int (*runMonitor)(void* self));

#endif // RENSSELAERMOTORSPORT_MONITOR_SYSTEM_H
