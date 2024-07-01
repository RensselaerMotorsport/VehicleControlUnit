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
    PQNode nodes[MAX_SENSORS + 1]; // Use 1-based indexing for easier parent/child calculations
    int size;
} PriorityQueue;

void PQInit(PriorityQueue* pq);
bool PQPush(PriorityQueue* pq, Task task, int priority);
bool PQPop(PriorityQueue* pq, Task* task);
bool PQPeek(PriorityQueue* pq, Task* task);
bool PQIsEmpty(PriorityQueue* pq);
bool PQIsFull(PriorityQueue* pq);

#endif
