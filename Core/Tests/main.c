// Forward declare the main functions
void lut_main();
int wheel_speed_main();
int torque_control_main();
// FIXME Needs fixing (check with code owners)
// int brake_main();
int apps_main();
int bms_main();

int main() {
  lut_main();
  wheel_speed_main();
  torque_control_main();
  // FIXME Needs fixing (check with code owners)
  // brake_main();
  apps_main();
  bms_main();
  return 0;
}
