/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i2s.c
 * @brief HAL for I2S driver
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 */

#include <string.h>
#include "vsd_error.h"
#include "hal_i2s.h"

static I2sDevice *g_i2s_dev[I2S_PORT_MAX] = { NULL };

static inline I2SOperations *get_ops(const I2sDevice *i2s)
{
    return (I2SOperations *)i2s->ops;
}

int hal_i2s_add_dev(I2sDevice *i2s)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < ARRAY_SIZE(g_i2s_dev); i++) {
        if (g_i2s_dev[i] == NULL) {
            g_i2s_dev[i] = i2s;
            ret          = VSD_SUCCESS;
        }
    }

    return ret;
}

int hal_i2s_remove_dev(I2sDevice *i2s)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < ARRAY_SIZE(g_i2s_dev); i++) {
        if (g_i2s_dev[i] == i2s) {
            g_i2s_dev[i] = NULL;
            ret          = VSD_SUCCESS;
        }
    }

    return ret;
}

I2sDevice *hal_i2s_get_device(uint8_t id)
{
    uint8_t i;

    for (i = 0; i < ARRAY_SIZE(g_i2s_dev); i++) {
        if (g_i2s_dev[i]->hw_config && g_i2s_dev[i]->hw_config->id == id)
            return g_i2s_dev[i];
    }

    return NULL;
}

int hal_i2s_init(const I2sDevice *i2s)
{
    if (get_ops(i2s)->init)
        return get_ops(i2s)->init(i2s);

    return VSD_SUCCESS;
}

int hal_i2s_start(const I2sDevice *i2s, I2sSubstream *stream)
{
    if (get_ops(i2s)->start)
        return get_ops(i2s)->start(i2s, stream);

    return VSD_SUCCESS;
}

void hal_i2s_stop(const I2sDevice *i2s, I2sSubstream *stream)
{
    if (get_ops(i2s)->stop)
        get_ops(i2s)->stop(i2s, stream);
}

void hal_i2s_set_irq_callback(const I2sDevice *i2s, i2s_irq_callback cb,
                              void *ctx)
{
    if (get_ops(i2s)->set_irq_callback)
        get_ops(i2s)->set_irq_callback(i2s, cb, ctx);
}

uint8_t hal_i2s_get_tx_status(const I2sDevice *i2s)
{
    if (get_ops(i2s)->get_tx_status)
        return get_ops(i2s)->get_tx_status(i2s);

    return I2S_PROCESSING;
}

uint8_t hal_i2s_get_rx_status(const I2sDevice *i2s)
{
    if (get_ops(i2s)->get_rx_status)
        return get_ops(i2s)->get_rx_status(i2s);

    return I2S_PROCESSING;
}

int hal_i2s_finalize(const I2sDevice *i2s)
{
    if (get_ops(i2s)->deinit)
        get_ops(i2s)->deinit(i2s);

    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_i2s_irq_handler(const I2sDevice *i2s)
{
    if (!i2s || !get_ops(i2s)->irq_handler)
        return;

    get_ops(i2s)->irq_handler(i2s);
}
