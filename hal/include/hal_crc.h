/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_crc.h
 * @brief public head file of CRC driver
 */

#ifndef _HAL_CRC_H_
#define _HAL_CRC_H_

#include <stdio.h>
#include <stdbool.h>
#include "vsd_error.h"
#include "vsd_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CRC
 *  Cyclic Redundancy Checkhal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @brief CRC polynomial
 * */
typedef enum {
    CRC_POLYNOMIAL_CRC_8 = 1, /**< 8-bit CRC-8 */
    CRC_POLYNOMIAL_CRC_16, /**<16-bit CRC-16 */
    CRC_POLYNOMIAL_CRC_CCITT, /**<16-bit CRC-CCITT */
    CRC_POLYNOMIAL_CRC_32, /**<32-bit CRC-32 */
    CRC_POLYNOMIAL_CRC_32C, /**<32-bit CRC-32C */
} CrcPolynomial;

/**
 * @enum CrcCapability
 * @brief Define the capability of crc Polynomial
 */
enum CrcCapability {
    CRC_CAP_CRC_8     = (0x1 << CRC_POLYNOMIAL_CRC_8),
    CRC_CAP_CRC_16    = (0x1 << CRC_POLYNOMIAL_CRC_16),
    CRC_CAP_CRC_CCITT = (0x1 << CRC_POLYNOMIAL_CRC_CCITT),
    CRC_CAP_CRC_32    = (0x1 << CRC_POLYNOMIAL_CRC_32),
    CRC_CAP_CRC_32C   = (0x1 << CRC_POLYNOMIAL_CRC_32C),
};

/**
 * @brief CRC Calculation Switching (LMS)
 * */
typedef enum {
    CRC_LMS_LSB, /**<Generates CRC for LSB first */
    CRC_LMS_MSB, /**<Generates CRC for MSB first */
} CrcCalSwitch;

/**
 * @brief Structure for CRC inputs
 * */
typedef struct CrcInput {
    void *p_buffer;
    uint32_t buffer_length;
    uint8_t poly; /**< @see CrcPolynomial */
    uint8_t cal_switch; /**< @see CrcCalSwitch */
} CrcInput;

/**
 * @brief Structure of CRC hardware configuration
 */
typedef struct CrcHwConfig {
    uint32_t addr; /**< Base address of CRC device */
    uint8_t clk_id; /**< clk id */
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */
} CrcHwConfig;

/**
 * @brief This struct define CRC
 */
typedef struct CrcDevice {
    const CrcHwConfig *hw_config;
    const void *ops; /**< crc operations */
    uint32_t capability; /**< crc capability */
    uint8_t dev_prop; /**< CRC device type(HW/SW), @see DeviceProperty */
} CrcDevice;

/**
 * @brief Structure of operations for HAL of CRC
 */
typedef struct CrcOperations {
    int (*init)(const CrcDevice *crc);
    int (*enable)(const CrcDevice *crc, bool enable);
    int (*calculate)(const CrcDevice *crc, CrcInput *crc_input,
                     uint32_t *crc_result);
    int (*get_result)(const CrcDevice *CrcDevice, uint32_t *crc_result);
} CrcOperations;

/**
 * @brief Create CRC instances
 * @param crc handle of crc device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_crc_create(const CrcDevice *crc);

/**
 * @brief Get crc device instance, Only support one crc
 * @return Return the crc instance
 */
CrcDevice *hal_crc_get_device(void);

/**
 * @brief Initialize the crc
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_crc_init(void);

/**
 * @brief Enable/Disable crc
 * @param enable true or false
 * @return Return result
 * @retval VSD_SUCCESS on success, error code on failure.
 */
int hal_crc_enable(bool enable);

/**
 * @brief compute the CRC value
 * @param[in] crc_input  A pointer to structure for CRC inputs
 * @param[out] crc_result The calculated value of the CRC calculation.
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_crc_calculate(CrcInput *crc_input, uint32_t *crc_result);

/**
 * @brief get CRC compute value
 * @param crc_result The calculated value of the CRC calculation.
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_crc_get_result(uint32_t *crc_result);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /*_HAL_CRC_H_ */
