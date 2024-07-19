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

void initExternalSystem(ExternalSystem* external, const char* name, int hz, ExternalType type);

#endif
