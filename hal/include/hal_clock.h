/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 *
 * @file hal_clock.h
 * @brief Clock HAL interface
 */

#ifndef __HAL_CLOCK_H__
#define __HAL_CLOCK_H__

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup CLOCK
 *  Clock hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define CLK_INVALID 0xFF
#define CLK_ROOT 0xFE

enum ClkType {
    CLK_TYPE_FIX, /**< clock root like crystal */
    CLK_TYPE_GATE, /**< can be on/off */
    CLK_TYPE_DIV, /**< can divide from parent clock */
    CLK_TYPE_FIX_DIV, /**< can divide from parent clock with fixed divider */
    CLK_TYPE_MUX, /**< can select different clock source */
    CLK_TYPE_COMP, /**< gate, mux and div composite clock */
    CLK_TYPE_PLL, /**< pll clock */
    CLK_TYPE_COUNT /**< number of clock type */
};

struct ClkHwCfg;
struct ClkFuncs;

/**
 * @brief Structure of clock device.
 * @param config Platform config of this clock.
 * @param funcs Function of this clock.
 */
typedef struct ClkHw {
    const struct ClkHwCfg *config; /**< Config of clock HW */
    const struct ClkFuncs *funcs; /**< Functions for clock */
} ClkHw;

/**
 * Init clock device
 *
 * @param clk handle of clock device
 * @param config pointer of clock config
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_init(ClkHw *clk, const struct ClkHwCfg *config);

/**
 * Set new clock rate
 *
 * @param clk handle of clock device
 * @param rate Target clock rate in Hz
 * @param parent_rate parent clock rate in Hz
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_set_rate(ClkHw *clk, uint32_t rate, uint32_t parent_rate);

/**
 * calculate clock' rate with HW setting
 *
 * @param clk handle of clock device
 * @param parent_rate parent clock rate in Hz
 *
 * @return clock rate in Hz on success,
 *         parent rate on failure or clock not change rate.
 */
uint32_t hal_clk_recalc_rate(ClkHw *clk, uint32_t parent_rate);

/**
 * Set clock' new parent
 *
 * @param clk handle of clock device
 * @param parent_id New parent clock ID
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_set_parent(ClkHw *clk, uint8_t parent_id);

/**
 * Get clock' parent
 *
 * @param clk handle of clock device
 *
 * @return perent ID on success, CLK_INVALID on failure.
 */
uint8_t hal_clk_get_parent(ClkHw *clk);

/**
 * Enable/Disable clock
 *
 * @param clk handle of clock device
 * @param enable true or false
 *
 * @return 0 on success, error code on failure.
 */
int hal_clk_enable(ClkHw *clk, bool enable);

/** @} */

#endif /* __HAL_CLOCK_H__ */
