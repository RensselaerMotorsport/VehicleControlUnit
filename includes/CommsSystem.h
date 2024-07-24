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

void initCommsSystem(CommsSystem* comms, const char* name, int hz, CommsType type);

#endif
