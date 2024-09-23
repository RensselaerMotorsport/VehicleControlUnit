#include "../../Inc/Systems/ControllerSystem.h"
#include "../../Inc/Utils/Common.h"

void initControllerSystem(ControllerSystem* controller, const char* name, int hz,
                          ControllerType type) {
    initSystem(&controller->system, name, hz, CONTROLLER);
    controller->type = type;
    controller->num_monitors = 0;
    controller->safety = NULL;
}

int defaultAddMonitor(void* self, MonitorSystem* monitor) {
    ControllerSystem* controller = (ControllerSystem*)self;
    if (controller->num_monitors >= MAX_MONITORS) {
        printf("Cannot add more monitors to the controller\n");
        return FAILURE;
    }
    controller->monitors[controller->num_monitors++] = monitor;
    return SUCCESS;
}

int defaultRemoveMonitor(void* self, MonitorSystem* monitor) {
    ControllerSystem* controller = (ControllerSystem*)self;
    for (int i = 0; i < controller->num_monitors; i++) {
        if (controller->monitors[i] == monitor) {
            for (int j = i; j < controller->num_monitors - 1; j++) {
                controller->monitors[j] = controller->monitors[j + 1];
            }
            controller->num_monitors--;
            return SUCCESS;
        }
    }
    printf("Monitor not found in the controller\n");
    return FAILURE;
}
