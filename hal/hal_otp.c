/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_otp.c
 * @brief HAL for OTP/eFuse driver
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#include <stdbool.h>
#include <stddef.h>
#include "hal_otp.h"
#include "hal_crc.h"
#include "vsd_error.h"
#include "osal_heap_api.h"

static OtpDevice *g_otp_device[OTP_MAX_GROUPS] = { NULL };

/* local variables to avoid reading from OTP for multiple times */
static bool otp_injection_read = false;
static union OtpInjectionState injection_state;

static inline OtpOperations *get_ops(OtpDevice *dev)
{
    return (OtpOperations *)dev->ops;
}

static inline OtpHwConfig *get_cfg(OtpDevice *dev)
{
    return (OtpHwConfig *)dev->config;
}

int hal_otp_add_dev(OtpDevice *dev)
{
    uint32_t i;
    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    for (i = 0; i < sizeof(g_otp_device) / sizeof(g_otp_device[0]); i++) {
        if (g_otp_device[i] == NULL) {
            g_otp_device[i] = dev;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_FULL;
}

int hal_otp_remove_dev(OtpDevice *dev)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_otp_device) / sizeof(g_otp_device[0]); i++) {
        if (g_otp_device[i] != NULL && g_otp_device[i]->id == dev->id) {
            g_otp_device[i] = NULL;
        }
    }
    return VSD_SUCCESS;
}

OtpDevice *hal_otp_get_device(uint8_t dev_id)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_otp_device) / sizeof(g_otp_device[0]); i++) {
        if (g_otp_device[i] != NULL && g_otp_device[i]->id == dev_id) {
            return g_otp_device[i];
        }
    }
    return NULL;
}

int hal_otp_init(OtpDevice *dev, uint32_t clock_mhz)
{
    if (!dev) {
        return VSD_ERR_INVALID_PARAM;
    }
    otp_injection_read    = false;
    injection_state.value = 0;
    if (!get_ops(dev)->init)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(dev)->init(dev, clock_mhz);
}

int hal_otp_injection_state(OtpDevice *dev, OTP_INJECTION_ID inj_id,
                            uint8_t *state)
{
    int ret;
    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    if ((inj_id >= OTP_INJ_MAX) || (state == NULL))
        return VSD_ERR_INVALID_PARAM;

    if (!otp_injection_read) {
        /* was not read, try to read from OTP/efuse */
        injection_state.value = 0;

        if (!get_ops(dev)->read_block)
            return VSD_ERR_INVALID_POINTER;
        ret = get_ops(dev)->read_block(dev, OTP_INJECTION_STATE_ADDR,
                                       &injection_state.value,
                                       OTP_INJECTION_STATE_BYTES);
        if (VSD_SUCCESS != ret)
            return ret;

        otp_injection_read = true;
    }

    switch (inj_id) {
    case OTP_INJ_DEVICE_ID:
        *state = injection_state.device_id;
        break;

    case OTP_INJ_ROOT_KEY:
        *state = injection_state.root_key;
        break;

    case OTP_INJ_ECDSA_BOOT_KEY:
        *state = injection_state.ecdsa_boot_key;
        break;

    case OTP_INJ_ECDSA_EXCH_KEY:
        *state = injection_state.ecdsa_exch_key;
        break;

    case OTP_INJ_BLE_MAC:
        *state = injection_state.ble_mac;
        break;

    default:
        return VSD_ERR_INVALID_PARAM;
    }
    return VSD_SUCCESS;
}

int hal_otp_read_block(OtpDevice *dev, uint32_t addr, uint8_t *data,
                       uint32_t size)
{
    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(dev)->read_block)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(dev)->read_block(dev, addr, data, size);
}

int hal_otp_validate_mp_block(OtpDevice *dev)
{
    if (OTP_MP_ADDR_BASE == EFUSE_INVALID_ADDR) {
        return VSD_ERR_INVALID_STATE;
    }
    uint8_t read_data[PLATFORM_OTP_MP_BYTES];
    uint32_t size = PLATFORM_OTP_MP_BYTES;
    CrcInput crc_input;
    uint32_t crc_calc;
    unsigned short crc_le;
    const uint8_t crc_size = sizeof(unsigned short);
    bool ret;

    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(dev)->read_block)
        return VSD_ERR_INVALID_POINTER;

    ret = get_ops(dev)->read_block(dev, OTP_MP_ADDR_BASE, read_data, size);
    if (VSD_SUCCESS != ret)
        return ret;

    crc_input.p_buffer      = (void *)read_data;
    crc_input.buffer_length = (size - crc_size);
    crc_input.poly          = CRC_POLYNOMIAL_CRC_CCITT;
    crc_input.cal_switch    = CRC_LMS_MSB;
    hal_crc_calculate(&crc_input, &crc_calc);

    crc_le = (read_data[size - 1] << 8) + read_data[size - 2];
    if (crc_calc != crc_le)
        return VSD_ERR_INVALID_DATA;
    else
        return VSD_SUCCESS;
}

int hal_otp_is_mp_ready(OtpDevice *dev, uint8_t *ready)
{
    bool ret;
    uint8_t injection_val;
    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    if ((!get_ops(dev)->read_block) || !ready)
        return VSD_ERR_INVALID_POINTER;

    ret = get_ops(dev)->read_block(dev, OTP_INJECTION_STATE_ADDR,
                                   &injection_val, OTP_INJECTION_STATE_BYTES);
    if (VSD_SUCCESS != ret)
        return ret;

    /* Injection state is not empty means MP flow is ready */
    if (injection_val != 0)
        *ready = 1;
    else
        *ready = 0;

    return VSD_SUCCESS;
}

int hal_otp_program_block(OtpDevice *dev, uint32_t addr, const uint8_t *data,
                          uint32_t size)
{
    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(dev)->program_block)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(dev)->program_block(dev, addr, data, size);
}

int hal_otp_program_block_crc16(OtpDevice *dev, uint32_t addr,
                                const uint8_t *data, uint32_t size)
{
    uint32_t crc_calc;
    unsigned short crc_le;
    CrcInput crc_input;
    const uint8_t crc_size = sizeof(unsigned short);

    if (!dev)
        return VSD_ERR_INVALID_PARAM;
    if (size <= 2)
        return VSD_ERR_INVALID_PARAM;

    crc_input.p_buffer      = (void *)data;
    crc_input.buffer_length = (size - crc_size);
    crc_input.poly          = CRC_POLYNOMIAL_CRC_CCITT;
    crc_input.cal_switch    = CRC_LMS_MSB;
    /* CRC size is 16bit, follows CCITT standards */
    hal_crc_calculate(&crc_input, &crc_calc);
    crc_le = (data[size - 1] << 8) + data[size - 2];
    if (crc_calc != crc_le) {
        return VSD_ERR_INVALID_DATA;
    }

    if (!get_ops(dev)->program_block)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(dev)->program_block(dev, addr, data, size);
}
void hal_otp_irq_handler(OtpDevice *dev)
{
    if (!dev) {
        return;
    }
    if (!get_ops(dev)->irq_handler) {
        return;
    }
    return get_ops(dev)->irq_handler(dev);
}
