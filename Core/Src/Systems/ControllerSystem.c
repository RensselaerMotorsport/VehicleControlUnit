#include "../../Inc/Systems/ControllerSystem.h"
#include "../../Inc/Utils/Common.h"
#include "../../Inc/Utils/Updateable.h"
#include "../../Inc/Scheduler/Task.h"

void initControllerSystem(ControllerSystem* controller, const char* name, int hz,
                          ControllerType type, int (*updateController)(void* self)) {
    initSystem(&controller->system, name, hz, CONTROLLER, controller);
    controller->type = type;
    controller->num_monitors = 0;
    controller->safety = c_defaultSafety;
    controller->addMonitor = c_defaultAddMonitor;
    controller->removeMonitor = c_defaultRemoveMonitor;
    controller->state = c_computed;
    controller->updateController = updateController;

    // Set the updateable function to generic controller update/safety
    controller->system.updateable.update = c_defaultUpdate;
    
}

int c_defaultAddMonitor(void* self, MonitorSystem* monitor) {
    ControllerSystem* controller = (ControllerSystem*)self;
    if (controller->num_monitors >= MAX_MONITORS) {
        printf("Cannot add more monitors to the controller\n");
        return FAILURE;
    }
    controller->monitors[controller->num_monitors++] = monitor;
    return SUCCESS;
}

int c_defaultRemoveMonitor(void* self, MonitorSystem* monitor) {
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

void c_defaultUpdate(void* self) {
    // Cast the void pointer to a Updateable pointer
    Updateable* updateable = ((Task*)self)->updateable;

    // Cast the child pointer to a controller system
    System* system = (System*)updateable->child;
    ControllerSystem* controller = (ControllerSystem*)system->child;

    // Perform the controller update
    if (controller->updateController(controller) == FAILURE) {
        return;
    }

    // Set the controller to computed
    controller->state = c_computed;

    // Perform the safety check
    if (controller->safety(controller) == FAILURE) {
        return;
    }
    
    // Return success
    return;
}

int c_defaultSafety(void* self) {
    ControllerSystem* controller = (ControllerSystem*)self;

    if (controller->state != c_computed) {
        printf("Controller not computed new value\n");
        return FAILURE;
    }

    if (controller->num_monitors == 0) {
        printf("No monitors set for Controller\n");
        return FAILURE;
    }
    
    for (int i = 0; i < controller->num_monitors; i++) {
        if (controller->monitors[i]->runMonitor(controller->monitors[i]) == FAILURE) {
            return FAILURE;
        }
    }
    controller->state = c_validated;
    return SUCCESS;
}
