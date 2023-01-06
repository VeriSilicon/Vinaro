/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_battery.c
 * @brief HAL interface for battery
 */

#include "vs_conf.h"
#include "vsd_common.h"
#include "hal_lb_adc.h"
#include "hal_battery.h"
#include "vsd_error.h"

static const BatteryOperations *bat_ops;

int hal_battery_create(const BatteryOperations *ops)
{
    if (ops) {
        bat_ops = ops;
        return VSD_SUCCESS;
    } else {
        return VSD_ERR_INVALID_POINTER;
    }
}

int hal_battery_init(void)
{
    if (!bat_ops || !bat_ops->init)
        return VSD_ERR_INVALID_POINTER;

    return bat_ops->init();
}

int hal_battery_read_capacity(BatteryDev *bat)
{
    if (!bat_ops || !bat_ops->read_capacity)
        return VSD_ERR_INVALID_POINTER;

    return bat_ops->read_capacity(&bat->capacity);
}

int hal_battery_read_level(BatteryDev *bat)
{
    if (!bat_ops || !bat_ops->read_level)
        return VSD_ERR_INVALID_POINTER;

    return bat_ops->read_level(&bat->level);
}
