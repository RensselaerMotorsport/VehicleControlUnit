#ifndef RENSSELAERMOTORSPORT_EXTERNAL_SYSTEM_H
#define RENSSELAERMOTORSPORT_EXTERNAL_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"
#include "CommsSystem.h"
#include "../Scheduler/Task.h"

typedef enum {
    BMS,
    e_INVERTER,
    IMD
} ExternalType;

// Need to do this because of circular dependency
typedef struct ExternalSystem ExternalSystem;
struct ExternalSystem {
    System system;
    ExternalType type;
    CommsSystem* comms; //What comms are we using to communicate with the external system? CAN, Ethernet, etc.
    int (*updateExternal)(struct ExternalSystem* external); // Update the external system
    int (*check_heartbeat)(void* self); // Check if the controller is still alive
    void* child; // Pointer to the child struct
};

/**
 * Initializes the External System with initial settings.
 *
 * @param external A pointer to the ExternalSystem structure.
 * @param name The name of the external system.
 * @param hz Rate at which the external system is called (in hz).
 * @param type The type of external system (per ExternalType).
 * @param updateExternal The function to update the external system.
 * @param check_heartbeat The function to check the heartbeat of the external system.
 * @param child Pointer to the child struct.
*/
void initExternalSystem(ExternalSystem* external, const char* name, int hz,
                        ExternalType type, int (*updateExternal)(ExternalSystem* external),
                        int (*check_heartbeat)(void* self), void* child);

/**
 * Default update function for ExternalSystem objects.
 * 
 * @param task Pointer to the task to update.
 * @return int _SUCCESS or _FAILURE.
 */
int e_defaultUpdate(Updateable* updateable);
#endif // RENSSELAERMOTORSPORT_EXTERNAL_SYSTEM_H
