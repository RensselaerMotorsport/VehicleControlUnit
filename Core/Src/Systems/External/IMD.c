#include "../../../Inc/Systems/External/IMD.h"
#include "../../../Inc/Systems/ExternalSystem.h"
// Make sure to update IMD.h as well!!!

/* Things to do
* 0. Make sure to implement everything needed from updateable type
* ---> Check Updateable.c and Updateable.h
* 1. Implement constructor
* 2. Make template for storing data from IMD
* 3. Implement getting data from IMD thru CAN
* ---> CAN needs to be implemented first
* 4. Tests, check the test folder
* 5. Figure out the rest~ you can ask for help 
*/

void initIMD(IMDValues* IMD, int hz) {
    initExternalSystem(&IMD->base, "IMD", hz, EXTERNAL);
    IMD->base.system.updateable.update = updateIMD;
    IMD->base.system.updateable.status = getIMDStatus;
    IMD->IMDStatus = IMD_OK;
    IMD->IsoResistanceNeg = 0;
    IMD->IsoResistancePos = 0;
    IMD->IsoResistanceCorrected = 0;
}

void updateIMD(void* IMDValues) {
        /*if (something){
        set IMD_ERROR;
    } else if (something){
        set IMD_START;
    } else {*/
    //  set IMD_OK
        return;
    }

int getIMDStatus(void* imdValues, IMDStatus status) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IMDStatus;
}

float getIsoResistanceNeg (void* imdValues, float IsoResistanceNeg) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IsoResistanceNeg;
}

float getIsoResistancePos (void* imdValues, float IsoResistancePos) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IsoResistancePos;
}

float getIsoResistanceCorrected (void* imdValues, float IsoResistancePos) {
    IMDValues* IMD = (IMDValues*) imdValues;
    return IMD->IsoResistanceCorrected;
}

