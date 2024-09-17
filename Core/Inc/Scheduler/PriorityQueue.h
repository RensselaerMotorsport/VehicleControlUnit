#ifndef RENSSELAERMOTORSPORT_PRIORITYQUEUE_H
#define RENSSELAERMOTORSPORT_PRIORITYQUEUE_H

#include <stdbool.h>

#include "Task.h"

#define MAX_SENSORS 10

typedef struct {
    int priority;
    Task task;
} PQNode;

typedef struct {
    // Use 1-based indexing for easier parent/child calculations
    PQNode nodes[MAX_SENSORS + 1];
    int size;
} PriorityQueue;

/**
 * @brief Initializes the priority queue.
 *
 * @param pq Pointer to the PriorityQueue structure to initialize.
 */
void PQInit(PriorityQueue* pq);

/**
 * @brief Pushes a task with a given priority onto the priority queue.
 *
 * @param pq       Pointer to the PriorityQueue structure.
 * @param task     The task to push onto the queue.
 * @param priority The priority of the task.
 * @return         True if the task was successfully pushed, false if the queue is full.
 */
bool PQPush(PriorityQueue* pq, Task task, int priority);

/**
 * @brief Pops the highest priority task from the priority queue.
 *
 * @param pq   Pointer to the PriorityQueue structure.
 * @param task Pointer to the Task structure to store the popped task.
 * @return     True if a task was successfully popped, false if the queue is empty.
 */
bool PQPop(PriorityQueue* pq, Task* task);


/**
 * @brief Peeks at the highest priority task in the priority queue without removing it.
 *
 * @param pq   Pointer to the PriorityQueue structure.
 * @param task Pointer to the Task structure to store the peeked task.
 * @return     True if a task was successfully peeked, false if the queue is empty.
 */
bool PQPeek(PriorityQueue* pq, Task* task);

/**
 * @brief Checks if the priority queue is empty.
 *
 * @param pq Pointer to the PriorityQueue structure.
 * @return   True if the queue is empty, false otherwise.
 */
bool PQIsEmpty(PriorityQueue* pq);

/**
 * @brief Checks if the priority queue is full.
 *
 * @param pq Pointer to the PriorityQueue structure.
 * @return   True if the queue is full, false otherwise.
 */
bool PQIsFull(PriorityQueue* pq);

#endif // RENSSELAERMOTORSPORT_PRIORITYQUEUE_H
