/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_crc.c
 * @brief HAL for crc driver
 */

#include "hal_crc.h"

static CrcDevice *crc_device = NULL;

static inline CrcOperations *get_ops(void)
{
    return (CrcOperations *)crc_device->ops;
}

int hal_crc_create(const CrcDevice *crc)
{
    if (!crc)
        return VSD_ERR_INVALID_POINTER;

    crc_device = (CrcDevice *)crc;
    return VSD_SUCCESS;
}

CrcDevice *hal_crc_get_device(void)
{
    return crc_device;
}

int hal_crc_init(void)
{
    if (!get_ops()->init)
        return VSD_ERR_UNSUPPORTED;

    return get_ops()->init(crc_device);
}

int hal_crc_enable(bool enable)
{
    if (!get_ops()->enable)
        return VSD_ERR_UNSUPPORTED;

    return get_ops()->enable(crc_device, enable);
}

int hal_crc_calculate(CrcInput *crc_input, uint32_t *crc_result)
{
    if (!crc_input->p_buffer)
        return VSD_ERR_INVALID_POINTER;
    if (!crc_input->buffer_length)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops()->calculate)
        return VSD_ERR_UNSUPPORTED;

    return get_ops()->calculate(crc_device, crc_input, crc_result);
}

int hal_crc_get_result(uint32_t *crc_result)
{
    if (!crc_result)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops()->get_result)
        return VSD_ERR_UNSUPPORTED;

    return get_ops()->get_result(crc_device, crc_result);
}
