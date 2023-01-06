/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_rtc.c
 * @brief HAL interface for RTC driver
 */
#include <stdio.h>
#include <stdlib.h>
#include "vsd_common.h"
#include "vsd_error.h"
#include "hal_rtc.h"

static RtcHw *rtc_hw   = NULL;
static bool rtc_synced = false;

static inline RtcOperations *get_ops()
{
    return (RtcOperations *)rtc_hw->ops;
}

static inline const RtcHwConfig *get_hw_cfg()
{
    return rtc_hw->hw_config;
}

/**
 * @brief Create RTC instances
 * @param[in] hw_config Pointer to hardware configuration of this RTC devices
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_create(const RtcHw *hw_config)
{
    if (!hw_config)
        return VSD_ERR_INVALID_POINTER;
    rtc_hw = (RtcHw *)hw_config;
    return VSD_SUCCESS;
}

/**
 * @brief Initialize the real time clock
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_init(void)
{
    if ((!get_ops()->init))
        return VSD_ERR_UNSUPPORTED;
    return get_ops()->init(get_hw_cfg());
}

/**
 * @brief Enable the RTC
 * @param  enable Determine if the RTC is enabled
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_enable(bool enable)
{
    if ((!get_ops()->rtc_enable))
        return VSD_ERR_UNSUPPORTED;
    return get_ops()->rtc_enable(get_hw_cfg(), enable);
}

/**
 * @brief Get time from the RTC
 * @param time pointer to a time structure
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_get_time(RtcTime *time)
{
    if (!get_ops()->get_time)
        return VSD_ERR_UNSUPPORTED;
    if (!time)
        return VSD_ERR_INVALID_POINTER;
    return get_ops()->get_time(get_hw_cfg(), time);
}

/**
 * @brief Get UTC time in millisecond from the RTC
 * @param utc pointer to utc time
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_get_utc_time(uint64_t *utc)
{
    if (!get_ops()->get_utc_time)
        return VSD_ERR_UNSUPPORTED;
    if (!utc)
        return VSD_ERR_INVALID_POINTER;
    return get_ops()->get_utc_time(get_hw_cfg(), utc);
}

/**
 * @brief Set UTC time in millisecond to the RTC
 * @param utc new utc time
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_set_utc_time(uint64_t utc)
{
    int ret;
    if (!get_ops()->set_utc_time)
        return VSD_ERR_UNSUPPORTED;
    ret = get_ops()->set_utc_time(get_hw_cfg(), utc);
    if (ret == VSD_SUCCESS) {
        rtc_synced = true;
    }
    return ret;
}

/**
 * @brief Get alarm time from the RTC
 * @param time pointer to a alarm time
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_get_alarm(uint64_t *time)
{
    if (!get_ops()->get_alarm)
        return VSD_ERR_UNSUPPORTED;
    if (!time)
        return VSD_ERR_INVALID_POINTER;
    return get_ops()->get_alarm(get_hw_cfg(), time);
}

/**
 * @brief Set alarm time from the RTC
 * @param time set new alarm time
 * @param handler A function pointer to the interrupt handler
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_set_alarm(uint64_t time, AlarmIrqHandler handler)
{
    if ((!get_ops()->set_alarm))
        return VSD_ERR_UNSUPPORTED;
    return get_ops()->set_alarm(get_hw_cfg(), time, handler);
}

/**
 * @brief Get boot time in millisecond from the RTC
 * @param time pointer to boot time (ms)
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_get_boot_time(uint64_t *time)
{
    if (!get_ops()->get_boot_time)
        return VSD_ERR_UNSUPPORTED;
    if (!time)
        return VSD_ERR_INVALID_POINTER;
    return get_ops()->get_boot_time(get_hw_cfg(), time, false);
}

/**
 * @brief Get boot time in microsecond(us) from the RTC
 * @param time pointer to boot time (us)
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_get_boot_time_us(uint64_t *time)
{
    if (!get_ops()->get_boot_time)
        return VSD_ERR_UNSUPPORTED;
    if (!time)
        return VSD_ERR_INVALID_POINTER;
    return get_ops()->get_boot_time(get_hw_cfg(), time, true);
}

/**
 * @brief Get RTC synced flag
 * @return Return synced flag
 */
bool hal_rtc_has_synced(void)
{
    return rtc_synced;
}
