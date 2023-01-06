/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 *
 * @file osal_time_api.h
 * @brief OSAL time APIs
 */

#ifndef __OSAL_TIME_H__
#define __OSAL_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "osal_adapter.h"

/** @addtogroup TIME
 *  OSAL Time API
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief Get OS boot time
 * @retval uint64_t The number of milliseconds since vTaskStartScheduler was
 * called
 */
uint64_t osal_get_uptime(void);

/**
 * @brief Get OS boot time in us
 * @return uint64_t The number of microseconds since vTaskStartScheduler was
 * called
 */
uint64_t osal_get_uptime_us(void);

/**
 * @brief Setup the OS timer to generate the tick
 */
void osal_setup_timer(void);

/**
 * @brief Update the interrupt frequency of SysTick
 */
void osal_update_systick(void);

/**
 * @brief Convert time in ms to ticks
 *
 * @param duration a period of time in ms
 * @return uint32_t number of ticks
 */
uint32_t osal_ms_to_tick(uint32_t duration);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_TIME_H__ */
