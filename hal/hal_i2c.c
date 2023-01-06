/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i2c.c
 * @brief HAL interface for I2C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal_i2c.h"
#include "vsd_error.h"
#include "vsd_common.h"

static I2cDevice *g_i2c_dev[I2C_PORT_MAX] = { NULL };

static inline I2cOperations *get_ops(const I2cDevice *i2c)
{
    return (I2cOperations *)i2c->ops;
}

int hal_i2c_add_dev(I2cDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_i2c_dev) / sizeof(g_i2c_dev[0]); i++) {
        if (g_i2c_dev[i] == NULL) {
            g_i2c_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_i2c_remove_dev(I2cDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_i2c_dev) / sizeof(g_i2c_dev[0]); i++) {
        if (g_i2c_dev[i] == device) {
            g_i2c_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

I2cDevice *hal_i2c_get_device(uint8_t port_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_i2c_dev) / sizeof(g_i2c_dev[0]); i++) {
        if (g_i2c_dev[i]->port_id == port_id) {
            return g_i2c_dev[i];
        }
    }
    return NULL;
}

int hal_i2c_init(const I2cDevice *i2c)
{
    if (get_ops(i2c)->init)
        get_ops(i2c)->init(i2c);

    return VSD_SUCCESS;
}

int hal_i2c_reg_write(const I2cDevice *i2c, const I2cConfig *config,
                      uint32_t reg_addr, const uint8_t *data, uint32_t size,
                      uint32_t timeout)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (get_ops(i2c)->master_reg_write)
        ret = get_ops(i2c)->master_reg_write(i2c, config, reg_addr, data, size);

    return ret;
}

int hal_i2c_reg_read(const I2cDevice *i2c, const I2cConfig *config,
                     uint32_t reg_addr, uint8_t *data, uint32_t size,
                     uint32_t timeout)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (get_ops(i2c)->master_reg_read)
        ret = get_ops(i2c)->master_reg_read(i2c, config, reg_addr, data, size);

    return ret;
}

int hal_i2c_finalize(const I2cDevice *i2c)
{
    if (get_ops(i2c)->deinit)
        get_ops(i2c)->deinit(i2c);

    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_i2c_irq_handler(const I2cDevice *i2c)
{
    if (!i2c || !get_ops(i2c)->irq_handler)
        return;

    return get_ops(i2c)->irq_handler(i2c);
}
