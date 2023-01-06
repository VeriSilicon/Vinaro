/**
 *
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i2s.h
 * @brief Public head file of I2S HAL
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 *
 */

#ifndef _HAL_I2S_H
#define _HAL_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup I2S
 *  - I2S HAL API and definition
 *  - The construct show as below:
 *           ---------      ----------------
 *      --> | TX FIFO |--->| I2S            |
 *           ---------     |  ----------    |
 *                         | | control  |   |
 *           ---------     |  ----------    |
 *      --> | RX FIFO |<---|                |
 *           ---------      ----------------
 *
 * @ingroup HAL
 * Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>
#include "hal_common.h"

/**
 * @brief Max i2s instance
 */
#define I2S_PORT_MAX (1)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

#define I2S_IRQ_MUSK_TX BIT(0)
#define I2S_IRQ_MUSK_OVERRUN BIT(1)
#define I2S_IRQ_MUSK_RX BIT(2)
#define I2S_IRQ_MUSK_UNDERRUN BIT(3)

/**
 * @enum I2sIdDef
 * @brief ID definition of I2S interface
 */
typedef enum I2sIdDef {
    I2S_ID_0 = 0,
    I2S_ID_MAX,
} I2sIdDef;

/**
 * @enum I2sDirection
 * @brief i2s transfer direction
 */
typedef enum I2sDirection {
    I2S_TRANSMIT = 0,
    I2S_RECEIVE,
} I2sDirection;

/**
 * @enum I2sStatus
 * @brief i2s transfer status
 */
typedef enum I2sStatus {
    I2S_PROCESSING,
    I2S_UNDERRUN,
    I2S_OVERRUN,
} I2sStatus;

typedef struct _I2sSubstream {
    uint32_t sample_rate; /**< sample rate */
    uint8_t direction; /**< transfer direction, @see I2sDirection */
    uint8_t sample_width; /**< sample width */
    uint8_t channels; /**< channel number */
    uint8_t work_mode; /**< master or slave mode, @see DeviceWorkMode */
    uint8_t xfer_mode; /**< transfer mode, @see XferModeDef */
} I2sSubstream;

typedef struct _I2sFifo {
    uint32_t offset; /**< Fifo register */
    uint32_t handshake; /**< Hardware handshake id */
    uint8_t width; /**< Fifo word width of I2S device */
    uint8_t depth; /**< Fifo depth of I2S device in word */
} I2sFifo;

typedef struct _I2sHWConfig {
    uint32_t base; /**< Base address of I2S device */
    uint16_t mclk_id; /**< MCLK ID of this I2S device */
    uint16_t sclk_id; /**< SCLK ID of this I2S device */
    uint16_t irq_id; /**< IRQ ID of this I2S device */
    uint8_t id; /**< Device ID of this I2S device */
    uint8_t xfer_capability; /**< I2S capability, @see XferCapability */
    uint8_t ratio; /**< Clock ratio between mclk and sclk */

    I2sFifo tx_fifo; /**< IN: tx fifo configuration */
    I2sFifo rx_fifo; /**< OUT: rx fifo configuration */
} I2SHWConfig;

/**
 * @struct I2sDevice
 * @brief Define I2S controller driver
 */
typedef struct _I2sDevice {
    const I2SHWConfig *hw_config;
    const void *ops; /**< I2S operations */
    void *private_data; /**< private data*/
} I2sDevice;

/**
 * @fn i2s_irq_callback
 * @brief I2S interrupt callback function
 * @param i2s i2s device instance
 * @param mask @see I2S_IRQ_MUSK_*
 */
typedef void (*i2s_irq_callback)(const I2sDevice *i2s, uint32_t mask,
                                 void *cb_ctx);
typedef struct I2SOperations {
    /**
     * I2S operation for init
     */
    int (*init)(const I2sDevice *i2s);

    /**
     * I2S operation to start i2s device
     */
    int (*start)(const I2sDevice *i2s, I2sSubstream *stream);

    /**
     * I2S operation to stop i2s device
     */
    void (*stop)(const I2sDevice *i2s, I2sSubstream *stream);

    /**
     * I2S operation to set i2s interrupt callback
     */
    void (*set_irq_callback)(const I2sDevice *i2s, i2s_irq_callback cb,
                             void *ctx);

    /**
     * I2S operation to get i2s tx fifo status
     */
    uint8_t (*get_tx_status)(const I2sDevice *i2s);

    /**
     * I2S operation to get i2s rx fifo status
     */
    uint8_t (*get_rx_status)(const I2sDevice *i2s);

    /**
     * I2S operation for deinit
     */
    void (*deinit)(const I2sDevice *i2s);

    /**
     * I2S operation for interrupt handler.
     */
    void (*irq_handler)(const I2sDevice *i2s);
} I2SOperations;

/**
 * @brief Add I2S device instance
 * @param[in] i2s i2s device to be added
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_i2s_add_dev(I2sDevice *i2s);

/**
 * @brief Remove I2S device instance
 * @param[in] i2s i2s device to be removed
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_i2s_remove_dev(I2sDevice *i2s);

/**
 * @brief Get I2S device instance
 * @param[in] id The device ID of I2C, @see I2sIdDef
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
I2sDevice *hal_i2s_get_device(uint8_t id);

/**
 * @brief Initialises the I2S interface
 * @param[in] i2s the device of i2s port which will be initialized
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_i2s_init(const I2sDevice *i2s);

/**
 * @brief Start the i2s device.
 * @param[in] i2s i2s device
 * @param[in] stream i2s device steam configuration
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_i2s_start(const I2sDevice *i2s, I2sSubstream *stream);

/**
 * @brief Stop the i2s device
 * @param[in] i2s i2s device
 * @param[in] stream i2s stream
 */
void hal_i2s_stop(const I2sDevice *i2s, I2sSubstream *stream);

/**
 * @brief Set the interrupt callback function.
 * @param[in] i2s i2s device
 * @param[in] cb callback function
 * @param[in] cb_ctx private context of callback function
 */
void hal_i2s_set_irq_callback(const I2sDevice *i2s, i2s_irq_callback cb,
                              void *cb_ctx);

/**
 * @brief Get current i2s tx fifo status, enum @see I2sStatus
 * @param[in] i2s i2s device
 * @return uint8_t enum @see I2sStatus
 */
uint8_t hal_i2s_get_tx_status(const I2sDevice *i2s);

/**
 * @brief Get current i2s rx fifo status, enum @see I2sStatus
 * @param[in] i2s i2s device
 * @return uint8_t enum @see I2sStatus
 */
uint8_t hal_i2s_get_rx_status(const I2sDevice *i2s);

/**
 * @brief Deinitialises the i2s device
 * @param[in] i2s i2s device
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_i2s_finalize(const I2sDevice *i2s);

/**
 * @brief Interrupt handler for external interrupt.
 * this function will be called by interrupt controller, userspace ignore.
 * @param[in] i2s i2s device
 */
void hal_i2s_irq_handler(const I2sDevice *i2s);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I2S_H */
