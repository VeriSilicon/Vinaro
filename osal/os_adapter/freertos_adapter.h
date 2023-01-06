/**
 * Copyright (C) 2022. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file freertos_adapter.h
 * @brief Definitions for compatibility with FreeRTOS
 */

#ifndef __FREERTOS_ADAPTER__
#define __FREERTOS_ADAPTER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "vs_conf.h"
#if CONFIG_RISCV_ARCH
#include "platform.h"
#endif
#if CONFIG_SOC_CACHE_SUPPORT
/** Non cacheable memory is required and is managed by FreeRTOS */
#include "heap_4_noncache.h"
#endif

/* osal_lock */
#define OSAL_MUTEXT_WAIT_FOREVER portMAX_DELAY
typedef struct OsalMutex {
    SemaphoreHandle_t mutex; /* FreeRTOS mutex */
} OsalMutex;

/* osal_notify */
/**
 * @brief - OSAL notify struct
 */
typedef struct OsalNotify {
    void *task_to_notify; // xTaskToNotify;
    unsigned long index_to_notify; // uxIndexToNotify;
    uint32_t notify_value;
    eNotifyAction action;
    uint32_t *pre_ntfy_val; // pulPreviousNotificationValue;
    long *higher_task_woken; // pxHigherPriorityTaskWoken;
} OsalNotify;

/* osal_semaphore */
typedef struct OsalSemaphore {
    SemaphoreHandle_t semaphore;
} OsalSemaphore;

/* sys_state */
/*
 * Macros used to indicate which state a task is in.
 */
#define TASK_RUNNING_CHAR ('X')
#define TASK_BLOCKED_CHAR ('B')
#define TASK_READY_CHAR ('R')
#define TASK_DELETED_CHAR ('D')
#define TASK_SUSPENDED_CHAR ('S')
/*
 * Macros used to indicate indicate whether the task is alive.
 */
#define TASK_ALIVE_CHAR ('Y')
#define TASK_DEAD_CHAR ('N')
#define EACH_TASK_INFO_SIZE 64

/* osal_task */
#define osal_enter_critical() taskENTER_CRITICAL()
#define osal_exit_critical() taskEXIT_CRITICAL()
#define OSAL_TASK_PRI_HIGHEST 255

/* osal_time */
#define OSAL_TICK_PERIOD_MS portTICK_PERIOD_MS

/* types */
#define OSAL_TRUE pdTRUE
#define OSAL_FALSE pdFALSE

#ifdef __cplusplus
}
#endif

#endif /* __FREERTOS_ADAPTER__ */
