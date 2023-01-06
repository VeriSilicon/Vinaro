/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_clock.c
 * @brief Clock HAL
 */

#include "hal_clock.h"
#include "clk_drv.h"
#include "vsd_error.h"

/**
 * Init clock device
 *
 * @param clk handle of clock device
 * @param config pointer of clock config
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_init(ClkHw *clk, const ClkHwCfg *config)
{
    if (!clk || !config || (config->type >= CLK_TYPE_COUNT))
        return VSD_ERR_INVALID_PARAM;

    switch (config->type) {
    case CLK_TYPE_FIX:
        clk->funcs = &fix_funcs;
        break;
    case CLK_TYPE_GATE:
        clk->funcs = &gate_funcs;
        break;
    case CLK_TYPE_DIV:
        clk->funcs = &div_funcs;
        break;
    case CLK_TYPE_FIX_DIV:
        clk->funcs = &fix_div_funcs;
        break;
    case CLK_TYPE_MUX:
        clk->funcs = &mux_funcs;
        break;
    case CLK_TYPE_COMP:
        clk->funcs = &comp_funcs;
        break;
    case CLK_TYPE_PLL:
        clk->funcs = &pll_funcs;
        break;
    default:
        return VSD_ERR_INVALID_PARAM;
    }
    clk->config = config;

    if (clk->funcs->init)
        return clk->funcs->init(clk->config);

    return 0;
}

/**
 * Set new clock rate
 *
 * @param clk handle of clock device
 * @param rate Target clock rate in Hz
 * @param parent_rate parent clock rate in Hz
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_set_rate(ClkHw *clk, uint32_t rate, uint32_t parent_rate)
{
    if (!clk)
        return VSD_ERR_INVALID_PARAM;

    if (clk->funcs->set_rate)
        return clk->funcs->set_rate(clk->config, rate, parent_rate);

    return VSD_ERR_UNSUPPORTED;
}

/**
 * calculate clock' rate with HW setting
 *
 * @param clk handle of clock device
 * @param parent_rate parent clock rate in Hz
 *
 * @return clock rate in Hz on success,
 *         parent rate on failure or clock not change rate.
 */
uint32_t hal_clk_recalc_rate(ClkHw *clk, uint32_t parent_rate)
{
    if (!clk)
        return parent_rate;

    if (clk->funcs->recalc_rate)
        return clk->funcs->recalc_rate(clk->config, parent_rate);

    return parent_rate;
}

/**
 * Set clock' new parent
 *
 * @param clk handle of clock device
 * @param parent_id New parent clock ID
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_set_parent(ClkHw *clk, uint8_t parent_id)
{
    if (!clk)
        return VSD_ERR_INVALID_PARAM;

    if (clk->funcs->set_parent)
        return clk->funcs->set_parent(clk->config, parent_id);

    return VSD_ERR_UNSUPPORTED;
}

/**
 * Get clock' parent
 *
 * @param clk handle of clock device
 *
 * @return perent ID on success, CLK_INVALID on failure.
 */
uint8_t hal_clk_get_parent(ClkHw *clk)
{
    if (!clk)
        return CLK_INVALID;

    if (clk->funcs->get_parent)
        return clk->funcs->get_parent(clk->config);

    return CLK_INVALID;
}

/**
 * Enable/Disable clock
 *
 * @param clk handle of clock device
 * @param enable true or false
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_enable(ClkHw *clk, bool enable)
{
    if (!clk)
        return VSD_ERR_INVALID_PARAM;

    if (clk->funcs->enable)
        clk->funcs->enable(clk->config, enable);

    return 0;
}
