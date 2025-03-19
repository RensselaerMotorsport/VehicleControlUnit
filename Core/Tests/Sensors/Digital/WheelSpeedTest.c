#include "../Inc/Sensors/DigitalSensors/WheelSpeed.h"
#include "test.h"

float testTransferFunction(float radius, int numTeeth, int pulseCount,
                           float timeInterval) {
  WheelSpeed wheelSpeed;
  int wheelSpeedHz = 200;
  initWheelSpeed(&wheelSpeed, wheelSpeedHz, 0, radius, numTeeth, 0);

  addPulse(&wheelSpeed, pulseCount);
  // Inverting time because get function relies on current time
  setTimeInterval(&wheelSpeed, -timeInterval);

  return calculateSpeed(&wheelSpeed);
}

void wheel_speed_main() {
  TEST(low_speed_1, {
    ASSERT_IN_ERROR(testTransferFunction(0.127f, 48, 100, 0.5f), 7.44f, "speed",
                    "expected speed");
  })

  TEST(low_speed_2, {
    ASSERT_IN_ERROR(testTransferFunction(0.15f, 60, 100, 1.0f), 3.51f, "speed",
                    "expected speed");
  })

  TEST(low_speed_3, {
    ASSERT_IN_ERROR(testTransferFunction(0.1f, 40, 25, 0.25f), 3.51f, "speed",
                    "expected speed");
  })

  TEST(medium_speed_1, {
    ASSERT_IN_ERROR(testTransferFunction(0.127f, 48, 300, 0.5f), 22.31f,
                    "speed", "expected speed");
  })

  TEST(medium_speed_2, {
    ASSERT_IN_ERROR(testTransferFunction(0.15f, 60, 300, 1.0f), 10.54f, "speed",
                    "expected speed");
  })

  TEST(medium_speed_3, {
    ASSERT_IN_ERROR(testTransferFunction(0.1f, 40, 150, 0.25f), 21.08f, "speed",
                    "expected speed");
  })

  TEST(high_speed_1, {
    ASSERT_IN_ERROR(testTransferFunction(0.127f, 48, 1000, 0.5f), 74.37f,
                    "speed", "expected speed");
  })

  TEST(high_speed_2, {
    ASSERT_IN_ERROR(testTransferFunction(0.15f, 60, 1000, 1.0f), 35.14f,
                    "speed", "expected speed");
  })

  TEST(high_speed_3, {
    ASSERT_IN_ERROR(testTransferFunction(0.1f, 40, 500, 0.25f), 70.28f, "speed",
                    "expected speed");
  })
}
