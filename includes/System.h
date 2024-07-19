#ifndef RENSSELAERMOTORSPORT_SYSTEM_H
#define RENSSELAERMOTORSPORT_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Updateable.h"

typedef enum {
    COMMS, // CAN, Ethernet, etc
    EXTERNAL, // ESC, BMS, etc
    MONITOR, // Torque Police, Traction Control, etc
    CONTROLLER // Torque Control, etc
} SystemType;

typedef struct {
    Updateable updateable;
    SystemType type;
} System;

void initSystem(System* system, const char* name, int hz, SystemType type);

#endif
