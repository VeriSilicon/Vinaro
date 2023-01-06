/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_mbox.h
 * @brief Mailbox HAL interface
 */

#ifndef _HAL_MBOX_H
#define _HAL_MBOX_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup MAILBOX
 *  Mailbox hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdio.h>
#include <stdbool.h>
#include "vsd_error.h"
#include "vsd_common.h"

#define MBOX_DEVICE_MAX (2)

/**
 * @brief Mailbox interrupt callback handler
 */
typedef void (*MboxHandler)(void);

/**
 * @brief Structure of Mailbox hardware configuration
 */
typedef struct MboxHwConfig {
    uint32_t base; /**< Base address of Mailbox device */
    uint16_t ack_irq_id; /**< ack Interrupt ID which defined by SoC */
    uint16_t req_irq_id; /**< req Interrupt ID which defined by SoC */
    uint8_t clk_id; /**<  clk id */
} MboxHwConfig;

/**
 * @brief Device hal handle for mailbox.
 * */
typedef struct {
    uint8_t hw_id; /**< Port ID, the ID of hardware device */
    const void *ops; /**< Mailbox operations */
    const MboxHwConfig *hw_config; /**< Mailbox hardware configuration */
    void *ctx;
} MboxDevice;

/**
 * @brief Structure of operations for HAL of Mailbox.
 */
typedef struct MboxOperations {
    int (*init)(const MboxDevice *device);
    int (*enable)(const MboxDevice *device, bool enable,
                  MboxHandler req_cb_handler);
    int (*request)(const MboxDevice *device, MboxHandler ack_cb_handler);
    void (*req_irq_handler)(const MboxDevice *device);
    void (*ack_irq_handler)(const MboxDevice *device);
} MboxOperations;

/**
 * @brief Add mailbox device instance
 * @param device The device of mailbox will be added
 * @return Return result
 * @retval Return VSD_SUCCESS for succeed, others for failure
 */
int hal_mbox_add_dev(MboxDevice *device);

/**
 * @brief Get mailbox device instance
 * @param hw_id The device hw id of mailbox
 * @return Return the instance of mailbox device
 */
MboxDevice *hal_mbox_get_device(uint8_t hw_id);

/**
 * @brief Mailbox init.
 * @param device Pointer of Mailbox devices
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_mbox_init(const MboxDevice *device);

/**
 * @brief Mailbox enable/disable.
 * @param device Pointer of Mailbox devices
 * @param enable true or false
 * @param req_cb_handler request callback handler
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_mbox_enable(const MboxDevice *device, bool enable,
                    MboxHandler req_cb_handler);

/**
 * @brief Mailbox request interrupt.
 * @param device Pointer of Mailbox devices
 * @param ack_cb_handler ack callback handler
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_mbox_request(const MboxDevice *device, MboxHandler ack_cb_handler);

/**
 * @brief mailbox ack interrupt handler.
 * @param device Pointer of Mailbox devices
 */
void hal_mbox_ack_irq_handler(const MboxDevice *device);

/**
 * @brief mailbox req interrupt handler.
 * @param device Pointer of Mailbox devices
 */
void hal_mbox_req_irq_handler(const MboxDevice *device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // _HAL_MBOX_H
