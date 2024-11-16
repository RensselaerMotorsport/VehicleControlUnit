#include <stdio.h>
#include <string.h>

#include "../../../../Inc/Systems/External/Can/Can.h"

int testParseCanData(const char* fileName, const unsigned char* expected,
                     const char* testName) {
    CanMessage canMsg;

    // Parse the CAN data
    if (!parseCanData(&canMsg, fileName)) {
        printf("%s: parseCanData function failed.\n", testName);
        return 1;
    }

    // Compare the parsed data to the expected data
    if (strncmp((const char*)canMsg.data, expected, canMsg.dataLength) == 0) {
        printf("%s passed.\n", testName);
        return 0;
    } else {
        printf("%s failed.\nExpected: %s\nGot: %s\n", testName, expected, canMsg.data);
        return 1;
    }
}

int testExtractSignalValue(Signal* sig, unsigned int canData, float expected,
                           const char* testName) {
    float result = extractSignalValue(sig, &canData);

    if (result == expected) {
        printf("%s passed.\n", testName);
        return 0;
    } else {
        printf("%s failed. Expected %.2f, got %.2f.\n", testName, expected, result);
        return 1;
    }
}

int main() {
    int result = 0;

    // Test cases for parseCanData
    const unsigned char* expectedData1 = "0100000000000000";
    result += testParseCanData("Tests/Systems/External/Can/CanDataTest.txt",
                               expectedData1, "Parse CAN Data Test: General");
    const unsigned char* expectedData0 = "0000000000000000";
    result += testParseCanData("Tests/Systems/External/Can/CanDataTest1.txt",
                               expectedData0, "Parse CAN Data Test: General 2");

    // Base signal configuration
    Signal testSignal;
    strcpy(testSignal.name, "TestSignal");
    testSignal.start_bit = 0;
    testSignal.length = 16;
    testSignal.scale = 1.0f;
    testSignal.offset = 0.0f;
    testSignal.min = -10.0f;
    testSignal.max = 65535.0f;
    testSignal.endian = ENDIAN_LITTLE;

    // 1. Edge case - minimum value
    result += testExtractSignalValue(&testSignal, 0x00, 0.0f,
                 "Extract Signal Value Test: Minimum Value");
    // 2. Edge case - maximum 16-bit unsigned value
    result += testExtractSignalValue(&testSignal, 0xFFFF, 65535.0f,
                 "Extract Signal Value Test: Max 16-bit Value");
    // 3. Mid-range value
    result += testExtractSignalValue(&testSignal, 0x190, 400.0f,
                 "Extract Signal Value Test: Mid-range (400)");
    // 4. Scaling and offset adjustments
    testSignal.scale = 0.5f;
    testSignal.offset = 10.0f;
    result += testExtractSignalValue(&testSignal, 0x80, 74.0f,
                 "Extract Signal Value Test: Scale 0.5, Offset 10");
    // 5. Big endian test with a typical value
    testSignal.endian = ENDIAN_BIG;
    testSignal.scale = 1.0f;
    testSignal.offset = 0.0f;
    result += testExtractSignalValue(&testSignal, 0x1234, 4660.0f,
                 "Extract Signal Value Test: Big Endian");
    // 6. Signed value with sign extension
    testSignal.isSigned = 's';
    testSignal.length = 8;  // Only 8-bit length to test sign extension
    result += testExtractSignalValue(&testSignal, 0xFF, -1.0f,
                 "Extract Signal Value Test: Signed -1");
    // 7. Clamp to min and max - below range
    result += testExtractSignalValue(&testSignal, 0xF5, -10.0f,
                 "Extract Signal Value Test: Clamp Min -10");
    // 8. Clamp to min and max - above range
    testSignal.isSigned = 'u';
    testSignal.length = 16;
    testSignal.max = 5000.0f;
    result += testExtractSignalValue(&testSignal, 0xFFFF, 5000.0f,
                 "Extract Signal Value Test: Clamp Max 5000");

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
