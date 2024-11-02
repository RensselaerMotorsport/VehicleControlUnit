#ifndef RENNSSELAERMOTORSPORT_DBC_H
#define RENNSSELAERMOTORSPORT_DBC_H

#include <stdint.h>

#define MAX_SIGNALS 64
#define MAX_MESSAGES 256
#define MAX_SIGNAL_NAME_LENGTH 64
#define MAX_MESSAGE_NAME_LENGTH 64
#define MAX_NODE_NAME_LENGTH 64
#define MAX_UNIT_NAME_LENGTH 64

static const int MAX_LINE_LENGTH = 256;

typedef enum {
    ENDIAN_LITTLE = 0,
    ENDIAN_BIG = 1,
} Endianness;

typedef struct {
    char name[MAX_SIGNAL_NAME_LENGTH];
    int start_bit;
    int length;
    int endian;
    char isSigned;
    float scale;
    float offset;
    float min;
    float max;
    char unit[MAX_UNIT_NAME_LENGTH];
    char reciever[MAX_NODE_NAME_LENGTH];
} Signal;

typedef struct {
    int id;
    char name[MAX_MESSAGE_NAME_LENGTH];
    int dlc;  // Data Length Code
    char sender[MAX_NODE_NAME_LENGTH];
    Signal signals[MAX_SIGNALS];
    int signal_count;
} Message;

typedef struct {
    Message messages[MAX_MESSAGES];
    int messageCount;
} DBC;

/**
 * @brief Retrieves the name of a given signal.
 *
 * This function returns a pointer to the name string of the specified signal.
 *
 * @param[in] sig Pointer to the Signal structure from which to get the name.
 * @return Pointer to a character string containing the signal name.
 * @note The returned pointer references memory within the Signal structure; 
 * do not modify or free it.
 */
char* getSignalName(Signal* sig);

/**
 * @brief Retrieves the array of signals associated with a message.
 *
 * This function returns a pointer to the array of Signal structures that are 
 * associated with the given message.
 *
 * @param[in] msg Pointer to the Message structure from which to retrieve the signals.
 * @return Pointer to the first element of an array of Signal structures.
 * @note The number of signals can be obtained from the Message structure's 
 * signal count member.
 */
Signal* getSignals(Message* msg);

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
 * @brief Retrieves the data payload of a CAN message.
 *
 * This function returns a pointer to the data payload of the specified message.
 *
 * @param[in] msg Pointer to the Message structure from which to retrieve the data.
 * @return Pointer to an array of uint8_t containing the message data.
 * @note The length of the data can be obtained from the Message structure's data
 * length member.
 */
uint8_t* getMessageData(Message* msg);

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
