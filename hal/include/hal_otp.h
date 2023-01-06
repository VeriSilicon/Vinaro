/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_otp.h
 * @brief public head file of OTP HAL
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#ifndef _HAL_OTP_H
#define _HAL_OTP_H

#include "otp_layout.h"

#define EFUSE_INVALID_ADDR (-1)
#define OTP_MAX_GROUPS (2)
enum EFUSE_GROUP {
    EFUSE_GROUP0, /**< efuse group 0 */
    EFUSE_GROUP1, /**< efuse group 1 */
};

/** @addtogroup OTP
 *  One Time Program hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @brief Structure of OTP hardware configuration
 */
typedef struct OtpHwConfig {
    uint32_t base; /**< Base address of OTP device */
    uint8_t clk_id; /**< Clock id of OTP device */
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */
} OtpHwConfig;

/**
 * @brief Structure of OTP hardware
 */
typedef struct OtpDevice {
    uint8_t id; /**< OTP device id */
    const struct OtpHwConfig *config; /**< hardware configuration */
    const struct OtpOperations *ops; /**< operations of driver */
    void *ctx;
} OtpDevice;

/**
 * @brief Structure of operations for HAL of OTP
 */
typedef struct OtpOperations {
    int (*init)(OtpDevice *dev, uint32_t clock_mhz);
    int (*read_block)(OtpDevice *dev, uint32_t addr, uint8_t *data,
                      uint32_t size);
    int (*program_block)(OtpDevice *dev, uint32_t addr, const uint8_t *data,
                         uint32_t size);
    void (*irq_handler)(OtpDevice *dev);
} OtpOperations;

/**
 * @brief Create OTP instance
 * @param dev OTP device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_add_dev(OtpDevice *dev);

/**
 * @brief get otp device
 *
 * @param dev_id device id
 * @return OtpDevice*
 */
OtpDevice *hal_otp_get_device(uint8_t dev_id);

/**
 * @brief Destroy the OTP instance
 * @param dev OTP device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_remove_dev(OtpDevice *dev);

/**
 * @brief OTP HAL function, used to init the OTP driver and clear cached data
 * @param dev OTP device
 * @param clock_mhz the clock of eFuse/OTP in MHz
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_init(OtpDevice *dev, uint32_t clock_mhz);

/**
 * @brief OTP HAL function, used to get injection state from OTP/efuse
 * @param dev OTP device
 * @param inj_id the Injection ID which defined in OTP_INJECTION_ID
 * @param state the point of state need to be updated from OTP/efuse
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_injection_state(OtpDevice *dev, OTP_INJECTION_ID inj_id,
                            uint8_t *state);

/**
 * @brief OTP HAL function, used to read data block from OTP/efuse
 * @param dev OTP device
 * @param addr the start byte address of the block on OTP/efuse
 * @param data the point of data need to be read from OTP/efuse
 * @param size the size of block in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_read_block(OtpDevice *dev, uint32_t addr, uint8_t *data,
                       uint32_t size);

/**
 * @brief OTP HAL function, used to validate the MP block in OTP by CRC
 * @param dev OTP device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_validate_mp_block(OtpDevice *dev);

/**
 * @brief OTP HAL function, used to get ready state of MP Flow
 * @param dev OTP device
 * @param ready pointer to ready flag, value 1 for ready, value 0 for empty
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_is_mp_ready(OtpDevice *dev, uint8_t *ready);

/**
 * @brief OTP HAL function, used to program data block to OTP/efuse,read back
 * from OTP/eFuse for verification
 * @param dev OTP device
 * @param addr the start byte address of the block on OTP/efuse
 * @param data the point of data need to be programmed to OTP/efuse
 * @param size the size of block in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_program_block(OtpDevice *dev, uint32_t addr, const uint8_t *data,
                          uint32_t size);

/**
 * @brief OTP HAL function, used to program data block contains CRC16 to
 * OTP/eFuse, read back from OTP/eFuse for verification.
 * CRC16 should be little endian, contained in the last two bytes.
 * CRC16 will be programmed at last two bytes in the block.
 * @param dev OTP device
 * @param addr the start byte address of the block on OTP/efuse
 * @param data the point of data need to be programmed to OTP/efuse
 * @param size the size of block in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_otp_program_block_crc16(OtpDevice *dev, uint32_t addr,
                                const uint8_t *data, uint32_t size);

/**
 * @brief hal otp irq handler
 *
 * @param dev OTP device
 */
void hal_otp_irq_handler(OtpDevice *dev);
/** @} */

#endif /* _HAL_OTP_H */
