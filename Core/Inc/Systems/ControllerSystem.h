#ifndef RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H
#define RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"
#include "MonitorSystem.h"
#include "../Utils/Constants.h"
#include "../Utils/Updateable.h"
#include "../Scheduler/Task.h"

typedef enum {
    c_TORQUE,
    c_APPS,
    c_BRAKES,
    c_RTD
} ControllerType;

typedef enum {
    c_idle,
    c_computed,
    c_validated
} ControllerState;

// Need to do this because of circular dependency
typedef struct ControllerSystem ControllerSystem;
struct ControllerSystem {
    System system;
    ControllerType type;
    ControllerState state;
    MonitorSystem* monitors[MAX_MONITORS]; // What monitors are associated with this controller?
    int num_monitors;
    int (*safety)(struct ControllerSystem* controller); // Check if the controller is operating safely
    int (*addMonitor)(struct ControllerSystem* controller, MonitorSystem* monitor); // Add a monitor to the controller
    int (*removeMonitor)(struct ControllerSystem* controller, MonitorSystem* monitor); // Remove a monitor from the controller
    int (*updateController)(struct ControllerSystem* controller); // Update the controller
    void* child;
};

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
                          ControllerType type, int (*updateController)(ControllerSystem* controller),
                          void* child);

/**
 * @brief Adds a monitor to the controller.
 *
 * @param ctl A pointer to the ControllerSystem structure.
 * @param mtr A pointer to the MonitorSystem structure to add.
 * @return _SUCCESS if the monitor was added, _FAILURE otherwise.
*/
int c_defaultAddMonitor(ControllerSystem* controller, MonitorSystem* monitor);

/**
 * @brief Removes a monitor from the controller.
 *
 * @param ctl A pointer to the ControllerSystem structure.
 * @param mtr A pointer to the MonitorSystem structure to remove.
 * @return _SUCCESS if the monitor was removed, _FAILURE otherwise.
*/
int c_defaultRemoveMonitor(ControllerSystem* controller, MonitorSystem* monitor);

/**
 * @brief Default update function for ControllerSystem objects.
 *
 * @param pointer to the Updateable object.
 * @return int _SUCCESS or _FAILURE.
*/
int c_defaultUpdate(Updateable* updateable);

/**
 * @brief Default safety function for ControllerSystem objects.
 *
 * @param self Pointer to the ControllerSystem object.
 * @return int Status of the ControllerSystem object.
*/
int c_defaultSafety(ControllerSystem* controller);

#endif // RENSSELAERMOTORSPORT_CONTROLLER_SYSTEM_H

