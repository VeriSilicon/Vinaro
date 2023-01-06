/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_imu.c
 * @brief HAL interface for IMU
 */

#include "hal_imu.h"
#include "vsd_common.h"
#include "vsd_error.h"
#include "device.h"

int hal_imu_init(ImuDevice *dev)
{
    if (!dev || !dev->init) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->init(dev->ctx);
}

int hal_imu_deinit(ImuDevice *dev)
{
    if (!dev || !dev->deinit) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->deinit(dev->ctx);
}

int hal_imu_soft_reset(ImuDevice *dev)
{
    if (!dev) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (!dev->soft_reset) {
        return VSD_ERR_UNSUPPORTED;
    }
    return dev->soft_reset(dev->ctx);
}

int hal_imu_read_reg(ImuDevice *dev, uint8_t reg_addr, uint8_t *data,
                     uint16_t len)
{
    if (!dev || !dev->read_reg || !data)
        return VSD_ERR_INVALID_PARAM;

    return dev->read_reg(dev->ctx, reg_addr, data, len);
}

int hal_imu_write_reg(ImuDevice *dev, uint8_t reg_addr, uint8_t *data,
                      uint16_t len)
{
    if (!dev || !dev->write_reg || !data)
        return VSD_ERR_INVALID_PARAM;

    return dev->write_reg(dev->ctx, reg_addr, data, len);
}

int hal_imu_read_samples(ImuDevice *dev, uint8_t select_sensor,
                         ImuSensorData *accel, ImuSensorData *gyro, int count)
{
    if (!dev || !dev->read_sample)
        return VSD_ERR_INVALID_PARAM;

    return dev->read_sample(dev->ctx, select_sensor, accel, gyro, count);
}

int hal_imu_set_sensor_default_cfg(ImuDevice *dev)
{
    if (!dev || !dev->set_sensor_default_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_sensor_default_cfg(dev->ctx);
}

int hal_imu_set_accel_cfg(ImuDevice *dev, uint8_t range, uint8_t bwp,
                          uint16_t odr, uint8_t select_type)
{
    if (!dev || !dev->set_accel_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_accel_cfg(dev->ctx, range, bwp, odr, select_type);
}

int hal_imu_set_gyro_cfg(ImuDevice *dev, uint16_t range, uint8_t bwp,
                         uint16_t odr, uint8_t select_type)
{
    if (!dev || !dev->set_gyro_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_gyro_cfg(dev->ctx, range, bwp, odr, select_type);
}

int hal_imu_set_sensor_cfg(ImuDevice *dev, ImuSensorConfig *accel_cfg,
                           ImuSensorConfig *gyro_cfg, ImuAuxCfg *aux_cfg)
{
    if (!dev || (!accel_cfg && !gyro_cfg && !aux_cfg) || !dev->set_sensor_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_sensor_cfg(dev->ctx, accel_cfg, gyro_cfg, aux_cfg);
}

int hal_imu_get_sens_cfg(ImuDevice *dev, ImuSensorConfig *accel_cfg,
                         ImuSensorConfig *gyro_cfg, ImuAuxCfg *aux_cfg)
{
    if (!dev || !dev->get_sensor_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->get_sensor_cfg(dev->ctx, accel_cfg, gyro_cfg, aux_cfg);
}

int hal_imu_get_accel_range(ImuDevice *dev, uint8_t *range)
{
    if (!dev || !dev->get_accel_range) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->get_accel_range(dev->ctx, range);
}

int hal_imu_set_low_power(ImuDevice *dev, uint8_t sensor)
{
    if (!dev || !dev->set_low_power) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_low_power(dev->ctx, sensor);
}

int hal_imu_set_normal(ImuDevice *dev, uint8_t sensor)
{
    if (!dev || !dev->set_normal_mode) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_normal_mode(dev->ctx, sensor);
}

int hal_imu_set_stop(ImuDevice *dev, uint8_t sensor)
{
    if (!dev || !dev->set_stop_mode) {
        return VSD_ERR_INVALID_PARAM;
    }

    return dev->set_stop_mode(dev->ctx, sensor);
}

int hal_imu_get_power_mode(ImuDevice *dev, ImuPmuStatus *pmu_status)
{
    if (!dev || !dev->get_power_mode) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->get_power_mode(dev->ctx, pmu_status);
}

int hal_imu_set_fifo_wm(ImuDevice *dev, uint8_t fifo_wm)
{
    if (!dev || !dev->set_fifo_wm) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->set_fifo_wm(dev->ctx, fifo_wm);
}

int hal_imu_set_fifo_down(ImuDevice *dev, uint8_t fifo_down)
{
    if (!dev || !dev->set_fifo_down) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->set_fifo_down(dev->ctx, fifo_down);
}

int hal_imu_set_fifo_cfg(ImuDevice *dev, uint8_t config, uint8_t enable)
{
    if (!dev || !dev->set_fifo_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->set_fifo_cfg(dev->ctx, config, enable);
}

int hal_imu_flush_fifo(ImuDevice *dev)
{
    if (!dev || !dev->flush_fifo) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->flush_fifo(dev->ctx);
}

int hal_imu_get_fifo_data(ImuDevice *dev)
{
    if (!dev || !dev->get_fifo_data) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->get_fifo_data(dev->ctx);
}

int hal_imu_fifo_parsing_accel(ImuDevice *dev, ImuSensorData *accel_data,
                               int8_t frame_request, uint16_t *available_frame)
{
    if (!dev || !dev->fifo_parsing_accel) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->fifo_parsing_accel(dev->ctx, accel_data, frame_request,
                                   available_frame);
}

int hal_imu_set_interrupt_cfg(ImuDevice *dev, bool enable, uint8_t irq_type,
                              IMUInterruptSetting *irq_settings)
{
    if (!dev || !dev->set_interrupt_cfg) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->set_interrupt_cfg(dev->ctx, enable, irq_type, irq_settings);
}

int hal_imu_check_interrupt_status(ImuDevice *dev, uint8_t *irq_type)
{
    if (!dev || !dev->check_interrupt_status) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->check_interrupt_status(dev->ctx, irq_type);
}

int hal_imu_set_highg_threshold(ImuDevice *dev,
                                IMUHighGCondition highg_condition)
{
    if (!dev || !dev->set_highg_threshold) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->set_highg_threshold(dev->ctx, highg_condition);
}

int hal_imu_reset_step_counter(ImuDevice *dev)
{
    if (!dev || !dev->reset_step_counter) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->reset_step_counter(dev->ctx);
}

int hal_imu_set_step_counter(ImuDevice *dev, uint8_t step_cnt_enable)
{
    if (!dev || !dev->set_step_counter) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->set_step_counter(dev->ctx, step_cnt_enable);
}

int hal_imu_get_step_counter(ImuDevice *dev, uint16_t *step_val)
{
    if (!dev || !dev->get_step_counter) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->get_step_counter(dev->ctx, step_val);
}

int hal_imu_enable_interrupt(ImuDevice *dev, uint8_t pin, bool enable,
                             ImuDataReadyHandler data_ready_callback)
{
    if (!dev || !dev->enable_interrupt) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->enable_interrupt(dev->ctx, pin, enable, data_ready_callback);
}

int hal_imu_enable_power(ImuDevice *dev, bool enable)
{
    if (!dev || !dev->enable_power) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->enable_power(dev->ctx, enable);
}

int hal_imu_perform_self_test(ImuDevice *dev, uint8_t select_sensor)
{
    if (!dev || !dev->selftest) {
        return VSD_ERR_INVALID_PARAM;
    }
    return dev->selftest(dev->ctx, select_sensor);
}
