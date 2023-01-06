/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_common.h
 * @brief Common definition of HAL
 */

#ifndef _HAL_COMMON_H_
#define _HAL_COMMON_H_
#include <stdbool.h>
#include "sys_common.h"
#include "hal_gpio.h"
/** @addtogroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/** Define the wait forever timeout macro */
#define HAL_WAIT_FOREVER 0xFFFFFFFFU

/**
 * @brief Callback handler for transfer complete
 */
typedef void (*XferCallback)(void *param);

/**
 * @enum DeviceProperty
 * @brief Device property, indicate it's a HW or SW device
 */
enum DeviceProperty {
    DEVICE_PROP_HW, /**< A hardware device */
    DEVICE_PROP_SW, /**< A software device */
};

/**
 * @enum DeviceWorkMode
 * @brief Device work mode, master or slave
 */
enum DeviceWorkMode {
    DEVICE_MASTER_MODE,
    DEVICE_SLAVE_MODE,
};

/**
 * @enum XferModeDef
 * @brief Define common transfer modes
 */
typedef enum XferModeDef {
    XFER_MODE_POLLING = 0, /**< polling mode */
    XFER_MODE_INTR    = 1, /**< interrupt mode */
    XFER_MODE_DMA     = 2, /**< dma mode */
    XFER_MODE_MAX,
    XFER_MODE_DEFAULT = XFER_MODE_MAX
} XferModeDef;

/**
 * @enum XferCapability
 * @brief Define the capability of transfer modes
 */
enum XferCapability {
    XFER_CAP_POLLING = (0x1 << XFER_MODE_POLLING),
    XFER_CAP_INTR    = (0x1 << XFER_MODE_INTR),
    XFER_CAP_DMA     = (0x1 << XFER_MODE_DMA),
};

typedef struct {
    const GpioPort *power_pin; /**< gpio of power pin */
    const GpioPort *data_ready_pin; /**< gpio of interrupt pin */
    const GpioPort *reset_pin; /**< gpio of reset pin  */
    uint8_t device_id; /**< device id in multi device case */
    bool rx_data_cont; /**< Data read continues mode */
} SensorHwConfig;

/** @} */
#endif // _HAL_COMMON_H_
