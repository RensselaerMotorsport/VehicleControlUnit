#include "../../Inc/Systems/MonitorSystem.h"

void initMonitorSystem(MonitorSystem* monitor, const char* name, int hz,
                       MonitorType type, FaultType fault, int (*runMonitor)(void* self)) {
    initSystem(&monitor->system, name, hz, MONITOR, monitor);
    monitor->type = type;
    monitor->fault = fault;
    monitor->runMonitor = runMonitor;
}