#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Forward declare the main functions
void lut_main();
int wheel_speed_main();
int torque_control_main();
// FIXME Needs fixing (check with code owners)
// int brake_main();
int apps_main();
int bms_main();

// FIXME Temporary wrapper for lut_main (void) until migrated
int lut_main_wrapper() {
  lut_main();
  return 0;
}

// Defines a map from a string to a main function.
typedef struct {
  const char *name;
  // TODO Eventually this can / should become void
  int (*main)();
} map;

// NOTE run_all_tests calls each of the values (mains) of this!
// Adding aliases / multiple of the same will cause multiple runs
static map name_to_main[] = {{"apps", apps_main},
                             /*{ "brake", brake_main },*/
                             {"bms", bms_main},
                             {"lut", lut_main_wrapper},
                             {"torque_control", torque_control_main},
                             {"wheel_speed", wheel_speed_main}};
#define NUM_TESTS (sizeof(name_to_main)/sizeof(name_to_main[0]))

void run_all_tests() {
  for (int n = 0; n < NUM_TESTS; ++n) {
    name_to_main[n].main();
  }
  exit(EXIT_SUCCESS);
}

void run_test(const char *name) {
  for (int n = 0; n < NUM_TESTS; ++n) {
    map entry = name_to_main[n];
    if (strncmp(entry.name, name, strlen(entry.name)) == 0) {
      entry.main();
    }
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  bool all = false;

  int opt;
  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch (opt) {
    case 'a':
      all = true;
      break;
    default:
      fprintf(stderr, "Usage: %s [-a] [tests]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (all) {
    run_all_tests();
  }

  if (optind >= argc) {
    fprintf(stderr, "Expected name of test\n");
    exit(EXIT_FAILURE);
  }

  for (int n = optind; n < argc; ++n) {
    run_test(argv[n]);
  }

  return EXIT_SUCCESS;
}
