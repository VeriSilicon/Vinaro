/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_wdt.h
 * @brief Public head file of HAL for watch dog timer
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#ifndef _HAL_WDT_H_
#define _HAL_WDT_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup WDT
 *  Watch dog timer HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>

#define WDT_DEVICE_MAX (2)

/**
 * @brief WDT interrupt callback handler
 */
typedef void (*WdtIrqHandler)(void);

/**
 * @brief WDT action type select
 */
typedef enum {
    WDT_ACTION_RESET = 0, /**< Reset for timeout */
    WDT_ACTION_INTR  = 1, /**< Trigger interrupt for timeout */
} WdtActionControl;

/**
 * @brief The definition of WDT hardware type
 */
typedef enum WdtType {
    WDT_TYPE_WDT, /**< A WDT */
    WDT_TYPE_IWDT /**< A IWDT */
} WdtType;

/**
 * @brief Structure of WDT config
 */
typedef struct {
    uint32_t timeout_ms; /**< watch dog timeout */
    uint32_t irq2reset_ms; /**< for interrupt mode, expected duration from IRQ
                              to reset if interrupt is not cleared */
    WdtActionControl action; /**< watch dog action type */
    WdtIrqHandler irq_handler; /**< IRQ handler for interrupt mode */
} WdtCfg;

/**
 * @brief Structure of WDT hardware configuration
 */
typedef struct WdtHwConfig {
    uint32_t base; /**< Base address of WDT device */
    uint8_t clk_id; /**<  clk id */
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */
} WdtHwConfig;

/**
 * @brief Structure of device for WDT
 */
typedef struct {
    uint8_t type; /**< WDT type, @see WdtType */
    const WdtHwConfig *hw_config; /**< wdt hardware config see@ WdtHwConfig*/
    const void *ops; /**< wdt operations */
    WdtCfg config; /**< wdt config */
    void *ctx; /**< Private context */
} WdtDevice;

/**
 * @brief Structure of operations for HAL of WDT
 */
typedef struct WdtOperations {
    int (*init)(const WdtDevice *wdt);
    int (*enable)(const WdtDevice *wdt);
    int (*disable)(const WdtDevice *wdt);
    int (*reload)(const WdtDevice *wdt);
    void (*irq_handler)(const WdtDevice *wdt);
} WdtOperations;

/**
 * @brief Add wdt device instance
 * @note This API should be called by wdt IP driver to add initialized instance
 * to HAL, not for application usage
 * @param device The device of wdt will be added
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_add_dev(WdtDevice *wdt);

/**
 * @brief remove wdt device instance
 * @note This API should be called by wdt IP driver to remove instance
 * to HAL, not for application usage
 * @param device The device of wdt will be removed
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_remove_dev(WdtDevice *wdt);

/**
 * @brief Get wdt device instance
 * @param type The type id of wdt device
 * @return Return the wdt instance
 */
WdtDevice *hal_wdt_get_device(uint8_t type);

/**
 * @brief Initialize the watch dog timer
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_init(WdtDevice *wdt);

/**
 * @brief Enable the watch dog timer
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_enable(WdtDevice *wdt);

/**
 * @brief Disable the watch dog timer
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_disable(WdtDevice *wdt);

/**
 * @brief Reload the watch dog counter
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_reload(WdtDevice *wdt);

/**
 * @brief interrupt handler for an External Interrupt
 * @param device the wdt device instance
 */
void hal_wdt_irq_handler(const WdtDevice *wdt);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_WDT_H_ */
