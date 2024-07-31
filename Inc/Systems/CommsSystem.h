#ifndef RENSSELAERMOTORSPORT_COMMS_SYSTEM_H
#define RENSSELAERMOTORSPORT_COMMS_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"

typedef enum {
    CAN,
    ETHERNET
} CommsType;

typedef struct {
    System system;
    CommsType type;
    int (*check_heartbeat)(void* self);
} CommsSystem;

/**
 * @brief Initializes the Comms System with initial settings.
 *
 * @param comms A pointer to the CommsSystem structure.
 * @param name The name of the comms system.
 * @param hz Rate at which the comms system is called (in hz).
 * @param type The type of comms system (per CommsType).
*/
void initCommsSystem(CommsSystem* comms, const char* name, int hz, CommsType type);

#endif // RENSSELAERMOTORSPORT_COMMS_SYSTEM_H
