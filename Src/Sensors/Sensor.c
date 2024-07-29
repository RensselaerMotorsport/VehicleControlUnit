#include "../../includes/Sensor.h"
#include "../../includes/Updateable.h"
#include <string.h>

void initSensor(Sensor* sensor, const char* name, int hz, SensorType type) {
    initUpdateable(&sensor->updateable, name, hz);
    sensor->type = type;
}