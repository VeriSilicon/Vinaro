/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pmu.h
 * @brief Header of pmu HAL
 */

#ifndef __HAL_PMU_H__
#define __HAL_PMU_H__

#include <stdint.h>
#include <stdbool.h>

enum PmuType {
    PMU_TYPE_BLE_SYSCTRL, /**< sys control */
    PMU_TYPE_COUNT
};

enum PmuWakeUpSource {
    WAKEUP_SOURCE_UNKNOWN,
    WAKEUP_SOURCE_COLD_BOOT,
    WAKEUP_SOURCE_RTC,
    WAKEUP_SOURCE_EXT_IO_0,
    WAKEUP_SOURCE_EXT_IO_1,
    WAKEUP_SOURCE_EXT_IO_2,
    WAKEUP_SOURCE_EXT_IO_3,
    WAKEUP_SOURCE_EXT_IO_4,
    WAKEUP_SOURCE_EXT_IO_5,
};

/**
 * @brief - BLE sysctrl config
 */
typedef struct pmu_ble_sysctrl_cfg {
    const char *name;
    uint8_t type;
    uint32_t sys_base; /**< base address of sysctrl */
    uint32_t bb_base; /**< base address of BLE_BB */
    uint32_t latency; /**< time in ms between suspend and resume */
} pmu_ble_sysctrl_cfg;

struct PmuHwCfg;
struct PmuFuncs;
/**
 * @brief - structure of pmu device.
 */
typedef struct PmuHw {
    const struct PmuHwCfg *config; /**< Platform config of this pmu */
    const struct PmuFuncs *funcs; /**< Function of this pmu */
} PmuHw;

/**
 * Init power management unit
 *
 * @param pmu handle of pmu device
 * @param config pointer of pmu config
 *
 * @return 0 on success, error code on failure.
 */
int hal_pmu_init(PmuHw *pmu, const struct PmuHwCfg *config);

/**
 * enable/disable wakeup source
 *
 * @param pmu handle of pmu device
 * @param source wakeup source enum
 * @param enable true or false
 *
 * @return 0 on success, error code on failure.
 */
int hal_pmu_wakeup_source(PmuHw *pmu, uint8_t source, bool enable);

/**
 * power management unit suspend flow
 *
 * @param pmu handle of pmu device
 *
 * @return 0 on success, error code on failure.
 */
int hal_pmu_suspend(PmuHw *pmu, uint32_t sleep_period);

/**
 * power management unit resume flow
 *
 * @param pmu handle of pmu device
 *
 * @return wakeup source
 */
uint8_t hal_pmu_resume(PmuHw *pmu);

/**
 * time takes between suspend and resume
 *
 * @param pmu handle of pmu device
 *
 * @return time in ms. 0 if unknown
 */
uint32_t hal_pmu_latency_ms(PmuHw *pmu);

/**
 * @brief power management unit enter low power flow
 *
 * @param pmu handle of pmu device
 * @return 0 on success, error code on failure.
 */
int hal_pmu_enter_low_power(PmuHw *pmu);

/**
 * power management unit exit low power flow
 *
 * @param pmu handle of pmu device
 *
 * @return wakeup source
 */
uint8_t hal_pmu_exit_low_power(PmuHw *pmu);

#endif /* __HAL_PMU_H__ */
