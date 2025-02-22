#include "../../Inc/Sensors/Sensor.h"
#include "../../Inc/Utils/Updateable.h"

void initSensor(Sensor* sensor, const char* name, int hz, SensorType type) {
    initUpdateable(&sensor->updateable, name, hz, SENSOR, sensor);
    sensor->type = type;
}
