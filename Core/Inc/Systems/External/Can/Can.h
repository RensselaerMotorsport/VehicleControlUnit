#ifndef RENSSELAERMOTORSPORT_CAN_H
#define RENSSELAERMOTORSPORT_CAN_H

#include "DBCParser.h"

typedef struct {
    int messageId;
    unsigned int data;  // CAN message data
    int dataLength;   // Length of the data in bytes
} CanMessage;

/**
 * @brief Populates can message based on input data
 * 
 * @param canMsg The structure that will be populated.
 * @param fn The file name
 * @return Weather or not the data was parsed
 */
int parseCanData(CanMessage* canMsg, const char* fn);

/**
 * @brief Gets the value from can data
 *
 * @param sig The signal to get the factors from.
 * @param canData The data to extract to value from
 */ 
float extractSignalValue(Signal* sig, const unsigned int* canData);

#endif // RENSSELAERMOTORSPORT_CAN_H
