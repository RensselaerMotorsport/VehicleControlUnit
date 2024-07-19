#ifndef RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H
#define RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H

#include "System.h"
#include "MonitorSystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MONITORS 10

typedef enum {
    c_TORQUE
} ControllerType;

typedef struct {
    System system;
    ControllerType type;
    MonitorSystem* monitors[MAX_MONITORS]; // What monitors are associated with this controller?
    int num_monitors;
    int (*safety)(void* self); // Check if the controller is operating safely
    int (*addMonitor)(void* self, MonitorSystem* monitor); // Add a monitor to the controller
    int (*removeMonitor)(void* self, MonitorSystem* monitor); // Remove a monitor from the controller
} ControllerSystem;

void initControllerSystem(ControllerSystem* controller, const char* name, int hz, ControllerType type);
int defaultAddMonitor(void* self, MonitorSystem* monitor);
int defaultRemoveMonitor(void* self, MonitorSystem* monitor);

#endif
