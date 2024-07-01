#include "../../Inc/Sensors/Sensor.h"
#include <string.h>

void initSensor(Sensor* sensor, const char* name, int hz) {
    strncpy(sensor->sensorName, name, MAX_SENSOR_NAME_LENGTH);
    sensor->hz = hz;
    sensor->update = defaultUpdate;
}

void defaultUpdate(void* self) {
    printf("Warning: Calling default Sensor Update Function.\n");
}

void writeDataToFileImplementation(const char* filename, void* self) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    printf("Writing data to file %s\n", filename);

    // TODO: Implement this function
    // for (size_t i = 0; i < sensor->logger.count; i++) {
    //     fprintf(file, "%d,%f\n", sensor->logger.data[i].time, sensor->logger.data[i].value);
    // }

    fclose(file);
}
