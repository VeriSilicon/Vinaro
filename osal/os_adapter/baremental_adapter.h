/**
 * Copyright (C) 2022. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file baremental_adapter.h
 * @brief Definitions for compatibility with bare mental
 */

#ifndef __BAREMENTAL_ADAPTER__
#define __BAREMENTAL_ADAPTER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* osal_lock */
typedef struct OsalMutex {
    void *mutex;
} OsalMutex;
#define OSAL_MUTEXT_WAIT_FOREVER 0xFFFFFFFF

/* osal_notify */
/**
 * @brief - OSAL notify struct
 */
typedef struct OsalNotify {
    void *task_to_notify; // xTaskToNotify;
    unsigned long index_to_notify; // uxIndexToNotify;
    uint32_t notify_value;
    uint32_t *pre_ntfy_val; // pulPreviousNotificationValue;
    long *higher_task_woken; // pxHigherPriorityTaskWoken;
} OsalNotify;

/* osal_semaphore */
typedef struct OsalSemaphore {
    void *semaphore;
} OsalSemaphore;

/* osal_task */
#define osal_enter_critical()
#define osal_exit_critical()

/* types */
#define OSAL_TRUE 1
#define OSAL_FALSE 0

#ifdef __cplusplus
}
#endif

#endif /* __BAREMENTAL_ADAPTER__ */
