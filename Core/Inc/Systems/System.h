#ifndef RENSSELAERMOTORSPORT_SYSTEM_H
#define RENSSELAERMOTORSPORT_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/Updateable.h"

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

/**
 * @brief Initializes a System object.
 *
 * @param system Pointer to the System to initialize.
 * @param name Name of the system.
 * @param hz Frequency in Hertz.
 * @param type Type of the system.
 */
void initSystem(System* system, const char* name, int hz, SystemType type);

#endif // RENSSELAERMOTORSPORT_SYSTEM_H
