#include "../../../Inc/Systems/External/Imd.h"
#include "../../../Inc/Systems/ExternalSystem.h"
#include "../../../Inc/Utils/Updateable.h"
#include "../../../Inc/Utils/Common.h"

void initIMD(IMDValues* IMD, int hz) {
    initExternalSystem(&IMD->base, "IMD", hz, EXTERNAL, NULL, NULL, IMD);
    IMD->base.system.updateable.update = updateIMD;
    // IMD->base.system.updateable.status = getIMDStatus; // FIXME: Update getIMDStatus
    IMD->IMDStatus = IMD_OK;
    IMD->IsoResistanceNeg = 0;
    IMD->IsoResistancePos = 0;
    IMD->IsoResistanceCorrected = 0;
}

int updateIMD(Updateable* updateable) {
    //TODO: Implement logic to determine IMD status
    /*if (something){
    set IMD_ERROR;
    } else if (something){
        set IMD_START;
    } else {*/
    //  set IMD_OK
    return _FAILURE;
}

int getIMDStatus(void* imdValues, IMDStatus status) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IMDStatus;
}

float getIsoResistanceNeg (void* imdValues) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IsoResistanceNeg;
}

float getIsoResistancePos (void* imdValues) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IsoResistancePos;
}

float getIsoResistanceCorrected (void* imdValues) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IsoResistanceCorrected;
}

