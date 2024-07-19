#include "../../includes/ControllerSystem.h"

/* 
 * Initializes the Controller System with initial settings.
 *
 * @param controller A pointer to the ControllerSystem structure.
 * @param name The name of the controller.
 * @param hz Rate at which the controller is called (in hz).
 * @param type The type of controller (per ControllerType).
*/
void initControllerSystem(ControllerSystem* controller, const char* name, int hz, ControllerType type) {
    initSystem(&controller->system, name, hz, CONTROLLER);
    controller->type = type;
    controller->num_monitors = 0;
    controller->safety = NULL;
}

/* 
 * Adds a monitor to the controller.
 *
 * @param self A pointer to the ControllerSystem structure.
 * @param monitor A pointer to the MonitorSystem structure to add.
 * @return SUCCESS if the monitor was added, FAILURE otherwise.
*/
int defaultAddMonitor(void* self, MonitorSystem* monitor) {
    ControllerSystem* controller = (ControllerSystem*)self;
    if (controller->num_monitors >= MAX_MONITORS) {
        printf("Cannot add more monitors to the controller\n");
        return FAILURE;
    }
    controller->monitors[controller->num_monitors++] = monitor;
    return SUCCESS;
}  

/* 
 * Removes a monitor from the controller.
 *
 * @param self A pointer to the ControllerSystem structure.
 * @param monitor A pointer to the MonitorSystem structure to remove.
 * @return SUCCESS if the monitor was removed, FAILURE otherwise.
*/
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