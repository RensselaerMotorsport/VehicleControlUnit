#include "../../Inc/Utils/Updateable.h"
#include "../../Inc/Utils/Common.h"

#include <string.h>

void initUpdateable(Updateable* updateable, const char* name, int hz, UpdateableType utype, void* child) {
    updateable->hz = hz;
    updateable->update = defaultUpdate;
    updateable->status = defaultStatus;
    updateable->enable = defaultEnable;
    updateable->disable = defaultDisable;
    updateable->enabled = DISABLED;
    strncpy(updateable->name, name, MAX_NAME_LENGTH);
    updateable->type = utype;
    // Have a pointer to the child struct
    updateable->child = child;
}

void defaultUpdate(void* self) {
    // Print in yellow color
    printf(ANSI_COLOR_YELLOW "Warning: Calling default Update Function for %s\n" ANSI_COLOR_RESET, ((Updateable*)self)->name);
}

int defaultStatus(struct Updateable* self) {
    return self->enabled;
}

int defaultEnable(struct Updateable* self) {
    self->enabled = ENABLED;
    return _SUCCESS;
}

int defaultDisable(struct Updateable* self) {
    self->enabled = DISABLED;
    return _SUCCESS;
}

int writeDataToFileImplementation(const char* filename, void* self) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return _FAILURE;
    }

    printf("Writing data to file %s\n", filename);

    fclose(file);
    return _FAILURE;
}
