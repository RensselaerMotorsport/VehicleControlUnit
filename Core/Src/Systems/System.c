#include "../../Inc/Systems/System.h"

void initSystem(System* system, const char* name, int hz, SystemType type, void* child) {
    initUpdateable(system, name, hz, SYSTEM);
    system->type = type;
    system->child = child;
}
