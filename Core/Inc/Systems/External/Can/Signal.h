#ifndef RENNSSELAERMOTORSPORT_SIGNAL_H
#define RENNSSELAERMOTORSPORT_SIGNAL_H

#define MAX_SIGNALS 64
#define MAX_SIGNAL_NAME_LENGTH 64
#define MAX_UNIT_NAME_LENGTH 64
#define MAX_NODE_NAME_LENGTH 64

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
 * @brief Parses the CAN data with the given signal.
 *
 * @param sig The signal parsed from the DBC that is used to decode the data.
 * @param canData Hexadecimal string representing can data. (w/o 0x)
 * @return float Value from data and signal.
 */
float extractSignalValue(Signal* sig, const unsigned char* canData);

#endif  // RENNSSELAERMOTORSPORT_SIGNAL_H
