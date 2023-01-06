/**
 * Copyright (C) 2022. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_semaphore_api.h
 * @brief OSAL semaphore APIs
 */

#ifndef __OSAL_SEMAPHORE_H__
#define __OSAL_SEMAPHORE_H__

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

/** @addtogroup SEMAPHORE
 *  OSAL Semaphore API and definition
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief Init a semaphore
 *
 * @param sem The pointer/object of semaphore
 * @return OSAL_TRUE for success, others for failure
 */
int osal_init_sem(OsalSemaphore *sem);

/**
 * @brief Wait a semaphore
 *
 * @param sem The pointer/object of semaphore
 * @param timeout timeout in millisecond
 * @return OSAL_TRUE for success, others for failure
 */
int osal_sem_wait(OsalSemaphore *sem, uint32_t timeout);

/**
 * @brief Post a semaphore
 *
 * @param sem The pointer/object of semaphore
 * @return OSAL_TRUE for success, others for failure
 */
int osal_sem_post(OsalSemaphore *sem);

/**
 * @brief Post a semaphore from ISR
 *
 * @param sem The pointer/object of semaphore
 * @return OSAL_TRUE for success, others for failure
 */
int osal_sem_post_isr(OsalSemaphore *sem);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __OSAL_SEMAPHORE_H__
