/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_wdt.c
 * @brief HAL for watch dog driver
 */
#include <stdlib.h>
#include "hal_wdt.h"
#include "vsd_error.h"
#include "vsd_common.h"

static WdtDevice *g_wdt_dev[WDT_DEVICE_MAX] = { NULL };

static inline WdtOperations *get_ops(const WdtDevice *wdt)
{
    return (WdtOperations *)wdt->ops;
}

/**
 * @brief Add wdt device instance
 * @note This API should be called by wdt IP driver to add initialized instance
 * to HAL, not for application usage
 * @param device The device of wdt will be added
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_add_dev(WdtDevice *wdt)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_wdt_dev) / sizeof(g_wdt_dev[0]); i++) {
        if (g_wdt_dev[i] == NULL) {
            g_wdt_dev[i] = wdt;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

/**
 * @brief remove wdt device instance
 * @note This API should be called by wdt IP driver to remove instance
 * to HAL, not for application usage
 * @param device The device of wdt will be removed
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_remove_dev(WdtDevice *wdt)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_wdt_dev) / sizeof(g_wdt_dev[0]); i++) {
        if (g_wdt_dev[i] == wdt) {
            g_wdt_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

/**
 * @brief Get wdt device instance
 * @param type The type id of wdt device
 * @return Return the wdt instance
 */
WdtDevice *hal_wdt_get_device(uint8_t type)
{
    uint8_t i;
    for (i = 0; i < sizeof(g_wdt_dev) / sizeof(g_wdt_dev[0]); i++) {
        if (g_wdt_dev[i]->type == type) {
            return g_wdt_dev[i];
        }
    }
    return NULL;
}

/**
 * @brief Initialize the watch dog timer
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_init(WdtDevice *wdt)
{
    if (!wdt)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(wdt)->init)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(wdt)->init(wdt);
}

/**
 * @brief Enable the watch dog timer
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_enable(WdtDevice *wdt)
{
    if (!wdt)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(wdt)->enable)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(wdt)->enable(wdt);
}

/**
 * @brief Disable the watch dog timer
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_disable(WdtDevice *wdt)
{
    if (!wdt)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(wdt)->disable)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(wdt)->disable(wdt);
}

/**
 * @brief Reload the watch dog counter
 * @param wdt The watch dog device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_wdt_reload(WdtDevice *wdt)
{
    if (!wdt)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(wdt)->reload)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(wdt)->reload(wdt);
}

/**
 * @brief interrupt handler for an External Interrupt
 * @param device the wdt device instance
 */
void hal_wdt_irq_handler(const WdtDevice *wdt)
{
    if (!wdt)
        return;
    if (!get_ops(wdt)->irq_handler)
        return;

    return get_ops(wdt)->irq_handler(wdt);
}
