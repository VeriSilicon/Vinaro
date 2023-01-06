/**
 *
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pdm.h
 * @brief Public head file of PDM HAL
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 *
 */

#ifndef _HAL_PDM_H
#define _HAL_PDM_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup PDM
 *  - PDM HAL API and definition
 *  - The construct show as below:
 *                          ----------------
 *                         | PDM            |
 *                         |  ----------    |
 *                         | | control  |   |
 *           ---------     |  ----------    |
 *      <-- | RX FIFO |<---|                |
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
 * @brief Max PDM instance
 */
#define PDM_PORT_MAX (1)

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

#define PDM_IRQ_MUSK_RX BIT(0)
#define PDM_IRQ_MUSK_OVERRUN BIT(1)
#define PDM_IRQ_MUSK_UNDERRUN BIT(2)

/**
 * @enum PdmIdDef
 * @brief ID definition of PDM interface
 */
typedef enum PdmIdDef {
    PDM_ID_0 = 0,
    PDM_ID_MAX,
} PdmIdDef;

typedef struct _PdmFifo {
    uint32_t offset; /**< Fifo register */
    uint16_t handshake; /**< Hardware handshake id */
    uint8_t depth; /**< Fifo word width of PDM device */
    uint8_t width; /**< Fifo depth of PDM device in word */
} PdmFifo;

typedef struct _PDMHWConfig {
    uint32_t base; /**< Base address of PDM device */
    uint16_t sclk_id; /**< SCLK ID of this PDM device */
    uint16_t irq_id; /**< IRQ ID of this PDM device */
    uint8_t id; /**< Device ID of this PDM device, @see PdmIdDef */
    uint8_t xfer_capability; /**< PDM capability, @see XferCapability */

    PdmFifo rx_fifo; /**< OUT: rx fifo configuration */
} PDMHWConfig;

/**
 * @struct PdmDevice
 * @brief Define PDM controller driver
 */
typedef struct _PdmDevice {
    const PDMHWConfig *hw_config; /**< PDM hardware configuration */
    const void *ops; /**< PDM operations */
    void *private_data; /**< private data*/
} PdmDevice;

/**
 * @brief PDM interrupt callback function
 * @param pdm PDM device instance
 * @param mask @see PDM_IRQ_MUSK_*
 * @param cb_ctx callback context
 */
typedef void (*pdm_irq_callback)(const PdmDevice *pdm, uint32_t mask,
                                 void *cb_ctx);

typedef struct _PdmSubstream {
    uint32_t sample_rate; /**< sample rate */
    pdm_irq_callback cb;
    void *cb_context;
    uint8_t sample_width; /**< sample width */
    uint8_t chan_num; /**< @see pdm_channel_mask */
    uint8_t work_mode; /**< master or slave mode, @see DeviceWorkMode */
    uint8_t xfer_mode; /**< transfer mode, @see XferModeDef */
} PdmSubstream;

typedef struct _PdmOperation {
    /**
     * PDM operation for init
     */
    int (*init)(const PdmDevice *pdm);

    /**
     * PDM operation to start pdm device
     */
    int (*start)(const PdmDevice *pdm, PdmSubstream *stream);

    /**
     * PDM operation to stop pdm device
     */
    void (*stop)(const PdmDevice *pdm, PdmSubstream *stream);

    /**
     * PDM operation to set gain
     */
    void (*set_gain)(const PdmDevice *pdm, uint8_t gain);

    /**
     * PDM operation to get pdm rx fifo status
     */
    uint8_t (*get_rx_status)(const PdmDevice *pdm);

    /**
     * PDM operation to clear fifo status
     */
    void (*clear_staus)(const PdmDevice *pdm);

    /**
     * PDM operation for deinit
     */
    void (*deinit)(const PdmDevice *pdm);

    /**
     * PDM operation for interrupt handler.
     */
    void (*irq_handler)(const PdmDevice *pdm);
} PdmOperation;

/**
 * @brief Add pdm device instance
 * @param[in] pdm pdm device to be added
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_pdm_add_dev(PdmDevice *pdm);

/**
 * @brief Remove PDM device instance
 * @param[in] pdm pdm device to be removed
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_pdm_remove_dev(PdmDevice *pdm);

/**
 * @brief Get PDM device instance
 * @param dev_id The device ID of PDM, @see PdmIdDef
 * @return PdmDevice* PDM instance
 */
PdmDevice *hal_pdm_get_device(uint8_t dev_id);

/**
 * @brief Initialises the PDM interface
 * @param[in] pdm the device of pdm port which will be initialized
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_pdm_init(const PdmDevice *pdm);

/**
 * @brief Start the pdm device.
 * @param[in] pdm pdm device
 * @param[in] stream pdm device steam configuration
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_pdm_start(const PdmDevice *pdm, PdmSubstream *stream);

/**
 * @brief Stop the pdm device
 * @param[in] pdm pdm device
 * @param[in] stream pdm stream
 */
void hal_pdm_stop(const PdmDevice *pdm, PdmSubstream *stream);

/**
 * @brief Get current pdm tx fifo status
 * @param[in] pdm pdm device
 * @return uint8_t enum @see PdmIdDef
 */
uint8_t hal_pdm_get_rx_status(const PdmDevice *pdm);

/**
 * @brief Clear fifo status
 * @param pdm pdm device
 */
void hal_pdm_clear_status(const PdmDevice *pdm);

/**
 * @brief Deinitialises the pdm device
 * @param[in] pdm pdm device
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_pdm_finalize(const PdmDevice *pdm);

/**
 * @brief Interrupt handler for external interrupt.
 * this function will be called by interrupt controller, userspace ignore.
 * @param[in] pdm pdm device
 */
void hal_pdm_irq_handler(const PdmDevice *pdm);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_PDM_H */
