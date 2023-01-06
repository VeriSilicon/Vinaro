/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_task_api.h
 * @brief OSAL task APIs
 */

#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "vs_conf.h"
#include "osal_adapter.h"
/** @addtogroup TASK
 *  - OSAL Task API
 *  @ingroup OSAL
 *  @{
 */

enum TASK_STATUS {
    TASK_RUNNING,
    TASK_PAUSE,
    TASK_EXIT,
};

/**
 * @brief - OSAL create task
 *
 * @param func task function
 * @param name task name
 * @param stack_size task stack size
 * @param task_priority task priority
 * @param param param for task function
 * @return task handle on success, NULL on fail
 */
void *osal_create_task(void *func, char *name, uint32_t stack_size,
                       uint32_t task_priority, void *param);

/**
 * @brief - OSAL delete task
 *
 * @param task task handle
 */
void osal_delete_task(void *task);

/**
 * @brief - OSAL suspend all tasks
 */
void osal_suspend_all(void);

/**
 * @brief - OSAL resume all tasks
 */
void osal_resume_all(void);

/**
 * @brief - OSAL sleep task
 *
 * @param ms sleep time, miniseconds
 */
void osal_sleep(int32_t ms);

/**
 * @brief - OSAL start scheduler
 */
void osal_start_scheduler(void);

/**
 * @brief - OSAL stop scheduler
 */
void osal_end_scheduler(void);

/**
 * @brief - OSAL get OS scheduler state
 *
 * @return the status of OS scheduler, true for started
 */
bool osal_started(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_TASK_H__ */
