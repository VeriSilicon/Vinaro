/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_notify_api.h
 * @brief OSAL notify APIs
 */

#ifndef __OSAL_NOTIFY_H__
#define __OSAL_NOTIFY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "vs_conf.h"
#include "osal_adapter.h"

/** @addtogroup NOTIFY
 *  OSAL Notify API and definition
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief - OSAL notify waiting struct
 */
typedef struct OsalNotifyWait {
    unsigned long index_to_wait; // uxIndexToWait;
    uint32_t bits_to_clr_on_in; // ulBitsToClearOnEntry;
    uint32_t bits_to_clr_on_out; // ulBitsToClearOnExit;
    uint32_t notify_value;
    uint32_t ticks_to_wait; // xTicksToWait;
} OsalNotifyWait;

/**
 * @brief - OSAL wait for notify
 *
 * @param notify_wait the notify which caller is waiting for
 * @return OSAL_TRUE for success, others for failure
 */
int osal_task_notify_wait(OsalNotifyWait *notify_wait);

/**
 * @brief - OSAL send notify from ISR
 *
 * @param notify the notify which will be send by ISR
 * @return OSAL_TRUE for success, others for failure
 */
int osal_task_notify_from_isr(OsalNotify *notify);

/**
 * @brief - OSAL send notify from task
 *
 * @param notify the notify which will be send by task
 * @return OSAL_TRUE for success, others for failure
 */
int osal_task_notify(OsalNotify *notify);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_NOTIFY_H__ */
