/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_battery.h
 * @brief Battery HAL interface
 * @author Yuan Fang <yuan.fang@verisilicon.com>
 */

#ifndef _HAL_BAT_H
#define _HAL_BAT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BATTERY
 *  Battery hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include <vsd_common.h>

struct BatteryDev;

/**
 * @brief Structure of operations for HAL of Battery.
 */
typedef struct BatteryOperations {
    int (*init)(void);
    int (*read_capacity)(uint32_t *value);
    int (*read_level)(uint8_t *value);
    int (*deinit)(struct BatteryDev *device);
    int (*reset)(struct BatteryDev *device); /**< soft reset*/
    int (*read_reg)(struct BatteryDev *device, uint8_t reg, uint8_t *value,
                    uint8_t size);
    int (*write_reg)(struct BatteryDev *device, uint8_t reg, uint8_t *value,
                     uint8_t size);
} BatteryOperations;

/**
 * @brief Battery context in driver
 */
typedef struct {
    uint16_t volt; /**< voltage */
    uint16_t curr; /**< current */
    uint16_t temp; /**< temperature; */
    uint16_t soc; /**< stateofcharge */
    uint16_t soh; /**< stateofhealth */
    uint16_t tte; /**< timetoempty */
    uint16_t cc; /**< cyclecount */
    uint16_t avg_volt; /**< avg_voltage */
    uint16_t avg_temp; /**< avg_temperature */
    uint16_t avg_curr; /**< avg_current */
    uint16_t rm; /**< remainingcapacity */
    uint32_t fcc; /**< fullchargecapacity */
} BatteryContext;

/**
 * @brief Device hal handle for battery.
 * */
typedef struct BatteryDev {
    uint32_t capacity; /**< battery capacity */
    uint8_t level; /**< battery level in percentage */
    BusDevice bus_device;
    BusConfig bus_config;
    BatteryContext *ctx;
    BatteryOperations opers;
} BatteryDev;

/**
 * @brief Create battery HAL
 * @param ops The ops of battery driver
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_battery_create(const BatteryOperations *ops);

/**
 * @brief Battery init.
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_battery_init(void);

/**
 * @brief Read Battery capacity.
 * @param bat Battery device.
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_battery_read_capacity(BatteryDev *bat);

/**
 * @brief Read Battery level.
 * @param bat Battery device.
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_battery_read_level(BatteryDev *bat);

/** @} */

#endif // _HAL_BAT_H
