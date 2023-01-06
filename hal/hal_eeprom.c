/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_eeprom.c
 * @brief HAL interface for EEPEOM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal_eeprom.h"
#include "vsd_error.h"
#include "vsd_common.h"

static EepromDevice *g_eeprom_dev[EEPROM_ID_MAX] = { NULL };

static inline EepromOperations *get_ops(const EepromDevice *device)
{
    return (EepromOperations *)device->ops;
}

int hal_eeprom_add_dev(EepromDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    for (i = 0; i < sizeof(g_eeprom_dev) / sizeof(g_eeprom_dev[0]); i++) {
        if (g_eeprom_dev[i] == NULL) {
            g_eeprom_dev[i] = device;
            ret             = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_eeprom_remove_dev(EepromDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    for (i = 0; i < sizeof(g_eeprom_dev) / sizeof(g_eeprom_dev[0]); i++) {
        if (g_eeprom_dev[i] == device) {
            g_eeprom_dev[i] = NULL;
            ret             = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

EepromDevice *hal_eeprom_get_device(uint8_t dev_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_eeprom_dev) / sizeof(g_eeprom_dev[0]); i++) {
        if (g_eeprom_dev[i]->dev_id == dev_id) {
            return g_eeprom_dev[i];
        }
    }
    return NULL;
}

int hal_eeprom_write(const EepromDevice *device, uint32_t word_addr,
                     const uint8_t *data, uint32_t size)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->write)
        ret = get_ops(device)->write(device, word_addr, data, size);

    return ret;
}

int hal_eeprom_read(const EepromDevice *device, uint32_t word_addr,
                    uint8_t *data, uint32_t size)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->read)
        ret = get_ops(device)->read(device, word_addr, data, size);

    return ret;
}
