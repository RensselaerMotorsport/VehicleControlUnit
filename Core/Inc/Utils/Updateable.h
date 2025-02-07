#ifndef RENSSELAERMOTORSPORT_UPDATEABLE_H
#define RENSSELAERMOTORSPORT_UPDATEABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UPDATE(item_)  ((item_)->updateable->update(item_))
#define STATUS(item_)  ((item_).updateable.status(&item_.updateable))
#define ENABLE(item_)  ((item_).updateable.enable(&item_.updateable))
#define DISABLE(item_)  ((item_).updateable.disable(&item_.updateable))

#define MAX_NAME_LENGTH 32
#define DISABLED 0
#define ENABLED 1

typedef enum {
    SENSOR,
    SYSTEM
} UpdateableType;

typedef struct Updateable {
    char name[MAX_NAME_LENGTH];
    int hz;
    int enabled;
    void (*update)(void* self);
    int (*status)(struct Updateable* self);
    int (*enable)(struct Updateable* self);
    int (*disable)(struct Updateable* self);
    UpdateableType type;
    void* child;
} Updateable;

/*
 * @brief Initializes the Updateable with initial settings.
 *
 * @param updateable A pointer to the Updateable structure.
 * @param name The name of the updateable.
 * @param hz Rate at which the updateable is called (in hz).
*/
void initUpdateable(Updateable* updateable, const char* name, int hz, UpdateableType utype, void* child);

/**
 * @brief Default update function for Updateable objects.
 *
 * @param self Pointer to the object to update.
 */
void defaultUpdate(void* self);

/**
 * @brief Default status function for Updateable objects.
 *
 * @param self Pointer to the Updateable object.
 * @return int Status of the Updateable object.
 */
int defaultStatus(struct Updateable* self);

/**
 * @brief Default enable function for Updateable objects.
 *
 * @param self Pointer to the Updateable object.
 * @return int Status after enabling the Updateable object.
 */
int defaultEnable(struct Updateable* self);

/**
 * @brief Default disable function for Updateable objects.
 *
 * @param self Pointer to the Updateable object.
 * @return int Status after disabling the Updateable object.
 */
int defaultDisable(struct Updateable* self);

/**
 * @brief Write data to a file.
 *
 * @param filename Name of the file to write to.
 * @param self Pointer to the object whose data is to be written.
 * @return SUCCESS if the data was written, FAILURE otherwise.
 */
int writeDataToFileImplementation(const char* filename, void* self);

#endif // RENSSELAERMOTORSPORT_UPDATEABLE_H
