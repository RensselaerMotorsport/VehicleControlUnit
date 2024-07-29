#ifndef RENNSSELAERMOTORSPORT_IMD_H
#define RENNSSELAERMOTORSPORT_IMD_H

#include <stdbool.h>
#include "CANSensor.h"

typedef struct {
    CANSensor base;
    bool imd_status;
    bool running_flag;
} Imd;

void initImd(Imd* imd, int hz);
bool getImdStatus(const Imd* imd);
bool getRunningFlag(const Imd* imd);
void resetRunningFlag(Imd* imd);
void updateImd(void* imd);
char* toStringImd(const Imd* imd);

#endif
