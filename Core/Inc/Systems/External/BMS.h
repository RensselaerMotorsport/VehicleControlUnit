#ifndef RENNSSELAERMOTORSPORT_BMS_H
#define RENNSSELAERMOTORSPORT_BMS_H

#include <stdint.h>
#include <stdbool.h>

#include "../ExternalSystem.h"
#include "../Comms/Can/Can.h"
#include "../../Files/orion_canbus_dbc.h"

// New CAN data flags
#define BMS_NEW_STATUS_0X6B0 (1u << 0)
#define BMS_NEW_STATUS_0X6B1 (1u << 1)
#define BMS_NEW_STATUS_0X6B2 (1u << 2)
#define BMS_NEW_STATUS_0X6B3 (1u << 3)
#define BMS_NEW_STATUS_0X6B4 (1u << 4)

typedef enum {
    DISCHARGING,
    CHARGING,
    IDLE,
} BmsChargeStatus;

typedef struct {
    ExternalSystem base;

    // Bms Data
    float packVoltage;
    float packCurrent;
    float stateOfCharge;
    float cellVoltageMin;
    float cellVoltageMax;
    float cellTemperatureMin;
    float cellTemperatureMax;

    BmsChargeStatus chargeStatus;

    struct {
            MSGID_0X6B0_t status_0x6B0;   // Pack current / voltage / DCL
            MSGID_0X6B1_t status_0x6B1;   // SOC / amp-hours / limits
            MSGID_0X6B2_t status_0x6B2;   // Cell temperatures
            MSGID_0X6B3_t status_0x6B3;   // Cell voltages
            MSGID_0X6B4_t status_0x6B4;   // Pack voltage limits

            uint8_t  new_data_flags;
            uint32_t last_update;
            uint32_t message_count;
        } can_data;
} Bms;

/**
 * @brief Initializes the BMS system.
 *
 * @param bms Pointer to BMS instance.
 * @param hz Update rate in Hz.
 */
void initBms(Bms* bms, int hz);

/**
 * @brief Periodic BMS update (runs in scheduler context).
 *
 * @param external ExternalSystem pointer.
 * @return int _SUCCESS or _FAILURE.
 */
int updateBms(ExternalSystem* external);

/**
 * @brief Heartbeat / freshness check.
 *
 * @param self Pointer to BMS instance.
 * @return int _SUCCESS or _FAILURE.
 */
int checkBmsHeartbeat(void* self);

/**
 * @brief CAN handler called by CANCommsSystem routing.
 *
 * @param system Pointer to BMS instance.
 * @param message Parsed CAN message.
 */
void bmsCanHandler(void* system, CAN_Message* message);

/**
 * @brief Returns 1 if new CAN data is available.
 *
 * @param bms Pointer to BMS instance.
 * @return int
 */
int hasNewBmsCANData(Bms* bms);

/**
 * @brief Returns ms since last BMS CAN update.
 *
 * @param bms Pointer to BMS instance.
 * @return uint32_t
 */
uint32_t getBmsDataAge(Bms* bms);

/**
 * @brief Returns 1 if BMS CAN data is stale.
 *
 * @param bms Pointer to BMS instance.
 * @param timeout_ms Timeout in ms.
 * @return int
 */
int isBmsCANDataStale(Bms* bms, uint32_t timeout_ms);

/**
 * @brief Clears new-data flags.
 *
 * @param bms Pointer to BMS instance.
 */
void clearBmsNewDataFlags(Bms* bms);

#endif // RENNSSELAERMOTORSPORT_BMS_H
