/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 *
 * @file osal_time.c
 * @brief OSAL time APIs
 */

#include "vs_conf.h"
#include "osal_time_api.h"
#include "vs_debug.h"

#if CONFIG_FREERTOS
/**
 * @brief Convert ticks to time in ms
 *
 * @param ticks number of ticks
 * @return TickType_t time in ms
 */
TickType_t osal_tick_to_ms(TickType_t ticks)
{
    return ticks * OSAL_TICK_PERIOD_MS;
}

uint64_t osal_get_uptime(void)
{
    return (uint64_t)osal_tick_to_ms(xTaskGetTickCount());
}

/**
 * @brief Gets the time that less then one tick in microseconds
 */
static uint64_t get_remain_time_us(void)
{
    uint64_t cur_ticks, cmp_ticks, time_us;
#if CONFIG_RISCV_ARCH
    cur_ticks = SysTimer_GetLoadValue() % getSystickReloadDiffVal();
    cmp_ticks = getSystickReloadDiffVal();
    // increasing count value
    time_us = ((double)cur_ticks / cmp_ticks) * OSAL_TICK_PERIOD_MS * 1000;
#elif CONFIG_ZSP_ARCH
    cur_ticks = ZSP_timer_read(TIMER0);
    cmp_ticks = getSystickCompVal();
    // decreasing count value
    time_us = ((double)(cmp_ticks - cur_ticks) / cmp_ticks) *
              OSAL_TICK_PERIOD_MS * 1000;
#endif
    return time_us;
}

uint64_t osal_get_uptime_us(void)
{
    uint64_t boot_us, check_boot_us;
    uint64_t remain_us;
    boot_us   = osal_get_uptime() * 1000;
    remain_us = get_remain_time_us();
    // Read and verify the register value again, in case the timer just timed
    // out when reading the register value.
    check_boot_us = osal_get_uptime() * 1000;
    if (check_boot_us != boot_us) {
        return check_boot_us + get_remain_time_us();
    }
    return boot_us + remain_us;
}

void osal_setup_timer(void)
{
    return prvSetupTimerInterrupt();
}

void osal_update_systick(void)
{
    return updateSysTickTimer();
}

uint32_t osal_ms_to_tick(uint32_t duration)
{
    return duration / OSAL_TICK_PERIOD_MS;
}
#endif /* CONFIG_FREERTOS */

#if CONFIG_BAREMETAL
uint64_t osal_get_uptime(void)
{
    return 0;
}
#endif /* CONFIG_BAREMETAL */
