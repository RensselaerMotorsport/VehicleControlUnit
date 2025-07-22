#include "../../Inc/Systems/ExternalSystem.h"
#include "../../Inc/Utils/Common.h"
#include "../../Inc/Utils/Updateable.h"
#include "../../Inc/Scheduler/Task.h"

void initExternalSystem(ExternalSystem* external, const char* name, int hz,
                        ExternalType type, int (*updateExternal)(ExternalSystem* external), 
                        int (*check_heartbeat)(void* self), void* child) {
    initSystem(&external->system, name, hz, EXTERNAL, external);
    external->type = type;
    external->comms = NULL;
    external->check_heartbeat = check_heartbeat;
    external->updateExternal = updateExternal;
    external->child = 

    // Set the updateable function to generic external system update
    external->system.updateable.update = e_defaultUpdate;
}

int e_defaultUpdate(Updateable* updateable) {
    // Cast the child pointer to a external system
    System* system = (System*)updateable->child;
    ExternalSystem* external = (ExternalSystem*)system->child;

    // Perform the heartbeat check
    if (external->check_heartbeat(external) == _FAILURE) {
        return _FAILURE;
    }

    // Perform the external system update
    if (external->updateExternal(external) == _FAILURE) {
        return _FAILURE;
    }

    return _SUCCESS;
}
