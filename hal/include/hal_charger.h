/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_charger.h
 * @brief header of HAL for charger
 * @author Xie Mengdong <Mengdong.Xie@verisilicon.com>
 */

#ifndef __HAL_CHARGER_H
#define __HAL_CHARGER_H

#include "vsd_common.h"
#include "hal_gpio.h"

/** @addtogroup CHARGER
 *  Charger hal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @brief Charger state updating call back function
 */
typedef void (*ChargerUpdateHandler)(void);

/**
 * @brief config one control issue of charger, enable or disable it
 */
typedef enum HalChargerCtr {
    HAL_CHARGER_CTR_ENABLE, /**< enable control issue */
    HAL_CHARGER_CTR_DISABLE, /**< disable control issue */
} HalChargerCtr;

/**
 * @brief charge status of charger
 */
typedef enum ChargerStat {
    HAL_CHARGER_NOT_CHARGING, /**< charger is not charging */
    HAL_CHARGER_IN_PRECHARGE, /**< charger is in pre-charge mode */
    HAL_CHARGER_IN_CHARGE, /**< charger is in charge mode */
    HAL_CHARGER_CHARGE_DONE, /**< charger is in charge done mode */
} ChargerStat;

/**
 * @brief capabilities of charger, all bits should be 0 to represent this
 * capability is not supported, a specific charger should update them in
 * device_init according to its specific capability.
 */
typedef struct ChargerCapa {
    uint32_t max_in_curr : 1; /**< max input current */
    uint32_t min_in_vol : 1; /**< min input voltage */
    uint32_t batt_uvlo : 1; /**< battery under-voltage lockout */
    uint32_t wdt_timer : 1; /**< watchdog timer */
    uint32_t ctr_ext_power : 1; /**< on or off of external power */
    uint32_t ctr_charge : 1; /**< control on or off of charge */
    uint32_t ctr_batt : 1; /**< control on or off of battery */
} ChargerCapa;

/**
 * @brief signals of charger
 */
typedef struct ChargerSignal {
    uint32_t charge_stat : 3; /**< charge status of charger */
    uint32_t power_good : 1; /**< power good or not */
    uint32_t ntc_fault_hot : 1; /**< ntc hot protection */
    uint32_t ntc_fault_cold : 1; /**< ntc cold protection */
    uint32_t safety_timer_fault : 1; /**< safety timer expires */
    uint32_t batt_ovp_fault : 1; /**< battery over-voltage */
    uint32_t thermal_shutdown : 1; /**< thermal shutdown protection */
    uint32_t input_fault : 1; /**< input source is bad */
} ChargerSignal;

/**
 * @brief config parameters of charger, but it should be noticed that one
 * specific charger may doesn't support some of the following features.
 */
typedef struct ChargerConfig {
    uint16_t max_in_curr; /**< max input current from USB, mA */
    uint16_t min_in_vol; /**< min input voltage of USB, mV */
    uint16_t charge_curr; /**< charge current to the battery from USB, mA */
    uint16_t batt_vol; /**< charge voltage of battery, mV */
    uint16_t sys_vol; /**< voltage to the system from battery, mV */
    uint16_t max_discharge_curr; /**< max discharge current from battery, mA */
    uint16_t batt_uvlo; /**< battery under-voltage lockout threshold, mV */
    uint16_t wdt_count; /**< watchdog timer counter, s */
} ChargerConfig;

struct ChargerDevice;

/**
 * @brief all operations of charger in HAL
 */
typedef struct ChargerOpers {
    int (*init)(struct ChargerDevice *device, ChargerConfig *config);
    int (*destroy)(struct ChargerDevice *device);
    int (*reset)(struct ChargerDevice *device);
    int (*set_max_in_curr)(struct ChargerDevice *device, uint16_t current);
    int (*set_min_in_vol)(struct ChargerDevice *device, uint16_t voltage);
    int (*ctr_ext_power)(struct ChargerDevice *device, HalChargerCtr control);
    int (*ctr_charge)(struct ChargerDevice *device, HalChargerCtr control);
    int (*set_batt_uvlo)(struct ChargerDevice *device, uint16_t uvlo);
    int (*set_charge_curr)(struct ChargerDevice *device, uint16_t current);
    int (*set_discharge_curr)(struct ChargerDevice *device, uint16_t current);
    int (*set_batt_vol)(struct ChargerDevice *device, uint16_t voltage);
    int (*set_wdt_timer)(struct ChargerDevice *device, uint16_t timer);
    int (*feed_wdt)(struct ChargerDevice *device);
    int (*ctr_batt)(struct ChargerDevice *device, HalChargerCtr control);
    int (*set_sys_vol)(struct ChargerDevice *device, uint16_t voltage);
    int (*update_status)(struct ChargerDevice *device);
    int (*get_state)(struct ChargerDevice *device, ChargerStat *state);
    int (*enable_interrupt)(struct ChargerDevice *device, bool enable,
                            ChargerUpdateHandler callback);
} ChargerOpers;

/**
 * @brief device definition of charger
 */
typedef struct ChargerDevice {
    BusDevice bus_device; /**< bus device that charger mounts */
    BusConfig bus_config; /**< bus configuration */
    const GpioPort *irq_pin; /**< interrupt pin */
    ChargerCapa hw_capability; /**< capability supported by hardware */
    ChargerCapa capability; /**< final capability of charger */
    ChargerSignal signal; /**< signal comes from a charger */
    ChargerOpers opers; /**< operations of a charger */
    ChargerUpdateHandler irq_callback; /**< Callback for state updating */
} ChargerDevice;

/**
 * @brief initialize charger
 * @param device charger device
 * @param config config parameters of charger
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure.
 */
int hal_charger_init(ChargerDevice *device, ChargerConfig *config);

/**
 * @brief destroy charger device
 * @param device charger device
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure.
 */
int hal_charger_destroy(ChargerDevice *device);

/**
 * @brief Update status of charger to charger device
 * @param device charger device
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure.
 */
int hal_charger_update_status(ChargerDevice *device);

/**
 * @brief reset charger
 * @param device charger device
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure.
 */
int hal_charger_reset(ChargerDevice *device);

/**
 * @brief set charger's maximum input current from UAB or AC adapter
 * @param device charger device
 * @param current current to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_max_in_curr(ChargerDevice *device, uint16_t current);

/**
 * @brief set charger's minimum input voltage from USB or AC adapter
 * @param device charger device
 * @param voltage voltage to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_min_in_vol(ChargerDevice *device, uint16_t voltage);

/**
 * @brief control charger to receive external power from UAB or AC adapter or
 * not
 * @param device charger device
 * @param control enable or disable to connect or disconnect charger with USB or
 * AC adapter
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_ctr_ext_power(ChargerDevice *device, HalChargerCtr control);

/**
 * @brief control charge or not charge of charger
 * @param device charger device
 * @param control enable oe disable charge feature
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_ctr_charge(ChargerDevice *device, HalChargerCtr control);

/**
 * @brief set under-voltage lockout threshold of battery controlled by charger
 * @param device charger device
 * @param batt_uvlo under-voltage lockout threshold to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_batt_uvlo(ChargerDevice *device, uint16_t batt_uvlo);

/**
 * @brief set charger current to battery
 * @param device charger device
 * @param current charge current to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_charge_curr(ChargerDevice *device, uint16_t current);

/**
 * @brief set discharge current to system
 * @param device charger device
 * @param current discharge current to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_discharge_curr(ChargerDevice *device, uint16_t current);

/**
 * @brief set charge voltage of battery
 * @param device charger device
 * @param voltage charge voltage to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_batt_vol(ChargerDevice *device, uint16_t voltage);

/**
 * @brief set watchdog timer counter of charger
 * @param device charger device
 * @param timer timer counter to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_wdt_timer(ChargerDevice *device, uint16_t timer);

/**
 * @brief feed watchdog to avoid reset of charger
 * @param device charger device
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_feed_wdt(ChargerDevice *device);

/**
 * @brief control connection between battery and system
 * @param device charger device
 * @param control enable or disable
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_ctr_batt(ChargerDevice *device, HalChargerCtr control);

/**
 * @brief set voltage supplied to system
 * @param device charger device
 * @param voltage system voltage to set
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure, but if retval is VSD_ERR_UNSUPPORTED, you should ignore it.
 */
int hal_charger_set_sys_vol(ChargerDevice *device, uint16_t voltage);

/**
 * @brief Enable/Disable interrupt for charger
 * @param device charger device
 * @param enable true for enable, false for disable
 * @param callback The callback of charger state updating
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_charger_enable_interrupt(ChargerDevice *device, bool enable,
                                 ChargerUpdateHandler callback);

/** @} */

#endif /* __HAL_CHARGER_H */
