#include "../../../Inc/Sensors/DigitalSensor.h"
#include "../../../Inc/Sensors/DigitalSensors/Button.h"

void initButton(Button* button, const char* name, int hz, int port) {
  initDigitalSensor(&button->base, name, hz, port);
}

int getButtonReading(Button* button) {
  return getDigitalSensorReading(&button->base);
}
