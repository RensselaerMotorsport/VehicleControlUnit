#ifndef RENNSSELAERMOTORSPORT_MESSAGE_H
#define RENNSSELAERMOTORSPORT_MESSAGE_H

#include "Signal.h"

#define MAX_MESSAGES 256
#define MAX_MESSAGE_NAME_LENGTH 64

typedef struct {
    int id;
    char name[MAX_MESSAGE_NAME_LENGTH];
    int dlc;  // Data Length Code
    char sender[MAX_NODE_NAME_LENGTH];
    Signal signals[MAX_SIGNALS];
    int signal_count;
} Message;

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

#endif  // RENNSSELAERMOTORSPORT_MESSAGE_H
