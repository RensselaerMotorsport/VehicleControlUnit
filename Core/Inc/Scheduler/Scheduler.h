#ifndef RENSSELAERMOTORSPORT_SCHEDULER_H
#define RENSSELAERMOTORSPORT_SCHEDULER_H

#include <stdbool.h>
#include "../Utils/Updateable.h"

#define MAX_SENSORS 10
#define MAX_HZ 1000

/**
 * @brief Initializes the RTOS scheduler with the given Updatables.
 *
 * @param updateableArray Array of pointers to Updateable structures to be scheduled.
 */
void SchedulerInit(Updateable* updatableArray[]);

#endif // RENSSELAERMOTORSPORT_SCHEDULER_H
