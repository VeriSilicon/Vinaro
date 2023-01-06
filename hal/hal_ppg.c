/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_ppg.c
 * @brief HAL interface for PPG driver
 * @author Shangyun Qian <shangyun.qian@verisilicon.com>
 */

#include "hal_ppg.h"
#include "vsd_common.h"
#include "vsd_error.h"
#include "device.h"

int hal_ppg_init(PpgDevice *ppg_device)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->init) {
        ret = ppg_device->init(ppg_device->ctx);
        if (ppg_device->ctx)
            ppg_device->capability &= ppg_device->ctx->hw_capability;
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_wake_up(PpgDevice *ppg_device)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->wake_up) {
        ret = ppg_device->wake_up(ppg_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_shut_down(PpgDevice *ppg_device)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->shut_down) {
        ret = ppg_device->shut_down(ppg_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_reset(PpgDevice *ppg_device)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->reset) {
        ret = ppg_device->reset(ppg_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_enable_power(PpgDevice *ppg_device, bool is_power_on)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->power_on) {
        ret = ppg_device->power_on(ppg_device->ctx, is_power_on);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_set_led_amplitude(PpgDevice *ppg_device, uint8_t powerLevel,
                              uint8_t light_type)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->set_led_amplitude) {
        ret = ppg_device->set_led_amplitude(ppg_device->ctx, powerLevel,
                                            light_type);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_get_led_amplitude(PpgDevice *ppg_device, uint8_t *powerLevel,
                              uint8_t light_type)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->get_led_amplitude) {
        ret = ppg_device->get_led_amplitude(ppg_device->ctx, powerLevel,
                                            light_type);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_set_adc_range(PpgDevice *ppg_device, uint32_t range)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->set_adc_range) {
        ret = ppg_device->set_adc_range(ppg_device->ctx, range);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_set_samplerate(PpgDevice *ppg_device, uint32_t rate)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->set_samplerate) {
        ret = ppg_device->set_samplerate(ppg_device->ctx, rate);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_get_samplerate(PpgDevice *ppg_device, uint32_t *rate)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->get_samplerate) {
        ret = ppg_device->get_samplerate(ppg_device->ctx, rate);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_set_pulsewidth(PpgDevice *ppg_device, uint16_t width)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->set_pulsewidth) {
        ret = ppg_device->set_pulsewidth(ppg_device->ctx, width);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_set_work_mode(PpgDevice *ppg_device, uint8_t ledMode)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->set_work_mode) {
        ret = ppg_device->set_work_mode(ppg_device->ctx, ledMode);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_set_fifo_watermark(PpgDevice *ppg_device, uint8_t watermark)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->set_fifo_watermark) {
        ret = ppg_device->set_fifo_watermark(ppg_device->ctx, watermark);
    }
    return ret;
}

int hal_ppg_read_interrupt(PpgDevice *ppg_device, uint8_t *int_status)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->read_interrupt) {
        ret = ppg_device->read_interrupt(ppg_device->ctx, int_status);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_read_sample(PpgDevice *ppg_device, uint8_t *red, uint8_t *ir,
                        uint8_t *green, uint16_t count, int sample_format,
                        uint16_t *return_count)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->read_sample) {
        switch (sample_format) {
        case SENSOR_SAMPLE_2_BYTE_LITTLEENDIAN:
            /* code */
            ret = ppg_device->read_sample(ppg_device->ctx, red, ir, green,
                                          count, 2, false, return_count);
            break;
        case SENSOR_SAMPLE_4_BYTE_LITTLEENDIAN:
            /* code */
            ret = ppg_device->read_sample(ppg_device->ctx, red, ir, green,
                                          count, 4, false, return_count);
            break;
        case SENSOR_SAMPLE_3_BYTE_LITTLEENDIAN:
            /* code */
            ret = ppg_device->read_sample(ppg_device->ctx, red, ir, green,
                                          count, 3, false, return_count);
            break;
        case SENSOR_SAMPLE_2_BYTE_BIGENDIAN:
            /* code */
            ret = ppg_device->read_sample(ppg_device->ctx, red, ir, green,
                                          count, 2, true, return_count);
            break;
        case SENSOR_SAMPLE_4_BYTE_BIGENDIAN:
            /* code */
            ret = ppg_device->read_sample(ppg_device->ctx, red, ir, green,
                                          count, 4, true, return_count);
            break;
        case SENSOR_SAMPLE_3_BYTE_BIGENDIAN:
            /* code */
            ret = ppg_device->read_sample(ppg_device->ctx, red, ir, green,
                                          count, 3, true, return_count);
            break;
        default:
            ret = VSD_ERR_UNSUPPORTED;
            break;
        }
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_read_sample_bytes(PpgDevice *ppg_device, uint8_t *samples,
                              uint16_t exp_size, int sample_format,
                              uint16_t *act_size)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->read_sample_bytes) {
        switch (sample_format) {
        case SENSOR_SAMPLE_2_BYTE_LITTLEENDIAN:
            /* code */
            ret = ppg_device->read_sample_bytes(ppg_device->ctx, samples,
                                                exp_size, 2, false, act_size);
            break;
        case SENSOR_SAMPLE_4_BYTE_LITTLEENDIAN:
            /* code */
            ret = ppg_device->read_sample_bytes(ppg_device->ctx, samples,
                                                exp_size, 4, false, act_size);
            break;
        case SENSOR_SAMPLE_3_BYTE_LITTLEENDIAN:
            /* code */
            ret = ppg_device->read_sample_bytes(ppg_device->ctx, samples,
                                                exp_size, 3, false, act_size);
            break;
        case SENSOR_SAMPLE_2_BYTE_BIGENDIAN:
            /* code */
            ret = ppg_device->read_sample_bytes(ppg_device->ctx, samples,
                                                exp_size, 2, true, act_size);
            break;
        case SENSOR_SAMPLE_4_BYTE_BIGENDIAN:
            /* code */
            ret = ppg_device->read_sample_bytes(ppg_device->ctx, samples,
                                                exp_size, 4, true, act_size);
            break;
        case SENSOR_SAMPLE_3_BYTE_BIGENDIAN:
            /* code */
            ret = ppg_device->read_sample_bytes(ppg_device->ctx, samples,
                                                exp_size, 3, true, act_size);
            break;
        default:
            ret = VSD_ERR_UNSUPPORTED;
            break;
        }
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_enable_interrupt(PpgDevice *ppg_device, bool enable,
                             SensorDataReadyHandler data_ready_callback)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->enable_interrupt) {
        ret = ppg_device->enable_interrupt(ppg_device->ctx, enable,
                                           data_ready_callback);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_read_register(PpgDevice *ppg_device, uint8_t reg, uint8_t *val)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->read_register) {
        ret = ppg_device->read_register(ppg_device->ctx, reg, val);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_write_register(PpgDevice *ppg_device, uint8_t reg, uint8_t val)
{
    int ret = VSD_SUCCESS;
    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (ppg_device->write_register) {
        ret = ppg_device->write_register(ppg_device->ctx, reg, val);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_read_sample_callback(PpgDevice *ppg_device,
                                 SensorReadBackHandler read_ppg,
                                 SensorReadBackData *raw_data,
                                 uint8_t *ppg_sample, uint16_t ppg_size,
                                 uint16_t *re_count)
{
    int ret = VSD_SUCCESS;

    if (!ppg_device) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (ppg_device->read_sample_back) {
        ret = ppg_device->read_sample_back(read_ppg, raw_data, ppg_sample,
                                           ppg_size, re_count);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }

    return ret;
}
