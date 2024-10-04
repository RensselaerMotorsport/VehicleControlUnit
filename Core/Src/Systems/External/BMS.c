#include "../../../Inc/Sensors/CANSensors/BMS.h"

#include <stdio.h>  // For printf

void initBms(Bms* bms, int hz) {
    if (bms != NULL) {
        initCANSensor(&bms->base, "Bms", hz, 0);
        // Assume initBmsData initializes with default or zero values
        initBmsData(&bms->data, 0, 0, 0, 0, 0, 0, 0, 0, false);
        bms->base.sensor.updateable.update = updateBms;
    }
}

    initCANSensor(&bms->base, "Bms", hz);
    // Assume initBmsData initializes with default or zero values
    initBmsData(&bms->data, 0, 0, 0, 0, 0, 0, 0, 0, false);
    bms->base.sensor.updateable.update = updateBms;
}

bool parseBmsSignal(Bms* bmsData, BmsSignal signal, float value) {
    switch (signal) {
        case BMS_PACK_VOLTAGE:
            bmsData->packVoltage = value;
            break;
        case BMS_PACK_CURRENT:
            bmsData->packCurrent = value;
            break;
        case BMS_STATE_OF_CHARGE:
            bmsData->stateOfCharge = value;
            break;
        case BMS_CELL_VOLTAGE_MIN:
            bmsData->cellVoltageMin = value;
            break;
        case BMS_CELL_VOLTAGE_MAX:
            bmsData->cellVoltageMax = value;
            break;
        case BMS_CELL_TEMPERATURE_MIN:
            bmsData->cellTemperatureMin = value;
            break;
        case BMS_CELL_TEMPERATURE_MAX:
            bmsData->cellTemperatureMax = value;
            break;
        case BMS_TOTAL_PACK_CAPACITY:
            bmsData->totalPackCapacity = value;
            break;
        case BMS_REMAINING_PACK_CAPACITY:
            bmsData->remainingPackCapacity = value;
            break;
        case BMS_PACK_HEALTH:
            bmsData->packHealth = value;
            break;
        case BMS_CHARGE_STATUS:
            bmsData->chargeStatus = (uint8_t) value;
            break;
        default:
            // Handle unknown signals if necessary
            return false;
    }
    return true;
}

bool bmsTransferFunction(const Message* message, Bms* bmsData) {
    // Iterate through signals and decode each one
    for (int j = 0; j < message->signal_count; j++) {
        Signal signal = message->signals[j];
        float decodedValue = ParseSignal(message, &signal);

        // Find the BMS signal type based on the signal name
        BmsSignal bmsSignalType;
        bool signalFound = false;
        for (int k = 0; k < sizeof(bmsSignalMap)/sizeof(BmsSignalMap); k++) {
            if (strcmp(signal.name, bmsSignalMap[k].signalName) == 0) {
                bmsSignalType = bmsSignalMap[k].signalType;
                signalFound = true;
                break;
            }
        }

        if (!signalFound) {
            // Handle unknown signals if necessary
            continue;
        }

        if (!parseBmsSignal(bmsData, bmsSignalType, decodedValue)) {
            return false;
        }
    }

    return true;
}

void updateBms(void* bms) {
    Bms* myBms = (Bms*) bms;
    CanData canData = data; // Assume 'data' is obtained from the CAN message

    myBms->packVoltage = canData.packVoltage;
    myBms->packCurrent = canData.packCurrent;
    myBms->stateOfCharge = canData.stateOfCharge;
    myBms->cellVoltageMin = canData.cellVoltageMin;
    myBms->cellVoltageMax = canData.cellVoltageMax;
    myBms->cellTemperatureMin = canData.cellTemperatureMin;
    myBms->cellTemperatureMax = canData.cellTemperatureMax;
    myBms->totalPackCapacity = canData.totalPackCapacity;
    myBms->remainingPackCapacity = canData.remainingPackCapacity;
    myBms->packHealth = canData.packHealth;
    myBms->chargeStatus = canData.chargeStatus;
}