#ifndef RENSSELAERMOTORSPORT_BRAKE_POLICE_H
#define RENSSELAERMOTORSPORT_BRAKE_POLICE_H

#include "../MonitorSystem.h"
#include "../Controller/BrakeSystemControl.h"

typedef struct {
    MonitorSystem base;
    int maxTemperatureAllowed;
    BrakeSystemStatus status;
    BrakeSystemControl* bsc;
} BrakePolice;

/*
* @brief Initializes the Brake Police with initial settings.
*
* @param bp A pointer to the BrakePolice structure.
* @param bsc A pointer to the BrakeSystemControl structure.
* @param hz Rate at which the system operates (in Hz).
* @param maxTemp The maximum temperature limit set for the system (in Fahrenheit) for the brakediscs.
*/
void initBrakePolice(BrakePolice* bp, BrakeSystemControl *bsc, int hz, int maxTemp);

/*
* @brief Starts the Brake Police system.
*
* @param bp A pointer to the BrakePolice structure.
* @return int _SUCCESS or _FAILURE.
*/
int startBrakePolice(BrakePolice* bp);

/* 
* @brief Updates the Brake Police system based on the brake system control.
*
* @param bp A pointer to the BrakePolice structure.
* @return int _SUCCESS or _FAILURE.
*/
int checkBrakePolice(void* bp);

#endif // RENSSELAERMOTORSPORT_BRAKE_POLICE_H
