#ifndef RENSSELAERMOTORSPORT_UPDATEABLE_H
#define RENSSELAERMOTORSPORT_UPDATEABLE_H
#define UPDATE(item_)  ((item_)->updateable.update(item_))
#define STATUS(item_)  ((item_).updateable.status(&item_.updateable))
#define ENABLE(item_)  ((item_).updateable.enable(&item_.updateable))
#define DISABLE(item_)  ((item_).updateable.disable(&item_.updateable))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 64
#define DISABLED 0
#define ENABLED 1

typedef enum {
    SENSOR,
    SYSTEM
} type;

typedef struct Updateable {
    char name[MAX_NAME_LENGTH];
    int hz;
    int enabled;
    void (*update)(void* self);
    int (*status)(struct Updateable* self);
    int (*enable)(struct Updateable* self);
    int (*disable)(struct Updateable* self);
} Updateable;

void initUpdateable(Updateable* updateable, const char* name, int hz);
void defaultUpdate(void* self);
int defaultStatus(struct Updateable* self);
int defaultEnable(struct Updateable* self);
int defaultDisable(struct Updateable* self);
int writeDataToFileImplementation(const char* filename, void* self);

#endif
