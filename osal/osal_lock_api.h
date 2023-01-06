/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_lock_api.h
 * @brief OSAL lock APIs
 */

#ifndef __OSAL_LOCK_H__
#define __OSAL_LOCK_H__

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

/** @addtogroup LOCK
 *  OSAL Lock API and definition
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief - OSAL Initial mutex
 *
 * @param mu mutex pointer
 * @return OSAL_TRUE for success, others for failure
 */
int osal_init_mutex(OsalMutex *mu);

/**
 * @brief - OSAL unlock mutex
 *
 * @param mu mutex pointer
 * @return OSAL_TRUE for success, others for failure
 */
int osal_unlock_mutex(OsalMutex *mu);

/**
 * @brief - OSAL lock mutex
 *
 * @param mu mutex pointer
 * @param timeout timeout in millisecond
 * @return OSAL_TRUE for success, others for failure
 */
int osal_lock_mutex(OsalMutex *mu, uint32_t timeout);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_LOCK_H__ */
