/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_gpd.h
 * @brief Header of generic power domain HAL
 */

#ifndef __HAL_GPD_H__
#define __HAL_GPD_H__

/** @addtogroup GPD
 *  Generic Power Domain hal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>

enum GpdType {
    GPD_TYPE_BIN, /**< binary */
    GPD_TYPE_COUNT
};

/**
 * flags
 *
 * GPD_SET_TO_DISBALE - by default sets the bit at bit_idx to enable the
 *    power domain. Setting this flag does the opposite: setting the bit
 *    disable the pd and clearing it enables the pd
 */
#define GPD_BIN_SET_TO_DISBALE BIT(0)
#define GPD_BIN_IS_CRITICAL BIT(1)
/**
 * @brief - binary power domain config structure
 */
typedef struct GpdBinCfg {
    uint8_t type;
    uint8_t bit_idx;
    uint8_t flags;
    uint32_t reg;
} GpdBinCfg;

struct GpdHwCfg;
struct GpdFuncs;
/**
 * @brief - structure of power domain device.
 */
typedef struct GpdHw {
    const struct GpdHwCfg *config; /**< Platform config of the power domain */
    const struct GpdFuncs *funcs; /**< Function of this power domain */
} GpdHw;

int hal_gpd_init(GpdHw *gpd, const struct GpdHwCfg *config);
int hal_gpd_enable(GpdHw *gpd, bool enable);

/** @} */

#endif /* __HAL_GPD_H__ */
