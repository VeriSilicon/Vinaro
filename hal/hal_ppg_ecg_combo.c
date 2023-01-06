/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_ppg_ecg_combo.c
 * @brief HAL interface for combo PPG&ECG sensor
 * @author Shangyun Qian <shangyun.qian@verisilicon.com>
 */

#include "hal_ppg_ecg_combo.h"
#include "vsd_error.h"

int hal_ppg_ecg_init(PpgEcgComboDevice *combo_device)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->init) {
        ret = combo_device->init(combo_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_wake_up(PpgEcgComboDevice *combo_device)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->wake_up) {
        ret = combo_device->wake_up(combo_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_shut_down(PpgEcgComboDevice *combo_device)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->shut_down) {
        ret = combo_device->shut_down(combo_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_reset(PpgEcgComboDevice *combo_device)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->reset) {
        ret = combo_device->reset(combo_device->ctx);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_read_sample(PpgEcgComboDevice *combo_device, uint8_t *red,
                            uint8_t *ir, uint8_t *ecg, uint16_t count,
                            int sample_format, uint16_t *return_count)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->read_sample) {
        switch (sample_format) {
        case SENSOR_SAMPLE_2_BYTE_LITTLEENDIAN:
            /* code */
            ret = combo_device->read_sample(combo_device->ctx, red, ir, ecg,
                                            count, 2, false, return_count);
            break;
        case SENSOR_SAMPLE_4_BYTE_LITTLEENDIAN:
            /* code */
            ret = combo_device->read_sample(combo_device->ctx, red, ir, ecg,
                                            count, 4, false, return_count);
            break;
        case SENSOR_SAMPLE_3_BYTE_LITTLEENDIAN:
            /* code */
            ret = combo_device->read_sample(combo_device->ctx, red, ir, ecg,
                                            count, 3, false, return_count);
            break;
        case SENSOR_SAMPLE_2_BYTE_BIGENDIAN:
            /* code */
            ret = combo_device->read_sample(combo_device->ctx, red, ir, ecg,
                                            count, 2, true, return_count);
            break;
        case SENSOR_SAMPLE_4_BYTE_BIGENDIAN:
            /* code */
            ret = combo_device->read_sample(combo_device->ctx, red, ir, ecg,
                                            count, 4, true, return_count);
            break;
        case SENSOR_SAMPLE_3_BYTE_BIGENDIAN:
            /* code */
            ret = combo_device->read_sample(combo_device->ctx, red, ir, ecg,
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

int hal_ppg_ecg_read_sample_bytes(PpgEcgComboDevice *combo_device,
                                  uint8_t *samples, uint16_t count,
                                  int sample_format, uint16_t *return_count)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->read_sample_bytes) {
        switch (sample_format) {
        case SENSOR_SAMPLE_2_BYTE_LITTLEENDIAN:
            ret =
                combo_device->read_sample_bytes(combo_device->ctx, samples,
                                                count, 2, false, return_count);
            break;
        case SENSOR_SAMPLE_4_BYTE_LITTLEENDIAN:
            ret =
                combo_device->read_sample_bytes(combo_device->ctx, samples,
                                                count, 4, false, return_count);
            break;
        case SENSOR_SAMPLE_3_BYTE_LITTLEENDIAN:
            ret =
                combo_device->read_sample_bytes(combo_device->ctx, samples,
                                                count, 3, false, return_count);
            break;
        case SENSOR_SAMPLE_2_BYTE_BIGENDIAN:
            ret = combo_device->read_sample_bytes(combo_device->ctx, samples,
                                                  count, 2, true, return_count);
            break;
        case SENSOR_SAMPLE_4_BYTE_BIGENDIAN:
            ret = combo_device->read_sample_bytes(combo_device->ctx, samples,
                                                  count, 4, true, return_count);
            break;
        case SENSOR_SAMPLE_3_BYTE_BIGENDIAN:
            ret = combo_device->read_sample_bytes(combo_device->ctx, samples,
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

int hal_ppg_ecg_read_interrupt(PpgEcgComboDevice *combo_device, uint8_t *val)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->read_interrupt) {
        ret = combo_device->read_interrupt(combo_device->ctx, val);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_set_work_mode(PpgEcgComboDevice *combo_device,
                              uint8_t sensor_mode)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->set_work_mode) {
        ret = combo_device->set_work_mode(combo_device->ctx, sensor_mode);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_enable_interrupt(PpgEcgComboDevice *combo_device, bool enable,
                                 SensorDataReadyHandler data_ready_callback)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->enable_interrupt) {
        ret = combo_device->enable_interrupt(combo_device->ctx, enable,
                                             data_ready_callback);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}

int hal_ppg_ecg_get_active_channel_num(PpgEcgComboDevice *combo_device,
                                       uint8_t *cnt)
{
    int ret = VSD_SUCCESS;
    if (!combo_device) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (combo_device->get_active_channel) {
        ret = combo_device->get_active_channel(combo_device->ctx, cnt);
    } else
        ret = VSD_ERR_INVALID_POINTER;
    return ret;
}
