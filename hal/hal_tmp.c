/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_tmp.c
 * @brief HAL for temperature sensor driver
 */
#include "hal_tmp.h"
#include "vsd_error.h"

int hal_tmp_init(TmpDevice *device)
{
    int ret = VSD_SUCCESS;
    if (!device || !device->init) {
        return VSD_ERR_INVALID_PARAM;
    }
    ret = device->init(device);
    // update the capability per user definition and hw capability
    device->capability &= device->hw_capability;
    return ret;
}

int hal_tmp_destroy(TmpDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (device->destroy)
        return device->destroy(device);

    return VSD_SUCCESS;
}

int hal_tmp_power(TmpDevice *device, bool enable)
{
    if (!device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (device->power)
        return device->power(device, enable);

    return VSD_SUCCESS;
}

int hal_tmp_reset(TmpDevice *device)
{
    if (!device || !device->reset)
        return VSD_ERR_INVALID_PARAM;

    return device->reset(device);
}

int hal_tmp_set_mode(TmpDevice *device, uint32_t mode)
{
    if (!device || !device->set_mode)
        return VSD_ERR_INVALID_PARAM;

    return device->set_mode(device, mode);
}

int hal_tmp_read(TmpDevice *device, uint16_t *value)
{
    if (!device || !device->read || !value)
        return VSD_ERR_INVALID_PARAM;

    return device->read(device, value);
}

int hal_tmp_read_reg(TmpDevice *device, uint16_t reg, uint8_t *value,
                     uint16_t size)
{
    if (!device || !device->read_reg || !value || !size)
        return VSD_ERR_INVALID_PARAM;

    return device->read_reg(device, reg, value, size);
}

int hal_tmp_write_reg(TmpDevice *device, uint16_t reg, uint8_t *value,
                      uint16_t size)
{
    if (!device || !device->write_reg || !value || !size)
        return VSD_ERR_INVALID_PARAM;

    return device->write_reg(device, reg, value, size);
}
