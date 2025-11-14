#ifndef RENNSSELAERMOTORSPORT_MESSAGE_FORMAT_H
#define RENNSSELAERMOTORSPORT_MESSAGE_FORMAT_H

#include <stdio.h>
#include <stdarg.h>

// Message types
typedef enum {
    MSG_SENSOR_VALUE,
    MSG_OUTPUT_VALUE,
    MSG_SYSTEM_STATUS,
    MSG_WARNING,
    MSG_ERROR,
    MSG_DEBUG,
    MSG_CAN_TX,
    MSG_CAN_RX,
    MSG_TIMER_STATS,
    MSG_CONFIG
} MessageType;

// Structured message function
void sendMessage(const char* sender, MessageType type, const char* format, ...);

#endif // RENNSSELAERMOTORSPORT_MESSAGE_FORMAT_H