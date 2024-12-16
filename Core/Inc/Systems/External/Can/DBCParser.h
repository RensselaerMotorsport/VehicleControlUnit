#ifndef RENNSSELAERMOTORSPORT_DBC_H
#define RENNSSELAERMOTORSPORT_DBC_H

#include <stdint.h>

#include "Message.h"

static const int MAX_LINE_LENGTH = 256;

typedef enum {
    ENDIAN_LITTLE = 0,
    ENDIAN_BIG = 1,
} Endianness;

typedef struct {
    // FIXME: Messages can start in the thousands maybe need a key pair here
    Message messages[MAX_MESSAGES];
    int messageCount;
} DBC;

/**
 * @brief Parses a DBC (CAN database) file and populates the DBC structure.
 *
 * This function reads the specified DBC file and parses its content, populating
 * the provided DBC structure with messages and signals.
 *
 * @param[out] dbc Pointer to the DBC structure that will be populated with the
 * parsed data.
 * @param[in] filename Path to the DBC file to parse.
 * @return Returns 1 on success, or o on failure.
 * @note The DBC structure should be properly initialized before calling this function.
 */
int parseDbcFile(DBC* dbc, const char *filename);

/**
 * @brief Retrieves the array of messages from a DBC structure.
 *
 * This function returns a pointer to the array of Message structures contained
 * within the specified DBC structure.
 *
 * @param[in] dbc Pointer to the DBC structure from which to retrieve the messages.
 * @return Pointer to the first element of an array of Message structures.
 * @note The number of messages can be obtained from the DBC structure's message
 * count member.
 */
Message* getDbcMessages(DBC* dbc);

/**
 * @brief Prints the contents of a DBC structure to the standard output.
 *
 * This function outputs the details of the DBC structure, including messages
 * and signals, to the console.
 *
 * @param[in] dbc Pointer to the DBC structure to be printed.
 * @return None.
 * @note This function is mainly for debugging and visualization purposes.
 */
void printDbc(const DBC *dbc);

#endif  // RENNSSELAERMOTORSPORT_DBC_H
