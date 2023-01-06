/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_rtc.h
 * @brief Public head file of HAL for real time clock
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#ifndef _HAL_RTC_H_
#define _HAL_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup RTC
 *  Real Time Clock hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @brief RTC clock source
 */
typedef enum {
    RTC_OSC_ROSC = 0, /**< select ROSC as RTC clock source */
    RTC_OSC_XOSC = 1, /**< select XOSC as RTC clock source */
} RtcOsc;

/**
 * @brief Structure of RTC hardware configuration
 */
typedef struct RtcHwConfig {
    uint32_t base; /**< Base address of RTC device */
    uint32_t tclk_freq; /**< target clk config, when need different accuracy of
                           RTC, tclk_freq is set*/
    uint8_t clk_id; /**<  clk id */
    uint8_t clk_source; /**< RTC clock source */
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */

} RtcHwConfig;

/**
 * @brief Structure of RTC time
 */
typedef struct RtcTime {
    uint16_t tm_year; /*year, need when set*/
    uint8_t tm_mon; /*month, need when set*/
    uint8_t tm_day; /*day in the month, need when set*/
    uint8_t tm_week; /*day of the week, need when set*/
    uint8_t tm_hour; /*hour, need when set*/
    uint8_t tm_min; /*minute, need when set*/
    uint8_t tm_sec; /*second, need when set*/
    uint16_t tm_ms; /*millisecond, need when set*/
} RtcTime;

/**
 * @brief RTC interrupt callback handler
 */
typedef void (*AlarmIrqHandler)(void);

/**
 * @brief Structure of operations for HAL of RTC
 */
typedef struct RtcOperations {
    int (*init)(const RtcHwConfig *hw_config);
    int (*rtc_enable)(const RtcHwConfig *hw_config, bool enable);
    int (*get_time)(const RtcHwConfig *hw_config, RtcTime *time);
    int (*get_utc_time)(const RtcHwConfig *hw_config, uint64_t *utc);
    int (*set_utc_time)(const RtcHwConfig *hw_config, uint64_t utc);
    int (*get_alarm)(const RtcHwConfig *hw_config, uint64_t *time);
    int (*set_alarm)(const RtcHwConfig *hw_config, uint64_t time,
                     AlarmIrqHandler handler);
    int (*get_boot_time)(const RtcHwConfig *hw_config, uint64_t *time,
                         bool unit_us);
} RtcOperations;

/**
 * @brief This struct define RTC hardware
 */
typedef struct {
    const RtcHwConfig *hw_config;
    const RtcOperations *ops;
} RtcHw;

/**
 * @brief Create RTC instances
 * @param[in] hw_config Pointer to hardware configuration of this RTC devices
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_create(const RtcHw *hw_config);

/**
 * @brief Initialize the real time clock
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_init(void);

/**
 * @brief Enable the RTC
 * @param  enable Determine if the RTC is enabled
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_enable(bool enable);

/**
 * @brief Get time from the RTC
 * @param time pointer to a time structure
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_get_time(RtcTime *time);

/**
 * @brief Get UTC time in millisecond to the RTC
 * @param utc pointer to utc time
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_get_utc_time(uint64_t *utc);

/**
 * @brief Set UTC time in millisecond to the RTC
 * @param utc new utc time
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_set_utc_time(uint64_t utc);

/**
 * @brief Get alarm time from the RTC
 * @param time pointer to a alarm time
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_get_alarm(uint64_t *time);

/**
 * @brief Set alarm time from the RTC
 * @param time set new alarm time
 * @param handler A function pointer to the interrupt handler
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_rtc_set_alarm(uint64_t time, AlarmIrqHandler handler);

/**
 * @brief Get boot time in millisecond from the RTC
 * @param time pointer to boot time (ms)
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_get_boot_time(uint64_t *time);

/**
 * @brief Get boot time in microsecond(us) from the RTC
 * @param time pointer to boot time (us)
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_get_boot_time_us(uint64_t *time);

/**
 * @brief Get RTC synced flag
 * @return Return synced flag
 */
bool hal_rtc_has_synced(void);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_RTC_H_ */
