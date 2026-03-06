#include "../../../Inc/Systems/External/BMS.h"
#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Utils/MessageFormat.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

extern uint32_t HAL_GetTick(void);

void initBms(Bms* bms, int hz) {
    if (!bms) return;

    initExternalSystem(&bms->base, "BMS", hz, e_BMS, updateBms, checkBmsHeartbeat, bms);

    bms->packVoltage = 0.0f;
    bms->packCurrent = 0.0f;
    bms->stateOfCharge = 0.0f;
    bms->cellVoltageMin = 0.0f;
    bms->cellVoltageMax = 0.0f;
    bms->cellTemperatureMin = 0.0f;
    bms->cellTemperatureMax = 0.0f;
    bms->chargeStatus = IDLE;

    memset(&bms->can_data.status_0x6B0, 0, sizeof(bms->can_data.status_0x6B0));
    memset(&bms->can_data.status_0x6B1, 0, sizeof(bms->can_data.status_0x6B1));
    memset(&bms->can_data.status_0x6B2, 0, sizeof(bms->can_data.status_0x6B2));
    memset(&bms->can_data.status_0x6B3, 0, sizeof(bms->can_data.status_0x6B3));
    memset(&bms->can_data.status_0x6B4, 0, sizeof(bms->can_data.status_0x6B4));

    bms->can_data.new_data_flags = 0;
    bms->can_data.last_update = 0;
    bms->can_data.message_count = 0;
}

int updateBms(ExternalSystem* external) {
    Bms* bms = (Bms*)external->child;

    if (bms->can_data.new_data_flags & BMS_NEW_STATUS_0X6B0) {
        bms->packCurrent = (float)bms->can_data.status_0x6B0.pack_abs_current_unsigned * 0.1f;
        bms->packVoltage = (float)bms->can_data.status_0x6B0.pack_inst_voltage * 0.1f;
    }

    if (bms->can_data.new_data_flags & BMS_NEW_STATUS_0X6B1) {
        bms->stateOfCharge = (float)bms->can_data.status_0x6B1.pack_soc * 0.01f;
    }

    if (bms->can_data.new_data_flags & BMS_NEW_STATUS_0X6B2) {
        const float highT = (float)bms->can_data.status_0x6B2.high_temperature * 0.25f;
        const float lowT  = (float)bms->can_data.status_0x6B2.low_temperature  * 0.25f;

        bms->cellTemperatureMax = highT;
        bms->cellTemperatureMin = lowT;
    }

    if (bms->can_data.new_data_flags & BMS_NEW_STATUS_0X6B3) {
        const float highV = (float)bms->can_data.status_0x6B3.high_cell_voltage * 0.0001f;
        const float lowV  = (float)bms->can_data.status_0x6B3.low_cell_voltage  * 0.0001f;

        bms->cellVoltageMax = highV;
        bms->cellVoltageMin = lowV;
    }

    if (bms->can_data.new_data_flags & BMS_NEW_STATUS_0X6B4) {
        const float maxPackV = (float)bms->can_data.status_0x6B4.maximum_pack_voltage * 0.1f;
        const float minPackV = (float)bms->can_data.status_0x6B4.minimum_pack_voltage * 0.1f;
        const float packR    = (float)bms->can_data.status_0x6B4.pack_resistance * 0.0005f;
        const float intT     = (float)bms->can_data.status_0x6B4.internal_temperature * 0.3333333333f;
    }

    if (isBmsCANDataStale(bms, 5000)) {
        printf("[BMS] ERROR: CAN data stale\r\n");
        sendMessage("BMS", MSG_ERROR, "CAN communication timeout");
        return _FAILURE;
    }

    bms->can_data.new_data_flags = 0;
    return _SUCCESS;
}

int checkBmsHeartbeat(void* self) {
    (void)self;
    return _SUCCESS;
}

static inline void bms_store_0x6B0(Bms* bms, const CAN_Message* m) {
    MSGID_0X6B0_t* s = &bms->can_data.status_0x6B0;

    for (int i = 0; i < m->template->signal_count; i++) {
        const CAN_Signal* sig = &m->signals[i];
        const char* n = sig->template->name;

        if      (strcmp(n, "Pack_Abs_Current_Unsigned") == 0) s->pack_abs_current_unsigned = (uint16_t)sig->value;
        else if (strcmp(n, "Pack_Inst_Voltage") == 0)         s->pack_inst_voltage         = (uint16_t)sig->value;
        else if (strcmp(n, "Pack_Open_Voltage") == 0)         s->pack_open_voltage         = (uint16_t)sig->value;
        else if (strcmp(n, "Pack_DCL_KW") == 0)               s->pack_dcl_kw               = (uint8_t)sig->value;
        else if (strcmp(n, "CRC_Checksum") == 0)              s->crc_checksum              = (uint8_t)sig->value;
    }

    bms->can_data.new_data_flags |= BMS_NEW_STATUS_0X6B0;
}

static inline void bms_store_0x6B1(Bms* bms, const CAN_Message* m) {
    MSGID_0X6B1_t* s = &bms->can_data.status_0x6B1;

    for (int i = 0; i < m->template->signal_count; i++) {
        const CAN_Signal* sig = &m->signals[i];
        const char* n = sig->template->name;

        if      (strcmp(n, "Pack_SOC") == 0)              s->pack_soc              = (uint16_t)sig->value;
        else if (strcmp(n, "Pack_Amphours") == 0)         s->pack_amphours         = (uint16_t)sig->value;
        else if (strcmp(n, "Pack_DCL") == 0)              s->pack_dcl              = (uint16_t)sig->value;
        else if (strcmp(n, "Current_Limits_Status") == 0) s->current_limits_status = (uint8_t)sig->value;
        else if (strcmp(n, "CRC_Checksum") == 0)          s->crc_checksum          = (uint8_t)sig->value;
    }

    bms->can_data.new_data_flags |= BMS_NEW_STATUS_0X6B1;
}

static inline void bms_store_0x6B2(Bms* bms, const CAN_Message* m) {
    MSGID_0X6B2_t* s = &bms->can_data.status_0x6B2;

    for (int i = 0; i < m->template->signal_count; i++) {
        const CAN_Signal* sig = &m->signals[i];
        const char* n = sig->template->name;

        if      (strcmp(n, "High_Temperature") == 0)    s->high_temperature    = (uint8_t)sig->value;
        else if (strcmp(n, "Low_Temperature") == 0)     s->low_temperature     = (uint8_t)sig->value;
        else if (strcmp(n, "Average_Temperature") == 0) s->average_temperature = (uint8_t)sig->value;
        else if (strcmp(n, "High_Thermistor_ID") == 0)  s->high_thermistor_id  = (uint8_t)sig->value;
        else if (strcmp(n, "Low_Thermistor_ID") == 0)   s->low_thermistor_id   = (uint8_t)sig->value;
        else if (strcmp(n, "Blank") == 0)               s->blank               = (uint8_t)sig->value;
        else if (strcmp(n, "CRC_Checksum") == 0)        s->crc_checksum        = (uint8_t)sig->value;
    }

    bms->can_data.new_data_flags |= BMS_NEW_STATUS_0X6B2;
}

static inline void bms_store_0x6B3(Bms* bms, const CAN_Message* m) {
    MSGID_0X6B3_t* s = &bms->can_data.status_0x6B3;
    int high_cell_seen = 0; // Handle duplicate "High_Cell_Voltage" in some DBCs.

    for (int i = 0; i < m->template->signal_count; i++) {
        const CAN_Signal* sig = &m->signals[i];
        const char* n = sig->template->name;

        if (strcmp(n, "High_Cell_Voltage") == 0) {
            if (!high_cell_seen) {
                s->high_cell_voltage = (uint16_t)sig->value;
                high_cell_seen = 1;
            } else {
                s->low_cell_voltage = (uint16_t)sig->value;
            }
        } else if (strcmp(n, "Low_Cell_Voltage") == 0) {
            s->low_cell_voltage = (uint16_t)sig->value;
        } else if (strcmp(n, "High_Cell_Voltage_ID") == 0) {
            s->high_cell_voltage_id = (uint8_t)sig->value;
        } else if (strcmp(n, "Low_Cell_Voltage_ID") == 0) {
            s->low_cell_voltage_id = (uint8_t)sig->value;
        } else if (strcmp(n, "Blank") == 0) {
            s->blank = (uint8_t)sig->value;
        } else if (strcmp(n, "CRC_Checksum") == 0) {
            s->crc_checksum = (uint8_t)sig->value;
        }
    }

    bms->can_data.new_data_flags |= BMS_NEW_STATUS_0X6B3;
}

static inline void bms_store_0x6B4(Bms* bms, const CAN_Message* m) {
    MSGID_0X6B4_t* s = &bms->can_data.status_0x6B4;

    for (int i = 0; i < m->template->signal_count; i++) {
        const CAN_Signal* sig = &m->signals[i];
        const char* n = sig->template->name;

        if      (strcmp(n, "Maximum_Pack_Voltage") == 0)  s->maximum_pack_voltage = (uint16_t)sig->value;
        else if (strcmp(n, "Minimum_Pack_Voltage") == 0) s->minimum_pack_voltage  = (uint16_t)sig->value;
        else if (strcmp(n, "Pack_Resistance") == 0)      s->pack_resistance       = (uint8_t)sig->value;
        else if (strcmp(n, "Blank") == 0)                s->blank                 = (uint8_t)sig->value;
        else if (strcmp(n, "Internal_Temperature") == 0) s->internal_temperature  = (uint8_t)sig->value;
        else if (strcmp(n, "CRC_Checksum") == 0)         s->crc_checksum          = (uint8_t)sig->value;
    }

    bms->can_data.new_data_flags |= BMS_NEW_STATUS_0X6B4;
}

static inline void bms_print_signals(const CAN_Message* m) {
    for (int i = 0; i < m->template->signal_count; i++) {
        const CAN_Signal* s = &m->signals[i];

        float physical_value = s->value;

        printf("[BMS SIG] ID=0x%lX %s = %f %s\r\n",
               m->header.StdId,
               s->template->name,
               physical_value,
               s->template->unit ? s->template->unit : "");

        sendMessage("BMS_DATA", MSG_SENSOR_VALUE,
                    "Signal=%s;Value=%f;Unit=%s",
                    s->template->name,
                    physical_value,
                    s->template->unit ? s->template->unit : "");
    }
}

void bmsCanHandler(void* system, CAN_Message* message) {
    Bms* bms = (Bms*)system;
    const uint32_t id = message->header.StdId;

    if (!message || !message->template) {
        printf("[BMS CAN] RX NULL tmpl id=0x%lX\r\n", id);
        return;
    }

    switch (id) {
        case MSGID_0X6B0_ID:
            bms_store_0x6B0(bms, message);
            break;

        case MSGID_0X6B1_ID:
            bms_store_0x6B1(bms, message);
            break;

        case MSGID_0X6B2_ID:
            bms_store_0x6B2(bms, message);
            break;

        case MSGID_0X6B3_ID:
            bms_store_0x6B3(bms, message);
            break;

        case MSGID_0X6B4_ID:
            bms_store_0x6B4(bms, message);
            break;

        default:
            printf("[BMS CAN] RX Unknown ID=0x%lX\r\n", id);
            break;
    }

    bms->can_data.last_update = HAL_GetTick();
    bms->can_data.message_count++;
}

int hasNewBmsCANData(Bms* bms) {
    return bms->can_data.new_data_flags != 0;
}

uint32_t getBmsDataAge(Bms* bms) {
    if (bms->can_data.last_update == 0) return UINT32_MAX;
    return HAL_GetTick() - bms->can_data.last_update;
}

int isBmsCANDataStale(Bms* bms, uint32_t timeout_ms) {
    return getBmsDataAge(bms) > timeout_ms;
}

void clearBmsNewDataFlags(Bms* bms) {
    bms->can_data.new_data_flags = 0;
}
