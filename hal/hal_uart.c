/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_uart.c
 * @brief HAL interface for UART
 */

#include <string.h>
#include "hal_uart.h"
#include "vsd_error.h"
#include "hal_common.h"

static UartDevice *hal_dev[HAL_UART_DEV_MAX] = { NULL };

static inline UartOperations *get_ops(const UartDevice *dev)
{
    return (UartOperations *)dev->ops;
}

int hal_uart_add_dev(UartDevice *dev)
{
    int ret;
    uint8_t i = 0;
    for (i = 0; i < sizeof(hal_dev) / sizeof(hal_dev[0]); i++) {
        if (hal_dev[i] == NULL) {
            hal_dev[i] = dev;
            ret        = VSD_SUCCESS;
            break;
        }
    }
    if (i == sizeof(hal_dev) / sizeof(hal_dev[0])) {
        ret = VSD_ERR_FULL;
    }
    return ret;
}

int hal_uart_remove_dev(UartDevice *dev)
{
    int ret;
    uint8_t i = 0;
    for (i = 0; i < sizeof(hal_dev) / sizeof(hal_dev[0]); i++) {
        if (hal_dev[i] == dev) {
            hal_dev[i] = NULL;
            ret        = VSD_SUCCESS;
            break;
        }
    }
    if (i == sizeof(hal_dev) / sizeof(hal_dev[0])) {
        ret = VSD_ERR_NON_EXIST;
    }
    return ret;
}

UartDevice *hal_uart_get_device(uint8_t dev_id)
{
    UartDevice *dev = NULL;
    uint8_t i       = 0;
    for (i = 0; i < sizeof(hal_dev) / sizeof(hal_dev[0]); i++) {
        if (hal_dev[i]->dev_id == dev_id) {
            dev = hal_dev[i];
            break;
        }
    }
    if (i == sizeof(hal_dev) / sizeof(hal_dev[0])) {
        dev = NULL;
    }
    return dev;
}

int hal_uart_fifo_flush(const UartDevice *dev)
{
    if (!dev)
        return VSD_ERR_INVALID_POINTER;
    else if (!get_ops(dev)->fifo_flush)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(dev)->fifo_flush(dev);
}

int hal_uart_get_char(const UartDevice *dev, char *c)
{
    if (!dev)
        return VSD_ERR_INVALID_POINTER;
    else if (!get_ops(dev)->data_gets) {
        return VSD_ERR_UNSUPPORTED;
    }

    if (1 == get_ops(dev)->data_gets(dev, 1, c))
        return VSD_SUCCESS;
    else
        return VSD_ERR_GENERIC;
}

int hal_uart_put_char(const UartDevice *dev, const char c)
{
    uint32_t ret_len;
    if (!dev)
        return VSD_ERR_INVALID_POINTER;
    else if (!get_ops(dev)->data_puts)
        return VSD_ERR_UNSUPPORTED;

    ret_len = get_ops(dev)->data_puts(dev, 1, &c);
    if (1 == ret_len)
        return VSD_SUCCESS;
    else
        return VSD_ERR_GENERIC;
}

int hal_uart_put_string(const UartDevice *dev, const char *s)
{
    uint32_t len;
    uint32_t ret_len;
    if (!dev)
        return VSD_ERR_INVALID_POINTER;
    else if (!get_ops(dev)->data_puts)
        return VSD_ERR_UNSUPPORTED;

    len     = strlen((const char *)s);
    ret_len = get_ops(dev)->data_puts(dev, len, s);
    if (len == ret_len)
        return VSD_SUCCESS;
    else
        return VSD_ERR_GENERIC;
}

int hal_uart_get_string(const UartDevice *dev, char *s, uint32_t len,
                        uint32_t *act_len)
{
    if (!dev || !act_len)
        return VSD_ERR_INVALID_POINTER;
    else if (!get_ops(dev)->data_gets) {
        *act_len = 0;
        return VSD_ERR_UNSUPPORTED;
    }

    *act_len = get_ops(dev)->data_gets(dev, len, s);
    return VSD_SUCCESS;
}

int hal_uart_config(UartDevice *dev, const UartXferConfig *cfg)
{
    if (!dev || !cfg)
        return VSD_ERR_INVALID_POINTER;
    else if (!get_ops(dev)->uart_config)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(dev)->uart_config(dev, cfg);
}

DRV_ISR_SECTION
void hal_uart_irq_handler(const UartDevice *dev)
{
    if (!dev || !get_ops(dev)->irq_handler)
        return;

    return get_ops(dev)->irq_handler(dev);
}
