#include "../../Inc/Systems/MonitorSystem.h"

void initMonitorSystem(MonitorSystem* monitor, const char* name, int hz,
                       MonitorType type, FaultType fault) {
    initSystem(&monitor->system, name, hz, MONITOR);
    monitor->type = type;
    monitor->fault = fault;
}
