#include "../../../../Inc/Systems/External/Can/Signal.h"
#include "../../../../Inc/Systems/External/Can/Can.h"
#include "../../../../Inc/Systems/PrintHelpers.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* getSignalName(Signal* sig) {
    return sig->name;
}

/**
 * @brief Gets the bits corresponding to the value to be extracted.
 *
 * @param data Full hexadecimal can data. (w/o 0x)
 * @param startBit The start bit from the 
 * @param len Number of bits in value
 */ 
const unsigned char* extractSignalBytes(
    const unsigned char* data, int startBit, int len
) {
    static unsigned char result[MAX_CAN_DATA_LENGTH];
    memset(result, 0, MAX_CAN_DATA_LENGTH);

    int posAfterData = 7;
    int bitOffset = (startBit - posAfterData);
    int startByte = bitOffset / 8;
    int endByte = (bitOffset + len - 1) / 8;
    int resultIndex = 0;

    // Copy relevant bytes to the result array
    for (int i = startByte * 2; i <= endByte * 2; i+=2) {
        result[resultIndex++] = data[i];
        result[resultIndex++] = data[i+1];
    }

    return result;
}

/**
 * @brief Flips the bits from little-endian to big-endian
 *
 * @param littleEndianHex The hex value in little-endian format.
 * @param numBytes Number of bytes in hex
 * @return const unsigned char* Hex in big-endian format.
 */
const unsigned char* charToBigEndian(const unsigned char *hex, int len) {
    if (len % 8 != 0) {
        printf(
            ANSI_COLOR_RED
            "Error: Byte Length undivisable by 8. Unable to flip bytes.\n"
            ANSI_COLOR_RESET
        );
        return hex;
    }

    int numBytes = len / 8;
    static unsigned char result[MAX_CAN_DATA_LENGTH];

    for (int i = 0; i < numBytes; i++) {
        // Reverse bytes
        result[i * 2] = hex[(numBytes - 1 - i) * 2];
        result[i * 2 + 1] = hex[(numBytes - 1 - i) * 2 + 1];
    }

    result[numBytes * 2] = '\0';

    return result;
}

/**
 * @brief Applies sign extension to a raw value if the signal is signed.
 *
 * @param rawValue The extracted raw signal value.
 * @param sig The signal configuration, specifying if the signal is signed.
 * @return int64_t The signed (or unsigned) interpreted value.
 */
int64_t applySignExtension(uint64_t rawValue, const Signal* sig) {
    if (sig->isSigned == 's') {
        // Check the most significant bit for sign extension
        if (rawValue & (1ULL << (sig->length - 1))) {
            return (int64_t)(rawValue | (~0ULL << sig->length));
        }
    }
    return (int64_t)rawValue;
}
float extractSignalValue(Signal* sig, const unsigned char* canData) {
    const unsigned char* charData = extractSignalBytes(
        canData, sig->start_bit, sig->length
    );

    // Flip bits if little-endian
    // NOTE: This may not be how to handle endianess
    if (sig->endian == ENDIAN_LITTLE) {
        charData = charToBigEndian(charData, sig->length);
    }

    // FIXME: Figure out how to convent with an unsigned char
    uint64_t rawValue = strtol((const char*)charData, NULL, 16);

    // Apply sign extension if the signal is signed
    int64_t signedValue = applySignExtension(rawValue, sig);

    // Convert to physical value using scale and offset
    float physicalValue = (float)signedValue * sig->scale + sig->offset;

    // Clamp to minimum and maximum defined in the signal
    if (physicalValue < sig->min) physicalValue = sig->min;
    if (physicalValue > sig->max) physicalValue = sig->max;

    return physicalValue;
}
