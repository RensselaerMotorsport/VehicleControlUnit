#include "../../Inc/Utils/Updateable.h"
#include "../../Inc/Utils/Common.h"
#include <string.h>

void initUpdateable(Updateable* updateable, const char* name, int hz) {
    strncpy(updateable->name, name, MAX_NAME_LENGTH);
    updateable->hz = hz;
    updateable->update = defaultUpdate;
    updateable->status = defaultStatus;
    updateable->enable = defaultEnable;
    updateable->disable = defaultDisable;
    updateable->enabled = DISABLED;
}

void defaultUpdate(void* self) {
    printf("Warning: Calling default Sensor Update Function.\n");
}

int defaultStatus(struct Updateable* self) {
    return self->enabled;
}

int defaultEnable(struct Updateable* self) {
    self->enabled = ENABLED;
    return SUCCESS;
}

int defaultDisable(struct Updateable* self) {
    self->enabled = DISABLED;
    return SUCCESS;
}

int writeDataToFileImplementation(const char* filename, void* self) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return FAILURE;
    }

    printf("Writing data to file %s\n", filename);

    // TODO: Implement this function
    // for (size_t i = 0; i < sensor->logger.count; i++) {
    //     fprintf(file, "%d,%f\n", sensor->logger.data[i].time, sensor->logger.data[i].value);
    // }

    fclose(file);
    return FAILURE;
}
