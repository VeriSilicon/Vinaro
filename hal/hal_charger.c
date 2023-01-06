/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_charger.c
 * @brief source code of HAL for charger
 * @author Xie Mengdong <Mengdong.Xie@verisilicon.com>
 */

#include "hal_charger.h"
#include "vsd_error.h"

int hal_charger_init(ChargerDevice *device, ChargerConfig *config)
{
    if (!device || !config || !device->opers.init) {
        return VSD_ERR_INVALID_PARAM;
    }

    /* update hardware copability to capability */
    device->capability.max_in_curr   = device->hw_capability.max_in_curr;
    device->capability.min_in_vol    = device->hw_capability.min_in_vol;
    device->capability.batt_uvlo     = device->hw_capability.batt_uvlo;
    device->capability.wdt_timer     = device->hw_capability.wdt_timer;
    device->capability.ctr_ext_power = device->hw_capability.ctr_ext_power;
    device->capability.ctr_charge    = device->hw_capability.ctr_charge;
    device->capability.ctr_batt      = device->hw_capability.ctr_batt;

    /* initialize a specific charger */
    return device->opers.init(device, config);
}

int hal_charger_destroy(ChargerDevice *device)
{
    if (!device || !device->opers.destroy) {
        return VSD_ERR_INVALID_PARAM;
    }

    return device->opers.destroy(device);
}

int hal_charger_reset(ChargerDevice *device)
{
    if (!device || !device->opers.reset) {
        return VSD_ERR_INVALID_PARAM;
    }

    return device->opers.reset(device);
}

int hal_charger_set_max_in_curr(ChargerDevice *device, uint16_t current)
{
    if (!device || !device->opers.set_max_in_curr) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.max_in_curr == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.set_max_in_curr(device, current);
}

int hal_charger_set_min_in_vol(ChargerDevice *device, uint16_t voltage)
{
    if (!device || !device->opers.set_min_in_vol) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.min_in_vol == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.set_min_in_vol(device, voltage);
}

int hal_charger_ctr_ext_power(ChargerDevice *device, HalChargerCtr control)
{
    if (!device || !device->opers.ctr_ext_power) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.ctr_ext_power == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.ctr_ext_power(device, control);
}

int hal_charger_ctr_charge(ChargerDevice *device, HalChargerCtr control)
{
    if (!device || !device->opers.ctr_charge) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.ctr_charge == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.ctr_charge(device, control);
}

int hal_charger_set_batt_uvlo(ChargerDevice *device, uint16_t batt_uvlo)
{
    if (!device || !device->opers.set_batt_uvlo) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.batt_uvlo == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.set_batt_uvlo(device, batt_uvlo);
}

int hal_charger_set_charge_curr(ChargerDevice *device, uint16_t current)
{
    if (!device || !device->opers.set_charge_curr) {
        return VSD_ERR_INVALID_PARAM;
    }

    return device->opers.set_charge_curr(device, current);
}

int hal_charger_set_discharge_curr(ChargerDevice *device, uint16_t current)
{
    if (!device || !device->opers.set_discharge_curr) {
        return VSD_ERR_INVALID_PARAM;
    }

    return device->opers.set_discharge_curr(device, current);
}

int hal_charger_set_batt_vol(ChargerDevice *device, uint16_t voltage)
{
    if (!device || !device->opers.set_batt_vol) {
        return VSD_ERR_INVALID_PARAM;
    }

    return device->opers.set_batt_vol(device, voltage);
}

int hal_charger_set_wdt_timer(ChargerDevice *device, uint16_t timer)
{
    if (!device || !device->opers.set_wdt_timer) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.wdt_timer == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.set_wdt_timer(device, timer);
}

int hal_charger_feed_wdt(ChargerDevice *device)
{
    if (!device || !device->opers.feed_wdt) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.wdt_timer == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.feed_wdt(device);
}

int hal_charger_ctr_batt(ChargerDevice *device, HalChargerCtr control)
{
    if (!device || !device->opers.ctr_batt) {
        return VSD_ERR_INVALID_PARAM;
    }
    /* this feature is not supported by a specific charger */
    if (device->capability.ctr_batt == 0) {
        return VSD_ERR_UNSUPPORTED;
    }

    return device->opers.ctr_batt(device, control);
}

int hal_charger_set_sys_vol(ChargerDevice *device, uint16_t voltage)
{
    if (!device || !device->opers.set_sys_vol) {
        return VSD_ERR_INVALID_PARAM;
    }

    return device->opers.set_sys_vol(device, voltage);
}

int hal_charger_enable_interrupt(ChargerDevice *device, bool enable,
                                 ChargerUpdateHandler callback)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;
    if (!device->opers.enable_interrupt)
        return VSD_ERR_UNSUPPORTED;

    return device->opers.enable_interrupt(device, enable, callback);
}

int hal_charger_update_status(ChargerDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (!device->opers.update_status)
        return VSD_ERR_UNSUPPORTED;

    return device->opers.update_status(device);
}
