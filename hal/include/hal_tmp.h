/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_tmp.h
 * @brief public head file of temperature HAL
 */
#pragma once

/** @addtogroup TEMPERATURE
 *  Temperature Sensor HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include "vsd_common.h"
#include "hal_sensor_common.h"
#include "hal_i2c.h"

/**
 * @enum TmpModeEnum
 * @brief Define the work mode of tmp sensor
 */
typedef enum TmpModeEnum {
    TMP_SHUTDOWN = 0, /**<  Shut down mode */
    TMP_ONESHOT, /**< One shot mode */
    TMP_CONVERSION_AVG, /**< Conversion average mode */
    TMP_CONVERSION_CONT, /**< Conversion continues mode */
} TmpModeEnum;

/**
 * @enum TmpCapability
 * @brief Define the capability of tmp sensor
 */
enum TmpCapability {
    /** Supports shut down mode */
    TMP_CAP_SHUTDOWN = (0x1 << TMP_SHUTDOWN),
    /** Supports one shot mode */
    TMP_CAP_ONESHOT = (0x1 << TMP_ONESHOT),
    /** Supports conversion average mode */
    TMP_CAP_CONV_AVG = (0x1 << TMP_CONVERSION_AVG),
    /** Supports conversion continues mode */
    TMP_CAP_CONV_CONT = (0x1 << TMP_CONVERSION_CONT),
};

/**
 * @brief structure of temperature device
 */
typedef struct TmpDevice {
    BusDevice bus_device; /**< Bus device settings */
    BusConfig bus_config; /**< Configuration of bus */
    uint32_t hw_capability; /**< Hardware capability of sensor */
    /** Capability of senosr, initialized by user on board.c, will be updated
     * per hw_capability in driver, then it's the finial capability which can be
     * used by application */
    uint32_t capability;
    int (*init)(struct TmpDevice *device); /**< init device */
    int (*destroy)(struct TmpDevice *device); /**< destroy device */
    int (*reset)(struct TmpDevice *device); /**< soft reset device */
    /** power on/off device */
    int (*power)(struct TmpDevice *device, bool enable);
    /** read temperature value in 0.01 Celsius */
    int (*read)(struct TmpDevice *device, uint16_t *tmp);
    int (*set_mode)(struct TmpDevice *device, uint32_t mode); /**< set mode */
    /** Read register */
    int (*read_reg)(struct TmpDevice *device, uint16_t reg, uint8_t *value,
                    uint16_t size);
    /** Write register */
    int (*write_reg)(struct TmpDevice *device, uint16_t reg, uint8_t *value,
                     uint16_t size);
} TmpDevice;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief TMP hal API to init
 * @param device The TMP sensor device
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_init(TmpDevice *device);

/**
 * @brief TMP hal API to destroy
 * @param device The TMP sensor device
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_destroy(TmpDevice *device);

/**
 * @brief TMP hal API to control power
 *
 * @param device The TMP sensor device
 * @param enable Expected power state, true for ON
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_power(TmpDevice *device, bool enable);

/**
 * @brief TMP hal API to reset
 * @param device The TMP sensor device
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_reset(TmpDevice *device);

/**
 * @brief TMP hal API to set mode
 * @note For one shot mode, it always needs set to TMP_ONESHOT before reading
 * Flow:
 * - set to ONESHOT, delay, reading temperature;
 * - set to ONESHOT, delay, reading temperature;
 * @param device The TMP sensor device
 * @param mode The mode to be set @see TmpModeEnum
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_set_mode(TmpDevice *device, uint32_t mode);

/**
 * @brief TMP hal API to read temperature
 * @param device The TMP sensor device
 * @param value The value of temperature read
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_read(TmpDevice *device, uint16_t *value);

/**
 * @brief TMP hal API to read register
 * @param device The TMP sensor device
 * @param reg The register address to read
 * @param value The value read from register
 * @param size The size to read
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_read_reg(TmpDevice *device, uint16_t reg, uint8_t *value,
                     uint16_t size);

/**
 * @brief TMP hal API to write register
 * @param device The TMP sensor device
 * @param reg The register address to write
 * @param value The value write to register
 * @param size The size to write
 * @return Error code, VSD_SUCCESS for succeed, others for failure
 */
int hal_tmp_write_reg(TmpDevice *device, uint16_t reg, uint8_t *value,
                      uint16_t size);

/** @} */

#ifdef __cplusplus
}
#endif
