/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pmu.c
 * @brief HAL interface for PMU
 */

#include "pmu_drv.h"
#include "vsd_error.h"
#include "hal_pmu.h"

/**
 * Init power management unit
 *
 * @param pmu handle of pmu device
 * @param config pointer of pmu config
 *
 * @return 0 on success, error code on failure.
 */
int hal_pmu_init(PmuHw *pmu, const struct PmuHwCfg *config)
{
    if (!pmu || !config || (config->type >= PMU_TYPE_COUNT))
        return VSD_ERR_INVALID_PARAM;

    /* already inited */
    if (pmu->config && pmu->funcs)
        return VSD_ERR_INVALID_STATE;

    switch (config->type) {
    case PMU_TYPE_BLE_SYSCTRL:
        pmu->funcs = &ble_sysctrl_funcs;
        break;
    default:
        return VSD_ERR_INVALID_PARAM;
    }
    pmu->config = config;

    if (pmu->funcs->init)
        return pmu->funcs->init(pmu->config);

    return 0;
}

/**
 * enable/disable wakeup source
 *
 * @param pmu handle of pmu device
 * @param source wakeup source enum
 * @param enable true or false
 *
 * @return 0 on success, error code on failure.
 */
int hal_pmu_wakeup_source(PmuHw *pmu, uint8_t source, bool enable)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->wakeup_source || !pmu->config)
        return VSD_ERR_INVALID_PARAM;

    return pmu->funcs->wakeup_source(pmu->config, source, enable);
}

/**
 * power management unit suspend flow
 *
 * @param pmu handle of pmu device
 *
 * @return 0 on success, error code on failure.
 */
int hal_pmu_suspend(PmuHw *pmu, uint32_t sleep_period)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->suspend || !pmu->config)
        return VSD_ERR_INVALID_PARAM;

    return pmu->funcs->suspend(pmu->config, sleep_period);
}

/**
 * power management unit resume flow
 *
 * @param pmu handle of pmu device
 *
 * @return wakeup source
 */
uint8_t hal_pmu_resume(PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->resume || !pmu->config)
        return WAKEUP_SOURCE_UNKNOWN;

    return pmu->funcs->resume(pmu->config);
}

/**
 * time takes between suspend and resume
 *
 * @param pmu handle of pmu device
 *
 * @return time in ms. 0 if unknown
 */
uint32_t hal_pmu_latency_ms(PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->latency || !pmu->config)
        return 0;

    return pmu->funcs->latency(pmu->config);
}

/**
 * @brief power management unit enter low power flow
 *
 * @param pmu handle of pmu device
 * @return 0 on success, error code on failure.
 */
int hal_pmu_enter_low_power(PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->enter_low_power || !pmu->config)
        return VSD_ERR_INVALID_PARAM;

    return pmu->funcs->enter_low_power(pmu->config);
}

/**
 * power management unit exit low power flow
 *
 * @param pmu handle of pmu device
 *
 * @return wakeup source
 */
uint8_t hal_pmu_exit_low_power(PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->exit_low_power || !pmu->config)
        return WAKEUP_SOURCE_UNKNOWN;

    return pmu->funcs->exit_low_power(pmu->config);
}
