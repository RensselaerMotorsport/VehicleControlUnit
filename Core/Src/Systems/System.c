#include "../../Inc/Systems/System.h"

void initSystem(System* system, const char* name, int hz, SystemType type) {
    initUpdateable(&system->updateable, name, hz);
    system->type = type;
}
