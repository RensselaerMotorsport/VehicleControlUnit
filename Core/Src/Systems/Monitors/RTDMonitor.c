#include "../../../Inc/Systems/Monitor/RTDMonitor.h"
#include "../../../Inc/Utils/Common.h"

void initRTDMonitor(RTDMonitor* rtdm, RTD* rtd, int hz) {
  initMonitorSystem(&rtdm->base, "RTD Monitor", hz, m_RTD, VEHICLE_SHUTDOWN, checkRTDMonitor);
  rtdm->rtd = rtd;
}

int startRTDMonitor(RTDMonitor* rtdm) {
  if (rtdm->base.runMonitor == NULL) {
    printf("Monitor function not set for RTD Monitor\n");
    return _FAILURE;
  }
  ENABLE(rtdm->base.system);
  return _SUCCESS;
}

int checkRTDMonitor(void* rtdm) {
  // RTD controller is always in a safe state
  return _SUCCESS;
}
