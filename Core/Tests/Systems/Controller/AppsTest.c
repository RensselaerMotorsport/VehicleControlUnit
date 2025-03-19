#include "../Inc/Systems/Controller/Apps.h"
#include "test.h"

// NOTE Snippet common to almost all tests
#define APPS_DO(pos1, pos2) \
  Apps apps; \
  initApps(&apps, 0, 0, 1); \
  setAppPos(apps.app[0], pos1); \
  setAppPos(apps.app[1], pos2); \
  updateApps(&apps); \
  float pos = getAppsPosition(&apps);

void apps_main() {
    TEST(apps_init, {
      Apps apps;
      initApps(&apps, 0, 0, 1);
      ASSERT_OK(apps.status == APPS_OK, "intial status");
    })

    TEST(normal_operation_0, {
      APPS_DO(0, 0)
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(normal_operation_50, {
      APPS_DO(50, 50)
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 50, "position", "expected position");
    })
  
    TEST(normal_operation_100, {
      APPS_DO(100, 100)
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 100, "position", "expected position");
    })

    TEST(normal_operation_50_60, {
      APPS_DO(50, 60);
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 55, "position", "expected position");
    })

    TEST(normal_operation_55_56, {
      APPS_DO(55, 56);
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 55.5, "position", "expected position");
    })

    TEST(edge_difference_0_10, {
      APPS_DO(0, 10);
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 5, "position", "expected position");
    })

    TEST(edge_difference_50_60, {
      APPS_DO(50, 60);
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 55, "position", "expected position");
    })

    TEST(edge_difference_90_100, {
      APPS_DO(90, 100);
      ASSERT(apps.status == APPS_OK, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 95, "position", "expected position");
    })

    TEST(edge_difference_0_1001, {
      APPS_DO(0, 10.01);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(edge_difference_50_61, {
      APPS_DO(50, 51);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(edge_difference_0_100, {
      APPS_DO(0, 100);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(sensor_1_under, {
      APPS_DO(-1, 50);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(sensor_1_over, {
      APPS_DO(101, 50);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(sensor_2_under, {
      APPS_DO(50, -1);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(sensor_2_over, {
      APPS_DO(50, 101);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(sensors_under, {
      APPS_DO(-1, -1);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })

    TEST(sensors_over, {
      APPS_DO(101, 101);
      ASSERT(apps.status == APPS_FAULT, "status is expected", "status is not expected");
      ASSERT_EQ(pos, 0, "position", "expected position");
    })
}
