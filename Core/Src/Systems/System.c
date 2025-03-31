#include "../../Inc/Systems/System.h"

void initSystem(System* system, const char* name, int hz, SystemType type, void* child) {
    initUpdateable(&system->updateable, name, hz, SYSTEM, system);
    system->type = type;
    system->child = child;
}
