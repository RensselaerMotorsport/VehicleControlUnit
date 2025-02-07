#ifndef RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H
#define RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"
#include "MonitorSystem.h"
#include "../Utils/Updateable.h"

#define MAX_MONITORS 10

typedef enum {
    c_TORQUE,
    c_APPS,
    c_BRAKES,
} ControllerType;

typedef enum {
    c_idle,
    c_computed,
    c_validated
} ControllerState;

typedef struct {
    System system;
    ControllerType type;
    ControllerState state;
    MonitorSystem* monitors[MAX_MONITORS]; // What monitors are associated with this controller?
    int num_monitors;
    int (*safety)(void* self); // Check if the controller is operating safely
    int (*addMonitor)(void* self, MonitorSystem* monitor); // Add a monitor to the controller
    int (*removeMonitor)(void* self, MonitorSystem* monitor); // Remove a monitor from the controller
    int (*updateController)(void* self); // Update the controller
} ControllerSystem;

/**
 * @brief Initializes the Controller System with initial settings.
 *
 * @param controller A pointer to the ControllerSystem structure.
 * @param name The name of the controller.
 * @param hz Rate at which the controller is called (in hz).
 * @param type The type of controller (per ControllerType).
 * @param updateController The function to update the controller.
*/
void initControllerSystem(ControllerSystem* controller, const char* name, int hz,
                          ControllerType type, int (*updateController)(void* self));

/**
 * @brief Adds a monitor to the controller.
 *
 * @param self A pointer to the ControllerSystem structure.
 * @param monitor A pointer to the MonitorSystem structure to add.
 * @return SUCCESS if the monitor was added, FAILURE otherwise.
*/
int c_defaultAddMonitor(void* self, MonitorSystem* monitor);

/**
 * @brief Removes a monitor from the controller.
 *
 * @param self A pointer to the ControllerSystem structure.
 * @param monitor A pointer to the MonitorSystem structure to remove.
 * @return SUCCESS if the monitor was removed, FAILURE otherwise.
*/
int c_defaultRemoveMonitor(void* self, MonitorSystem* monitor);

/**
 * @brief Default update function for ControllerSystem objects.
 *
 * @param pointer to the Updateable object.
*/
void c_defaultUpdate(void* self);

/**
 * @brief Default safety function for ControllerSystem objects.
 *
 * @param self Pointer to the ControllerSystem object.
 * @return int Status of the ControllerSystem object.
*/
int c_defaultSafety(void* self);

#endif // RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H

