#ifndef RENSSELAERMOTORSPORT_CAN_H
#define RENSSELAERMOTORSPORT_CAN_H

#include "DBCParser.h"

#define MAX_CAN_DATA_LENGTH 16

typedef struct {
    int messageId;
    unsigned char dataLength;
    unsigned char data[MAX_CAN_DATA_LENGTH];
} CanMessage;

/**
 * @brief Parses CAN data from a file and populates a CanMessage structure.
 *
 * This function reads a CAN data file specified by `fn` and populates the
 * provided `canMsg` structure with the message ID and raw CAN data. The file
 * format is expected to contain lines with "BO_" to denote the message ID, and
 * "Raw CAN Data" for the hexadecimal CAN data. The function assumes a specific
 * line format and will exit if the file cannot be opened.
 *
 * @param canMsg Pointer to the CanMessage structure to populate with parsed data.
 * @param fn The path to the CAN data file.
 * @return int Returns 1 if parsing is successful.
 *
 * @note The function exits the program if the file cannot be opened.
 */
int parseCanData(CanMessage* canMsg, const char* fn);

/**
 * @brief Extracts the physical signal value from CAN data.
 *
 * This function extracts a signal value from raw CAN data, converts it to a
 * physical value using scaling and offset, and clamps it within the specified
 * minimum and maximum bounds. The extraction process depends on the signal's
 * endian type and whether it is signed or unsigned.
 *
 * @param sig The signal to get the factors from.
 * @param canData The data to extract to value from
 */
float extractSignalValue(Signal* sig, const unsigned char* canData);

#endif // RENSSELAERMOTORSPORT_CAN_H
