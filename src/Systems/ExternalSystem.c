#include "../../includes/ExternalSystem.h"

/* 
 * Initializes the External System with initial settings.
 *
 * @param external A pointer to the ExternalSystem structure.
 * @param name The name of the external system.
 * @param hz Rate at which the external system is called (in hz).
 * @param type The type of external system (per ExternalType).
*/
void initExternalSystem(ExternalSystem* external, const char* name, int hz, ExternalType type) {
    initSystem(&external->system, name, hz, EXTERNAL);
    external->type = type;
    external->comms = NULL;
    external->check_heartbeat = NULL;
}