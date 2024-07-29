#ifndef RENSSELAERMOTORSPORT_EXTERNAL_SYSTEM_H
#define RENSSELAERMOTORSPORT_EXTERNAL_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"
#include "CommsSystem.h"

typedef enum {
    BMS,
    ESC,
    IMD
} ExternalType;

typedef struct {
    System system;
    ExternalType type;
    CommsSystem* comms; //What comms are we using to communicate with the external system? CAN, Ethernet, etc.
    int (*check_heartbeat)(void* self); // Check if the controller is still alive
} ExternalSystem;

/*
 * Initializes the External System with initial settings.
 *
 * @param external A pointer to the ExternalSystem structure.
 * @param name The name of the external system.
 * @param hz Rate at which the external system is called (in hz).
 * @param type The type of external system (per ExternalType).
*/
void initExternalSystem(ExternalSystem* external, const char* name, int hz,
                        ExternalType type);

#endif // RENSSELAERMOTORSPORT_EXTERNAL_SYSTEM_H
