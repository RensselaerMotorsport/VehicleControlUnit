#include "../../Inc/Systems/ControllerSystem.h"
#include "../../Inc/Utils/Common.h"
#include "../../Inc/Utils/Updateable.h"
#include "../../Inc/Scheduler/Task.h"

void initControllerSystem(ControllerSystem* controller, const char* name, int hz,
                          ControllerType type, int (*updateController)(ControllerSystem* controller),
                          void* child) {
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
    controller->child = child;
    
}

int c_defaultAddMonitor(ControllerSystem* controller, MonitorSystem* monitor) {
    if (controller->num_monitors >= MAX_MONITORS) {
        printf("Cannot add more monitors to the controller\n");
        return _FAILURE;
    }
    controller->monitors[controller->num_monitors++] = monitor;
    return _SUCCESS;
}

int c_defaultRemoveMonitor(ControllerSystem* controller, MonitorSystem* monitor) {
    for (int i = 0; i < controller->num_monitors; i++) {
        if (controller->monitors[i] == monitor) {
            for (int j = i; j < controller->num_monitors - 1; j++) {
                controller->monitors[j] = controller->monitors[j + 1];
            }
            controller->num_monitors--;
            return _SUCCESS;
        }
    }
    printf("Monitor not found in the controller\n");
    return _FAILURE;
}

int c_defaultUpdate(Updateable* updateable) {
    // Cast the updateable pointer to a controller system
    System* system = (System*)updateable->child;
    ControllerSystem* controller = (ControllerSystem*)system->child;

    // Perform the controller update
    if (controller->updateController(controller) == _FAILURE) {
        return _FAILURE;
    }

    // Set the controller to computed
    controller->state = c_computed;

    // Perform the safety check
    if (controller->safety(controller) == _FAILURE) {
        return _FAILURE;
    }
    
    // Return _SUCCESS
    return _SUCCESS;
}

int c_defaultSafety(ControllerSystem* controller) {

    if (controller->state != c_computed) {
        printf("Controller not computed new value\n");
        return _FAILURE;
    }

    if (controller->num_monitors == 0) {
        printf("No monitors set for Controller\n");
        return _FAILURE;
    }
    
    for (int i = 0; i < controller->num_monitors; i++) {
        if (controller->monitors[i]->runMonitor(controller->monitors[i]) == _FAILURE) {
            return _FAILURE;
        }
    }
    controller->state = c_validated;
    return _SUCCESS;
}
