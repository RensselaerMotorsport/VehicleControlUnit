#include "../../Inc/Systems/ExternalSystem.h"

void initExternalSystem(ExternalSystem* external, const char* name, int hz,
                        ExternalType type) {
    initSystem(&external->system, name, hz, EXTERNAL, external);
    external->type = type;
    external->comms = NULL;
    external->check_heartbeat = NULL;
}
