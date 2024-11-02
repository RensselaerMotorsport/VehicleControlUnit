#include <stdio.h>
#include <string.h>

#include "../../../../Inc/Systems/External/Can/Can.h"

int testParseCanData(const char* fn, int expected, const char* testName) {
    CanMessage canMsg;

    int worked = parseCanData(&canMsg, fn);
    if (!worked) {
        printf("%s parseCanData function broke.\n", testName);
        return 0;
    }
    
    unsigned int result = canMsg.data;
    if (result == expected) {
        printf("%s passed.\n", testName);
        return 0;
    } else {
        printf("%s failed. Expected %d, got %d.\n", testName, expected, result);
        return 1;
    }
}

int testExtractSignalValue(Signal* sig, unsigned int canData, float expected, const char* testName) {
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
    result += testParseCanData("Tests/Systems/External/Can/CanDataTest.txt",
                               1, "Parse CAN Data Test 1");
    result += testParseCanData("Tests/Systems/External/Can/CanDataTest1.txt",
                               0, "Parse CAN Data Test 2");

    // Test cases for extractSignalValue
    Signal testSignal;
    strcpy(testSignal.name, "TestSignal");
    testSignal.start_bit = 0;
    testSignal.length = 16;
    testSignal.scale = 1.0f;
    testSignal.offset = 0.0f;
    testSignal.min = 0.0f;
    testSignal.max = 65535.0f;
    testSignal.endian = ENDIAN_LITTLE;

    result += testExtractSignalValue(&testSignal, 0xFF, 255.0f,
                 "Extract Signal Value Test 1 (255)");
    result += testExtractSignalValue(&testSignal, 0x00, 0.0f,
                 "Extract Signal Value Test 2 (0)");
    result += testExtractSignalValue(&testSignal, 0x190, 400.0f,
                 "Extract Signal Value Test 3 (400)");
    result += testExtractSignalValue(&testSignal, 0xFFFF, 65535.0f,
                 "Extract Signal Value Test 4 (Max 16-bit value)");
    result += testExtractSignalValue(&testSignal, 0x7A, 122.0f,
                 "Extract Signal Value Test 5 (122)");
    result += testExtractSignalValue(&testSignal, 0x1C8, 456.0f,
                 "Extract Signal Value Test 6 (456)");
    result += testExtractSignalValue(&testSignal, 0x12C, 300.0f,
                 "Extract Signal Value Test 7 (300)");

    // Testing with scaling and offset adjustments
    testSignal.scale = 0.5f;
    testSignal.offset = 10.0f;
    result += testExtractSignalValue(&testSignal, 0x80, 50.0f,
                 "Extract Signal Value Test 8 (Scale 0.5, Offset 10)");
    result += testExtractSignalValue(&testSignal, 0x258, 150.0f,
                 "Extract Signal Value Test 9 (Scale 0.5, Offset 10)");

    // // Reset scale and offset for further tests
    // testSignal.scale = 1.0f;
    // testSignal.offset = 0.0f;
    //
    // // Testing larger values within the signal's bit-length limits
    // result += testExtractSignalValue(&testSignal, 0x1F4, 500.0f,
    //              "Extract Signal Value Test 10 (500)");
    // result += testExtractSignalValue(&testSignal, 0x3E8, 1000.0f,
    //              "Extract Signal Value Test 11 (1000)");
    // result += testExtractSignalValue(&testSignal, 0x7D0, 2000.0f,
    //              "Extract Signal Value Test 12 (2000)");

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
