#include "../../Inc/Scheduler/PriorityQueue.h"

void PQInit(PriorityQueue* pq) {
    pq->size = 0;
}

/**
 * @brief Restores the heap property by moving the node at the given index up.
 *
 * @param pq    Pointer to the PriorityQueue structure.
 * @param index The index of the node to move up.
 */
void heapifyUp(PriorityQueue* pq, int index) {
    while (index > 1 && pq->nodes[index / 2].priority > pq->nodes[index].priority) {
        PQNode temp = pq->nodes[index];
        pq->nodes[index] = pq->nodes[index / 2];
        pq->nodes[index / 2] = temp;
        index /= 2;
    }
}


/**
 * @brief Restores the heap property by moving the node at the given index down.
 *
 * @param pq    Pointer to the PriorityQueue structure.
 * @param index The index of the node to move down.
 */
void heapifyDown(PriorityQueue* pq, int index) {
    while (2 * index <= pq->size) {
        int j = 2 * index;
        if (j < pq->size && pq->nodes[j].priority > pq->nodes[j + 1].priority) j++;
        if (pq->nodes[index].priority <= pq->nodes[j].priority) break;
        PQNode temp = pq->nodes[index];
        pq->nodes[index] = pq->nodes[j];
        pq->nodes[j] = temp;
        index = j;
    }
}

bool PQPush(PriorityQueue* pq, Task task, int priority) {
    if (PQIsFull(pq)) return false;
    pq->nodes[++pq->size] = (PQNode){priority, task};
    heapifyUp(pq, pq->size);
    return true;
}

bool PQPop(PriorityQueue* pq, Task* task) {
    if (PQIsEmpty(pq)) return false;
    *task = pq->nodes[1].task;
    pq->nodes[1] = pq->nodes[pq->size--];
    heapifyDown(pq, 1);
    return true;
}

bool PQPeek(PriorityQueue* pq, Task* task) {
    if (PQIsEmpty(pq)) return false;
    *task = pq->nodes[1].task;
    return true;
}

bool PQIsEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

bool PQIsFull(PriorityQueue* pq) {
    return pq->size == MAX_SENSORS;
}
