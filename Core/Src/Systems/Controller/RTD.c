#include "../../../Inc/Utils/Constants.h"
#include "../../../Inc/Systems/Controller/RTD.h"
#include "../../../Inc/Systems/ControllerSystem.h"
#include "../../../Inc/Utils/Common.h"

void initRTD(RTD* rtd, Apps* apps, BrakeSystemControl* bsc, int hz, int buttonPort, int piezoPort) {
  initControllerSystem(&rtd->base, "RTD", hz, c_RTD, updateRTD, rtd);
  rtd->readyToDrive = 0;
  rtd->precharged = 0;
  rtd->counter = 0;

  Button button;
  rtd->button = &button;
  initButton(rtd->button, "RTD Button", hz, 0);

  DigitalOutput piezo;
  rtd->piezo = &piezo;
  initDigitalOutput(rtd->piezo, "Piezo", hz, piezoPort);
  rtd->apps = apps;
  rtd->bsc = bsc;
}

int startRTD(RTD* rtd) {
  if (rtd->base.safety == NULL) {
    printf("Safety system not set for RTD\r\n");
    return _FAILURE;
  }
  else if (rtd->base.safety(&rtd->base) == _FAILURE) {
    printf("RTD Actuator is not in a safe state\r\n");
    return _FAILURE;
  }
  ENABLE(rtd->base.system);
  rtd->base.state = c_idle;
  return _SUCCESS;
}

int updateRTD(ControllerSystem* controller) {
  RTD* rtd = (RTD*)controller->child;
  if (rtd->apps->base.state != c_validated || rtd->bsc->base.state != c_validated) {
    printf("RTD: Apps or Brake System Control Actuator is not validated\r\n");
    return _FAILURE;
  }

  if (getAppsPosition(rtd->apps) > MAX_A_PEDAL_POSITION) {
    printf("RTD: APPS position is too high\r\n");
    return _FAILURE;
  }

  if (rtd->bsc->rearPressure->pressure < MIN_BRAKE_PRESSURE 
      && rtd->bsc->frontPressure->pressure < MIN_BRAKE_PRESSURE) {
    printf("RTD: Brake pressure is too low\r\n");
    return _FAILURE;
  }

  if (rtd->readyToDrive == 0) {
    rtd->readyToDrive = getButtonReading(rtd->button); // and precharged, etc.
  } 

  if (rtd->readyToDrive == 1 && rtd->counter < 30) {
    rtd->counter++;
    // Turn on piezo
    writeDigitalOutputData(rtd->piezo, 1);
  } else {
    // Turn off piezo
    writeDigitalOutputData(rtd->piezo, 0);
  }

  rtd->base.state = c_computed;
  
  #ifdef DEBUGn
  printf("RTD updated. Ready to Drive: %d\r\n", rtd->readyToDrive);
  #endif

  return _SUCCESS;
}

int checkRTD(void* rtd) {
  RTD* rtdPtr = (RTD*)rtd;
  if (rtdPtr->readyToDrive == 1) {
      printf("RTD is ready to drive\r\n");
      return _SUCCESS;
  }
  return _FAILURE;
}
