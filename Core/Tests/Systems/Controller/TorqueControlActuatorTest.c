#include "../Inc/Systems/Controller/TorqueControl.h"
#include "test.h"

#define MAX_TORQUE 100

#define TORQUE_DO(torque, desired) \
  TorqueControl tc; \
  initTorqueControl(&tc, 0, MAX_TORQUE); \
  setActualTorque(&tc, torque); \
  setDesiredTorque(&tc, desired); \
  updateTorqueControl(&tc); \
  TorqueStatus status = checkTorqueLimits(&tc);

void torque_control_main() {
    TEST(torque_init, {
      TorqueControl tc;
      initTorqueControl(&tc, 0, 100);
      ASSERT(tc.status == TORQUE_OK, "status is expected", "status is not expected");
    })

    TEST(update_normal, {
      TORQUE_DO(75, 75)
      ASSERT(tc.status == TORQUE_OK, "status is expected", "status is not expected");
    })

    TEST(update_below_desired, {
      TORQUE_DO(93.5, 95)
      ASSERT(tc.status == TORQUE_OK, "status is expected", "status is not expected");
    })

    TEST(update_above_desired, {
      TORQUE_DO(98.5, 95)
      ASSERT(tc.status == TORQUE_OK, "status is expected", "status is not expected");
    })

    TEST(update_over_limit, {
      TORQUE_DO(101, 95)
      ASSERT(tc.status == TORQUE_OVER_LIMIT, "status is expected", "status is not expected");
    })

    TEST(update_negative_torque, {
      TORQUE_DO(-10, 95)
      ASSERT(tc.status == TORQUE_UNDER_LIMIT, "status is expected", "status is not expected");
    })

    TEST(update_sensor_error, {
      TORQUE_DO(10, 80)
      ASSERT(tc.status == TORQUE_SENSOR_ERROR, "status is expected", "status is not expected");
    })
}
