#ifndef RENSSELAERMOTORSPORT_IMD_H
#define RENSSELAERMOTORSPORT_IMD_H

#include <stdbool.h>
#include "../ExternalSystem.h"

typedef enum {
    IMD_OK,
    IMD_ERROR,
    IMD_START
} IMDStatus;

typedef struct {
    ExternalSystem base;
    bool runningFlag; //Status: Warnings and Alarms //IMD_Info_General: Byte 4, 5
    float IsoResistanceNeg; //IMD_Info_IsolationDetail: Byte 0, 1
    float IsoResistancePos; //IMD_Info_IsolationDetail: Byte 0, 1
    float IsoResistanceCorrected; //(neg. Tolerance shifted) IMD_Info_General: Byte 0, 1
    IMDStatus IMDStatus;
} IMDValues;

void initIMD(IMDValues* IMD, int hz);

void updateIMD(void* IMDValues);

int getIMDStatus(void* IMDValues, IMDStatus status);

float getIsoResistanceNeg(void* IMDValues, float IsoResistanceNeg);

float getIsoResistancePos(void* IMDValues, float IsoResistancePos);

float getIsoResistanceCorrected(void* IMDValues, float IsoResistancePos);



#endif // RENSSELAERMOTORSPORT_IMD_H