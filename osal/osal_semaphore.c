/**
 * Copyright (C) 2022. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_semaphore.c
 * @brief OSAL semaphore APIs
 */

#include <string.h>
#include <stdint.h>
#include "vs_conf.h"
#include "osal_semaphore_api.h"

#if (CONFIG_BAREMETAL)
int osal_init_sem(OsalSemaphore *sem)
{
    return OSAL_TRUE;
}

int osal_sem_wait(OsalSemaphore *sem, uint32_t timeout)
{
    return OSAL_TRUE;
}

/* Post semaphore */
int osal_sem_post(OsalSemaphore *sem)
{
    return OSAL_TRUE;
}

int osal_sem_post_isr(OsalSemaphore *sem)
{
    return OSAL_TRUE;
}
#endif

#if CONFIG_FREERTOS
int osal_init_sem(OsalSemaphore *sem)
{
    sem->semaphore = NULL;
    sem->semaphore = xSemaphoreCreateBinary();
    if (sem->semaphore == NULL)
        return OSAL_FALSE;
    return OSAL_TRUE;
}

int osal_sem_wait(OsalSemaphore *sem, uint32_t timeout)
{
    // convert millisecond to ticks
    uint32_t ticks = (timeout == portMAX_DELAY) ?
                         portMAX_DELAY :
                         (timeout / portTICK_PERIOD_MS);
    return xSemaphoreTake(sem->semaphore, ticks);
}

/* Post semaphore */
int osal_sem_post(OsalSemaphore *sem)
{
    return xSemaphoreGive(sem->semaphore);
}

int osal_sem_post_isr(OsalSemaphore *sem)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    /* The event has occurred, use the semaphore to unblock the task so the task
    can process the event. */
    int ret = xSemaphoreGiveFromISR(sem->semaphore, &xHigherPriorityTaskWoken);
    /* Clear the interrupt here. */
    /* Now the task has been unblocked a context switch should be performed if
    xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax required to
    perform a context switch from an ISR varies from port to port, and from
    compiler to compiler. Check the web documentation and examples for the port
    being used to find the syntax required for your application. */
    if (xHigherPriorityTaskWoken)
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    return ret;
}
#endif /* CONFIG_FREERTOS */
