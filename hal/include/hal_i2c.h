/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i2c.h
 * @brief public head file of I2C HAL
 * @author Junwei Yang <junwei.yang@verisilicon.com>
 *
 */

#ifndef _HAL_I2C_H
#define _HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup I2C
 *  I2C HAL API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include "hal_common.h"

/**< Max supported I2C ports */
#define I2C_PORT_MAX (3)

typedef enum I2cIdDef {
    I2C_ID_0,
    I2C_ID_1,
    I2C_ID_MAX,
} I2cIdDef;

#define HAL_I2C_DEV_MAX (I2C_ID_MAX)

// #define I2C_DEBUG
#ifdef I2C_DEBUG
#define I2C_DEBUG(fmt, args...) uart_printf(fmt, ##args)
#else
#define I2C_DEBUG(fmt, args...)
#endif

/**
 * @enum I2cWorkMode
 * @brief Define the I2C work mode
 */
enum I2cWorkMode {
    I2C_MODE_MASTER = 1, /**< i2c interface works as master */
    I2C_MODE_SLAVE  = 2, /**< i2c interface works as slave */
};

/**
 * @enum I2cBusSpeed
 * @brief Specifies one of the standard I2C bus bit rates for I2C communication
 */
enum I2cBusSpeed {
    I2C_BUS_SPEED_100K = 100000, /**< I2C bus speed at 100KHz */
    I2C_BUS_SPEED_400K = 400000, /**< I2C bus speed at 400KHz */
    I2C_BUS_SPEED_1M   = 1000000, /**< I2C bus speed at 1MHz */
    I2C_BUS_SPEED_3P4M = 3400000, /**< I2C bus speed at 3.4MHz */
};

/**
 * @enum I2cAddressWidth
 * @brief Define the address width of I2C device
 */
enum I2cAddressWidth {
    I2C_HAL_ADDRESS_WIDTH_7BIT  = 0, /**< 7 bit mode */
    I2C_HAL_ADDRESS_WIDTH_10BIT = 1, /**< 10 bit mode */
};

/**
 * @brief This struct define i2c config args
 */
typedef struct I2cConfig {
    uint32_t address_width; /**< Addressing mode, @see I2cAddressWidth */
    uint32_t freq; /**< I2C bus freq, @see I2cBusSpeed */
    uint8_t work_mode; /**< master or slave mode, @see I2cWorkMode */
    uint8_t xfer_mode; /**< polling, intr or dma mode, @see I2cXferMode */
    uint16_t dev_addr; /**< slave device addr */
    uint8_t reg_width; /**< width of reg addr in byte */
} I2cConfig;

/**
 * @brief Structure of I2C hardware configuration
 */
typedef struct I2cHwConfig {
    uint32_t base; /**< Base address of I2C device */
    uint8_t id; /**< Device ID of this I2C device */
    uint8_t xfer_capability; /**< I2C capability */
    uint16_t clk_id; /**< Clock ID of this I2C device */
    uint16_t irq_id; /**< IRQ ID of this I2C device */
} I2cHwConfig;

/**
 * @struct I2cDevice
 * @brief This struct define i2c main handle
 */
typedef struct I2cDevice {
    uint8_t port_id; /**< I2C port id */
    const I2cHwConfig *i2cHw; /**< I2C hardware configuration */
    const void *ops; /**< I2C operations */
    void *ctx; /**< Private context */
} I2cDevice;

/**
 * @brief Structure of operations for HAL of I2C
 */
typedef struct I2cOperations {
    /**
     * I2C operation for init
     */
    int (*init)(const I2cDevice *device);

    /**
     * I2C operation for single read
     */
    int (*master_reg_read)(const I2cDevice *device, const I2cConfig *config,
                           uint32_t reg_addr, uint8_t *rev_buf,
                           uint32_t rev_size);
    /**
     * I2C operation for single write
     */
    int (*master_reg_write)(const I2cDevice *device, const I2cConfig *config,
                            uint32_t reg_addr, const uint8_t *wr_addr,
                            uint32_t wr_size);
    /**
     * I2C operation for consecutive read
     */
    int (*master_read)(const I2cDevice *device, const I2cConfig *config,
                       uint8_t *data, uint32_t size, uint32_t timeout);
    /**
     * I2C operation for consecutive write
     */
    int (*master_write)(const I2cDevice *device, const I2cConfig *config,
                        const uint8_t *data, uint32_t size, uint32_t timeout);
    /**
     * I2C operation for deinit
     */
    void (*deinit)(const I2cDevice *device);
    /**
     * I2C operation for irq handler
     */
    void (*irq_handler)(const I2cDevice *device);
} I2cOperations;

/**
 * @brief Add I2C device instance
 * @note This API should be called by I2C IP driver to add initialized instance
 * to HAL, not for application usage
 * @param device The device of I2C will be added
 * @return Return VSD_SUCCESS for succeed, others for failure
 */
int hal_i2c_add_dev(I2cDevice *device);

/**
 * @brief Get I2C device instance
 * @param port_id The port id of I2C
 * @return Return the I2C instance
 */
I2cDevice *hal_i2c_get_device(uint8_t port_id);

/**
 * @brief Initialises an I2C interface
 * Prepares an I2C hardware interface for communication as a master or slave
 * @note This API shall be called once before calling any I2C read/write APIs
 * @param[in]  i2c  the device of i2c port which will be initialized
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i2c_init(const I2cDevice *i2c);

/**
 * @brief I2c reg write
 *
 * @param[in]  i2c            the i2c device
 * @param[in]  config         the i2c config of device
 * @param[in]  reg_addr       reg address
 * @param[in]  data           i2c master send data
 * @param[in]  size           i2c master send data size in byte
 * @param[in]  timeout        timeout in millisecond, set this value to
 * HAL_WAIT_FOREVER if you want to wait forever
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i2c_reg_write(const I2cDevice *i2c, const I2cConfig *config,
                      uint32_t reg_addr, const uint8_t *data, uint32_t size,
                      uint32_t timeout);

/**
 * @brief I2c master reg read
 *
 * @param[in]   i2c            the i2c device
 * @param[in]   config         the i2c config of device
 * @param[in]   reg_addr       reg address
 * @param[out]  data           buffer to store read data from I2C device
 * @param[in]   size           i2c master send data size in byte
 * @param[in]  timeout         timeout in millisecond, set this value to
 * HAL_WAIT_FOREVER if you want to wait forever
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i2c_reg_read(const I2cDevice *i2c, const I2cConfig *config,
                     uint32_t reg_addr, uint8_t *data, uint32_t size,
                     uint32_t timeout);

/**
 * @brief Deinitialises an I2C device
 * @param[in]  i2c  the i2c device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i2c_finalize(const I2cDevice *i2c);

/**
 * @brief Interrupt handler for an External Interrupt
 * @param[in] i2c The i2c device
 */
void hal_i2c_irq_handler(const I2cDevice *i2c);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I2C_H */
