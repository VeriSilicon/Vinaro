/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_gpd.c
 * @brief generic power domain HAL
 */

#include "gpd_drv.h"

/**
 * Init power domain with config
 *
 * @param gpd handle of gpd device
 * @param config pointer of gpd config
 *
 * @return 0 on success, error code on failure.
 */
int hal_gpd_init(GpdHw *gpd, const struct GpdHwCfg *config)
{
    if (!gpd || !config || config->type >= GPD_TYPE_COUNT)
        return VSD_ERR_INVALID_PARAM;

    switch (config->type) {
    case GPD_TYPE_BIN:
        gpd->funcs = &bin_funcs;
        break;
    default:
        return VSD_ERR_INVALID_PARAM;
    }
    gpd->config = config;

    if (gpd->funcs->init)
        return gpd->funcs->init(gpd);

    return 0;
}

/**
 * Enable/Disable gpd
 *
 * @param gpd handle of gpd device
 * @param enable true or false
 *
 * @return 0 on success, error code on failure.
 */
int hal_gpd_enable(GpdHw *gpd, bool enable)
{
    if (!gpd)
        return VSD_ERR_INVALID_PARAM;

    if (gpd->funcs->enable)
        gpd->funcs->enable(gpd, enable);

    return 0;
}
