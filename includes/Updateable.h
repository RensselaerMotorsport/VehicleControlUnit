#ifndef RENSSELAERMOTORSPORT_UPDATEABLE_H
#define RENSSELAERMOTORSPORT_UPDATEABLE_H
#define UPDATE(item_)  ((item_)->updateable.update(item_))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 64

typedef enum {
    SENSOR,
    ACTUATOR
} type;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int hz;
    void (*update)(void* self);
} Updateable;

void initUpdateable(Updateable* updateable, const char* name, int hz);
void defaultUpdate(void* self);

#endif
