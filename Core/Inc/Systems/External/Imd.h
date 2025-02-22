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

/**
 * @brief Initializes the IMD with the specified frequency.
 * 
 * @param IMD Pointer to the IMDValues structure to initialize.
 * @param hz Frequency in hertz at which the IMD should operate.
 */
void initIMD(IMDValues* IMD, int hz);

/**
 * @brief Updates the IMD values.
 * 
 * @param IMDValues Pointer to the IMD values that need to be updated.
 */
void updateIMD(void* IMDValues);

/**
 * FIXME: Update this function to use Updateable ptr
 * @brief Gets the status of the IMD.
 *
 * @param IMDValues A pointer to the structure IMDValues.
 * @param status An IMDStatus variable to store the retrieved status.
 * @return An integer representing the status of IMD.
 */
int getIMDStatus(void* IMDValues, IMDStatus status);

/**
 * @brief Retrieves the negative isolation resistance value from the given IMD values.
 *
 * @param IMDValues Pointer to the structure containing IMD values.
 * @return The negative isolation resistance value.
 */
float getIsoResistanceNeg(void* IMDValues);

/**
 * @brief Retrieves the positive isolation resistance value from the IMD values.
 *
 * @param IMDValues Pointer to the structure containing IMD values.
 * @return The positive isolation resistance value.
 */
float getIsoResistancePos(void* IMDValues);

/**
 * @brief Retrieves the positive isolation resistance value from the IMD values.
 *
 * @param IMDValues Pointer to the structure containing IMD values.
 * @return The corrected isolation resistance value.
 */
float getIsoResistanceCorrected(void* IMDValues);



#endif // RENSSELAERMOTORSPORT_IMD_H
