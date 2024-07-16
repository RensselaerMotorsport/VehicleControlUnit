#include "../../includes/CommsSystem.h"

/* 
 * Initializes the Comms System with initial settings.
 *
 * @param comms A pointer to the CommsSystem structure.
 * @param name The name of the comms system.
 * @param hz Rate at which the comms system is called (in hz).
 * @param type The type of comms system (per CommsType).
*/
void initCommsSystem(CommsSystem* comms, const char* name, int hz, CommsType type) {
    initSystem(&comms->system, name, hz, COMMS);
    comms->type = type;
    comms->check_heartbeat = NULL;
}