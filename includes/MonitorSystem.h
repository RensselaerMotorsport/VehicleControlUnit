#ifndef RENSSELAERMOTORSPORT_MONITOR_SYSTEM_H
#define RENSSELAERMOTORSPORT_MONITOR_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"

typedef enum {
    m_TORQUE,
    m_TRACTION,
    m_ACCUMULATOR
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
} MonitorSystem;

void initMonitorSystem(MonitorSystem* monitor, const char* name, int hz, MonitorType type, FaultType fault);

#endif
