#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "../../Inc/Scheduler/Scheduler.h"

typedef struct {
    Updateable* updateable;
    TimerHandle_t timer;
} ScheduledUpdateable;

// Work item for the worker task
typedef struct {
    Updateable* updateable;
    TickType_t timestamp;
} WorkItem;

static ScheduledUpdateable scheduledTasks[MAX_SENSORS];
static int taskCount = 0;

// Worker task handles
static TaskHandle_t workerTaskHandle = NULL;
static QueueHandle_t workQueue = NULL;

#define WORK_QUEUE_SIZE 32
#define WORKER_STACK_SIZE 1024
#define WORKER_PRIORITY (tskIDLE_PRIORITY + 2)

// Worker task that processes updateables
static void workerTask(void* pvParameters) {
    WorkItem workItem;
    
    while (1) {
        // Wait for work items from timer callbacks
        if (xQueueReceive(workQueue, &workItem, portMAX_DELAY) == pdTRUE) {
            if (workItem.updateable != NULL) {
                // Record start time for performance monitoring
                TickType_t startTime = xTaskGetTickCount();
                
                // Execute the updateable
                Task task;
                TaskInit(&task, workItem.updateable, workItem.updateable->hz);
                TaskExecute(&task);
                
                // Check execution time
                TickType_t executionTime = xTaskGetTickCount() - startTime;
                TickType_t maxTime = pdMS_TO_TICKS(1000 / workItem.updateable->hz) / 2; // 50% of period
                
                if (executionTime > maxTime) {
                    // TEMPORARY: Disable timing warnings to reduce printf overhead
                    // printf("WARNING: %s took %lu ticks (max recommended: %lu)\n", 
                    //       workItem.updateable->name, executionTime, maxTime);
                }
            }
        }
    }
}

// Fast timer callback - just queues work item
static void updateableTimerCallback(TimerHandle_t xTimer) {
    Updateable* updateable = (Updateable*)pvTimerGetTimerID(xTimer);
    
    if (updateable != NULL && workQueue != NULL) {
        WorkItem workItem = {
            .updateable = updateable,
            .timestamp = xTaskGetTickCount()
        };
        
        // Send to worker task (non-blocking from timer context)
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xQueueSendFromISR(workQueue, &workItem, &xHigherPriorityTaskWoken) != pdTRUE) {
            // Queue full - this indicates the worker task is overloaded
            // TEMPORARY: Disable error messages to reduce printf overhead
            // printf("ERROR: Work queue full for %s - dropping task\n", updateable->name);
        }
        
        // Yield if higher priority task was woken
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void SchedulerInit(Scheduler* scheduler, Updateable* updatableArray[]) {
    scheduler->running = 0;
    taskCount = 0;

    // Create work queue for communicating between timer callbacks and worker task
    workQueue = xQueueCreate(WORK_QUEUE_SIZE, sizeof(WorkItem));
    if (workQueue == NULL) {
        //printf("ERROR: Failed to create work queue\n");
        return;
    }

    // Create worker task to handle updateables
    if (xTaskCreate(workerTask, "SchedulerWorker", WORKER_STACK_SIZE, NULL, 
                    WORKER_PRIORITY, &workerTaskHandle) != pdPASS) {
        //printf("ERROR: Failed to create worker task\n");
        return;
    }

    //printf("Created scheduler worker task\n");

    // Create timers for each updateable
    for (int i = 0; updatableArray[i] != NULL && i < MAX_SENSORS; i++) {
        Updateable* updateable = updatableArray[i];
        
        if (updateable->hz <= 0 || updateable->hz > MAX_HZ) {
            //printf("Warning: Skipping %s - invalid frequency %d Hz\n", 
            //       updateable->name, updateable->hz);
            continue;
        }

        // Calculate period in FreeRTOS ticks
        TickType_t period = pdMS_TO_TICKS(1000 / updateable->hz);
        if (period == 0) period = 1; // Minimum 1 tick for 1kHz tasks
        
        // Create timer name
        char timerName[32];
        //printf(timerName, sizeof(timerName), "T_%s", updateable->name);
        
        // Create FreeRTOS software timer
        TimerHandle_t timer = xTimerCreate(
            timerName,                     // Timer name
            period,                        // Period in ticks
            pdTRUE,                       // Auto-reload (periodic)
            updateable,                   // Timer ID
            updateableTimerCallback       // Callback function
        );
        
        if (timer != NULL) {
            scheduledTasks[taskCount].updateable = updateable;
            scheduledTasks[taskCount].timer = timer;
            taskCount++;
            
            //printf("Created timer: %s at %dHz (period: %lu ticks)\n", 
            //       updateable->name, updateable->hz, period);
        } else {
            //printf("ERROR: Failed to create timer for %s\n", updateable->name);
        }
    }
    
    //printf("Scheduler initialized with %d timers and worker task\n", taskCount);
}

void SchedulerRun(Scheduler* scheduler) {
    scheduler->running = 1;
    
    // Start all timers
    int started = 0;
    for (int i = 0; i < taskCount; i++) {
        if (xTimerStart(scheduledTasks[i].timer, 0) == pdPASS) {
            started++;
        } else {
            //printf("ERROR: Failed to start timer for %s\n", 
            //       scheduledTasks[i].updateable->name);
        }
    }
    
    //printf("Started %d/%d timers\n", started, taskCount);
    
    // Start FreeRTOS scheduler
    vTaskStartScheduler();
}

void SchedulerStop(Scheduler* scheduler) {
    scheduler->running = 0;
    
    // Stop all timers
    for (int i = 0; i < taskCount; i++) {
        if (scheduledTasks[i].timer != NULL) {
            xTimerStop(scheduledTasks[i].timer, portMAX_DELAY);
        }
    }
    
    //printf("All timers stopped\n");
}

// Utility functions
void SchedulerSuspendUpdateable(const char* name) {
    for (int i = 0; i < taskCount; i++) {
        if (scheduledTasks[i].updateable && 
            strcmp(scheduledTasks[i].updateable->name, name) == 0) {
            if (xTimerStop(scheduledTasks[i].timer, 0) == pdPASS) {
                //printf("Suspended timer for %s\n", name);
                return;
            }
        }
    }
    //printf("Warning: Could not find or suspend timer for %s\n", name);
}

void SchedulerResumeUpdateable(const char* name) {
    for (int i = 0; i < taskCount; i++) {
        if (scheduledTasks[i].updateable && 
            strcmp(scheduledTasks[i].updateable->name, name) == 0) {
            if (xTimerStart(scheduledTasks[i].timer, 0) == pdPASS) {
                //printf("Resumed timer for %s\n", name);
                return;
            }
        }
    }
    //printf("Warning: Could not find or resume timer for %s\n", name);
}

void SchedulerGetStats(void) {
    UBaseType_t queueLength = uxQueueMessagesWaiting(workQueue);
    UBaseType_t queueSpaces = uxQueueSpacesAvailable(workQueue);
    
    //printf("Scheduler Statistics:\n");
    //printf("- Active timers: %d\n", taskCount);
    //printf("- Work queue: %lu items pending, %lu spaces available\n", 
    //       queueLength, queueSpaces);
    
    if (workerTaskHandle != NULL) {
        TaskStatus_t taskStatus;
        vTaskGetInfo(workerTaskHandle, &taskStatus, pdTRUE, eInvalid);
        //printf("- Worker task stack high water mark: %u words\n", 
        //       taskStatus.usStackHighWaterMark);
    }
    
    if (queueLength > (WORK_QUEUE_SIZE * 3 / 4)) {
        //printf("WARNING: Work queue is %lu%% full - worker may be overloaded\n",
        //       (queueLength * 100) / WORK_QUEUE_SIZE);
    }
}

// Cleanup function for proper shutdown
void SchedulerCleanup(void) {
    // Delete worker task
    if (workerTaskHandle != NULL) {
        vTaskDelete(workerTaskHandle);
        workerTaskHandle = NULL;
    }
    
    // Delete work queue
    if (workQueue != NULL) {
        vQueueDelete(workQueue);
        workQueue = NULL;
    }
    
    // Delete all timers
    for (int i = 0; i < taskCount; i++) {
        if (scheduledTasks[i].timer != NULL) {
            xTimerDelete(scheduledTasks[i].timer, portMAX_DELAY);
            scheduledTasks[i].timer = NULL;
        }
    }
    
    taskCount = 0;
    //printf("Scheduler cleaned up\n");
}