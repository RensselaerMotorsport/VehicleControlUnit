#include "../../Inc/Sensors/Sensor.h"
#include "../../Inc/Utils/Updateable.h"

void initSensor(Sensor* sensor, const char* name, int hz, SensorType type, void* child) {
    initUpdateable(sensor, name, hz, SENSOR);
    sensor->type = type;
    sensor->child = child;
}
