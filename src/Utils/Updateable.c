#include "../../includes/Updateable.h"
#include "../../includes/Common.h"
#include <string.h>

/* 
 * Initializes the Updateable with initial settings.
 *
 * @param updateable A pointer to the Updateable structure.
 * @param name The name of the updateable.
 * @param hz Rate at which the updateable is called (in hz).
*/
void initUpdateable(Updateable* updateable, const char* name, int hz) {
    strncpy(updateable->name, name, MAX_NAME_LENGTH);
    updateable->hz = hz;
    updateable->update = defaultUpdate;
    updateable->status = defaultStatus;
    updateable->enable = defaultEnable;
    updateable->disable = defaultDisable;
    updateable->enabled = DISABLED;
}

/* 
 * Default update function for the Updateable.
 *
 * @param self A pointer to the Updateable structure.
*/
void defaultUpdate(void* self) {
    printf("Warning: Calling default Sensor Update Function.\n");
}

/* 
 * Default status function for the Updateable.
 *
 * @param self A pointer to the Updateable structure.
*/
int defaultStatus(struct Updateable* self) {
    return self->enabled;
}

/* 
 * Default enable function for the Updateable.
 *
 * @param self A pointer to the Updateable structure.
*/
int defaultEnable(struct Updateable* self) {
    self->enabled = ENABLED;
    return SUCCESS;
}

/* 
 * Default disable function for the Updateable.
 *
 * @param self A pointer to the Updateable structure.
*/
int defaultDisable(struct Updateable* self) {
    self->enabled = DISABLED;
    return SUCCESS;
}

/* 
 * Writes data to a file.
 *
 * @param filename The name of the file to write to.
 * @param self A pointer to the Updateable structure.
 * @return SUCCESS if the data was written, FAILURE otherwise.
*/
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
}
