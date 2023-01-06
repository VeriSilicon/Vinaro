/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_lock.c
 * @brief OSAL lock APIs
 */

#include <string.h>
#include <stdint.h>
#include "vs_conf.h"
#include "vs_debug.h"
#include "osal_lock_api.h"

#if CONFIG_FREERTOS
int osal_init_mutex(OsalMutex *mu)
{
    mu->mutex = NULL;
    mu->mutex = xSemaphoreCreateMutex();
    if (mu->mutex == NULL)
        return OSAL_FALSE;
    return OSAL_TRUE;
}

/* Release a mutex */
int osal_unlock_mutex(OsalMutex *mu)
{
    return xSemaphoreGive(mu->mutex);
}

/* Pend (wait) for a mutex
A block time of zero can be used to poll the semaphore.
A block time of portMAX_DELAY can be used to block indefinitely.
*/
int osal_lock_mutex(OsalMutex *mu, uint32_t timeout)
{
    // convert millisecond to ticks
    uint32_t ticks = (timeout == portMAX_DELAY) ?
                         portMAX_DELAY :
                         (timeout / portTICK_PERIOD_MS);
    return xSemaphoreTake(mu->mutex, ticks);
}
#endif /* CONFIG_FREERTOS */

#if CONFIG_BAREMETAL
int osal_init_mutex(OsalMutex *mu)
{
    return OSAL_TRUE;
}

int osal_unlock_mutex(OsalMutex *mu)
{
    return OSAL_TRUE;
}

int osal_lock_mutex(OsalMutex *mu, uint32_t timeout)
{
    return OSAL_TRUE;
}
#endif /* CONFIG_BAREMETAL */
