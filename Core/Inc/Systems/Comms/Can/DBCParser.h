#ifndef RENNSSELAERMOTORSPORT_DBC_H
#define RENNSSELAERMOTORSPORT_DBC_H

#include <stdint.h>

#include "Can.h"

static const int MAX_LINE_LENGTH = 256;


/**
 * @brief Parses a single line of a DBC file and populates the CAN_MessageList structure.
 * 
 * This function parses a single line of a CAN_MessageList file and populates the provided
 * CAN_MessageList structure with messages and signals.
 *
 * @param[out] messages Pointer to the CAN_MessageList structure that will be 
 * populated with the parsed data.
 * @param[in] line Line of the DBC file to parse.
 * @return Returns 1 on _SUCCESS, or 0 on failure.
 */
int parseDbcLine(CAN_MessageList *messages, char *line);

/**
 * @brief Parses a DBC (CAN database) file and populates the DBC structure.
 *
 * This function reads the specified DBC file and parses its content, populating
 * the provided DBC structure with messages and signals.
 *
 * @param[out] CAN_MessageList Pointer to the CAN_MessageList structure that will 
 * be populated with the parsed data.
 * @param[in] filename Path to the DBC file to parse.
 * @return Returns 1 on _SUCCESS, or o on failure.
 * @note The CAN_MessageList structure should be properly initialized before calling this function.
 */
int parseDbcFile(CAN_MessageList *messages, const unsigned char *dbc_contents);

/**
 * @brief Prints the contents of a CAN message list (aka DBC file).
 *
 * This function prints the contents of a CAN message list (aka DBC file) to the
 * console.
 *
 * @param[in] messages Pointer to the CAN message list to print.
 * return void
*/
void print_CAN_MessageList(const CAN_MessageList *messages);

#endif  // RENNSSELAERMOTORSPORT_DBC_H
