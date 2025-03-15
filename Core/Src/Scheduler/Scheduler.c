#include "../../Inc/Scheduler/Scheduler.h"
#include "cmsis_os2.h"
#include <stdint.h>

#include <stdio.h>  // Required for printf

/**
 * @brief The process of each updatable.
 *
 * @param pvParameters the Updatable structure, should be automatically assigned.
 */
void SensorTask(void *pvParameters) {
	Updateable *updateable = (Updateable *)pvParameters;
	uint32_t frequency = 1000 / updateable->hz;

	// Infinite loop
	while (1) {
		if (!updateable || !updateable->update) {
			printf("[ERROR] %s Task exited unexpectedly!\r\n", updateable->name);
			osThreadExit();  // Properly terminate task
		}

		UPDATE(updateable);
		printf("[DEBUG SCEDULER] %s is updating...\r\n", updateable->name);
		osDelay(frequency);
	}
}

void SchedulerInit(Updateable* updatableArray[]) {
    for (int i = 0; updatableArray[i] != NULL; i++) {
        if (i >= MAX_SENSORS) {
            printf("Warning: Number of sensors exceeds MAX_SENSORS. "
                   "Some sensors will not be scheduled.\n");
            break;
        }

        Updateable* updateable = (Updateable*)updatableArray[i];
        if (updateable->hz <= 0 || updateable->hz > MAX_HZ) {
            continue; // Skip invalid frequencies
        }

        // Create RTOS task
        osThreadAttr_t thread_attr = {
            .name = updateable->name,
			.stack_size = 128 * 4,
            .priority = osPriorityNormal
        };

        if (osThreadNew(SensorTask, (void*)updateable, &thread_attr) == NULL) {
			fprintf(stderr, "Error: Failed to create thread for %s!\n", updateable->name);
			return;
		}
		printf("[DEBUG SCHEDULER] Created thread for %s\r\n", updateable->name);
    }
}
