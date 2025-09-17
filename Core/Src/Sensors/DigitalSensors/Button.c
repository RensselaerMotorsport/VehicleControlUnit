#include "../../../Inc/Sensors/DigitalSensor.h"
#include "../../../Inc/Sensors/DigitalSensors/Button.h"

void initButton(Button* button, const char* name, int hz, int port) {
  initDigitalSensor(&button->base, name, hz, port, button);
  button->telem_state = registerTelemetrySignal(name, TELEMETRY_SENSOR, UNIT_BOOLEAN, 1000/hz, 0, 1);
}

int getButtonReading(Button* button) {
  // Send the button state to the debugger
  // getDigitalSensorReading reads from a buffer, so separate calls are safe
  sendTelemetryValue(button->telem_state, getDigitalSensorReading(&button->base));
  return getDigitalSensorReading(&button->base);
}
