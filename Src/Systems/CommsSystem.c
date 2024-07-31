#include "../../Inc/Systems/CommsSystem.h"

void initCommsSystem(CommsSystem* comms, const char* name, int hz, CommsType type) {
    initSystem(&comms->system, name, hz, COMMS);
    comms->type = type;
    comms->check_heartbeat = NULL;
}
