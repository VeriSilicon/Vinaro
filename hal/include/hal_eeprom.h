/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_eeprom.h
 * @brief public head file of EEPROM driver
 */

#ifndef _HAL_EEPROM_H_
#define _HAL_EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vsd_error.h"
#include "vsd_common.h"

/** @addtogroup EEPROM
 *  EEPROM HAL API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

typedef enum EepromIdDef {
    EEPROM_ID_0,
    EEPROM_ID_1,
    EEPROM_ID_2,
    EEPROM_ID_3,
    EEPROM_ID_MAX,
} EEPROMIdDef;

/**
 * @brief Structure of EEPROM info
 */
typedef struct EepromInfo {
    const char *name; /**< EEPROM name */
    uint16_t page_size; /**< page size in byte */
    uint16_t device_size; /**< device size in byte */
} EepromInfo;

/**
 * @brief This struct define EEPROM device
 */
typedef struct EepromDevice {
    BusDevice bus_device; /**< Bus device settings */
    BusConfig bus_config; /**< Configuration of bus */
    const void *ops; /**< EEPROM operations */
    const EepromInfo *info; /**< EEPROM hardware configuration */
    uint8_t dev_id; /**< Device ID of EEPROM */
} EepromDevice;

/**
 * @brief Structure of operations for HAL of EEPROM
 */
typedef struct EepromOperations {
    int (*read)(const EepromDevice *device, uint32_t word_addr, uint8_t *data,
                uint32_t size);
    int (*write)(const EepromDevice *device, uint32_t word_addr,
                 const uint8_t *data, uint32_t size);
} EepromOperations;

/**
 * @brief Add EEPROM device instance
 * @note This API should be called by EEPROM IP driver to add initialized
 * instance to HAL, not for application usage
 * @param device The device of EEPROM will be added
 * @return Return VSD_SUCCESS for succeed, others for failure
 */
int hal_eeprom_add_dev(EepromDevice *device);

/**
 * @brief Get EEPROM device instance
 * @param port_id The hardware id of EepromDevice
 * @return Return the EEPROM instance
 */
EepromDevice *hal_eeprom_get_device(uint8_t hw_id);

/**
 * @brief EEPROM write
 *
 * @param[in]  device         the EEPROM device
 * @param[in]  word_addr      the EEPROM word address
 * @param[in]  data           the EEPROM write data
 * @param[in]  size           the EEPROM write data size in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_eeprom_write(const EepromDevice *device, uint32_t word_addr,
                     const uint8_t *data, uint32_t size);

/**
 * @brief EEPROM read
 *
 * @param[in]  device         the EEPROM device
 * @param[in]  word_addr      the EEPROM word address
 * @param[in]  data           the EEPROM read data
 * @param[in]  size           the EEPROM read data size in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_eeprom_read(const EepromDevice *device, uint32_t word_addr,
                    uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_EEPROM_H_ */
