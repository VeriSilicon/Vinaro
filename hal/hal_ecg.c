/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_ecg.c
 * @brief HAL for ECG sensor
 * @author Shangyun Qian <shangyun.qian@verisilicon.com>
 */
#include "hal_ecg.h"
#include "vsd_error.h"
#include "sys_common.h"
#include "sensor_log.h"

int hal_ecg_power(EcgDevice *ecg_device, uint8_t enable)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->power) {
        ret = ecg_device->power(ecg_device->ctx, enable);
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_read_sample(EcgDevice *ecg_device, uint16_t channel_mask,
                        uint32_t *sample, uint32_t buf_size, uint16_t *re_count)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->read_sample_raw) {
        ret = ecg_device->read_sample_raw(ecg_device->ctx, channel_mask, sample,
                                          buf_size, re_count);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_read_sample_dma(EcgDevice *ecg_device, uint16_t channel_mask,
                            uint32_t *sample, uint32_t buf_size,
                            uint16_t *re_count, XferCallback callback)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->read_sample_dma) {
        ret = ecg_device->read_sample_dma(ecg_device->ctx, channel_mask, sample,
                                          buf_size, re_count, callback);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_lod_detect(EcgDevice *ecg_device, uint8_t *connected)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->detect_lod) {
        ret = ecg_device->detect_lod(ecg_device->ctx, connected);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_configure_channel(EcgDevice *ecg_device, uint8_t channel_mask,
                              uint8_t p_pin, uint8_t n_pin)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    // Connect channel 1’s INP to IN2 and INN to IN1.
    if (ecg_device->configure_channel) {
        ret = ecg_device->configure_channel(ecg_device->ctx, channel_mask,
                                            p_pin, n_pin);
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_configure_rld_pin(EcgDevice *ecg_device, uint8_t pinid)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    // Connect the output of the RLD amplifier internally to pin IN4.
    if (ecg_device->configure_rld_pin) {
        ret = ecg_device->configure_rld_pin(ecg_device->ctx, pinid);
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_enable_channel(EcgDevice *ecg_device, uint16_t channel_mask)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    // Connect the output of the RLD amplifier internally to pin IN4.
    if (ecg_device->enable_channel) {
        ret = ecg_device->enable_channel(ecg_device->ctx, channel_mask);
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_set_samplerate(EcgDevice *ecg_device, uint32_t samplerate)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->set_samplerate) {
        ret = ecg_device->set_samplerate(ecg_device->ctx, samplerate);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_get_samplerate(EcgDevice *ecg_device, uint32_t *samplerate)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->get_samplerate) {
        ret = ecg_device->get_samplerate(ecg_device->ctx, samplerate);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_set_fifo_watermark(EcgDevice *ecg_device, uint8_t watermark)
{
    int ret = VSD_SUCCESS;
    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ecg_device->set_fifo_watermark) {
        ret = ecg_device->set_fifo_watermark(ecg_device->ctx, watermark);
    }
    return ret;
}

int hal_ecg_enable_lod_detect(EcgDevice *ecg_device, uint8_t enable)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->enable_lod_detect) {
        ret = ecg_device->enable_lod_detect(ecg_device->ctx, enable);
    } else if (ecg_device->ctx->hw_capability & ECG_CHANNEL_LOD) {
        ret = VSD_ERR_INVALID_PARAM;
    } else {
        ret = VSD_SUCCESS;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_start(EcgDevice *ecg_device)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->start) {
        ret = ecg_device->start(ecg_device->ctx);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_stop(EcgDevice *ecg_device)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->stop) {
        ret = ecg_device->stop(ecg_device->ctx);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_enable_interrupt(EcgDevice *ecg_device, uint8_t enable,
                             SensorDataReadyHandler data_ready_callback)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->enable_interrupt) {
        ret = ecg_device->enable_interrupt(ecg_device->ctx, enable,
                                           data_ready_callback);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_read_interrupt(EcgDevice *ecg_device, uint8_t *intr)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->read_interrupt) {
        ret = ecg_device->read_interrupt(ecg_device->ctx, intr);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_init(EcgDevice *ecg_device)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->init) {
        ret = ecg_device->init(ecg_device->ctx);
        if (ecg_device->ctx)
            ecg_device->capability &= ecg_device->ctx->hw_capability;
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_reset(EcgDevice *ecg_device)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->reset) {
        ret = ecg_device->reset(ecg_device->ctx);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_deinit(EcgDevice *ecg_device)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->deinit) {
        ret = ecg_device->deinit(ecg_device->ctx);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_write_register(EcgDevice *ecg_device, uint8_t reg, uint8_t val)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->write_regs) {
        ret = ecg_device->write_regs(ecg_device->ctx, reg, val);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_read_register(EcgDevice *ecg_device, uint8_t reg, uint8_t *val)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->read_regs) {
        ret = ecg_device->read_regs(ecg_device->ctx, reg, val);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_auto_gain_adjust(EcgDevice *ecg_device, uint16_t channel_id,
                             uint8_t gain_direction)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->auto_adjust_gain) {
        ret = ecg_device->auto_adjust_gain(ecg_device->ctx, channel_id,
                                           gain_direction);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}

int hal_ecg_read_sample_callback(EcgDevice *ecg_device,
                                 SensorReadBackHandler read_ecg,
                                 SensorReadBackData *raw_data,
                                 uint8_t *ecg_sample, uint16_t ecg_size,
                                 uint16_t *re_count)
{
    int ret = VSD_SUCCESS;

    if (!ecg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ecg_device->read_sample_back) {
        ret = ecg_device->read_sample_back(read_ecg, raw_data, ecg_sample,
                                           ecg_size, re_count);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    if (ret) {
        sensor_printf(SENSOR_ERR, "%s fail:%d", __func__, ret);
    }

    return ret;
}
