/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_heap.c
 * @brief OSAL heap APIs
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "osal_heap_api.h"
#include "vs_conf.h"

#if CONFIG_FREERTOS
extern uint8_t os_scheduler_start;

void *osal_malloc(size_t len)
{
    if (!os_scheduler_start)
        return NULL;
    return pvPortMalloc(len);
}

void osal_free(void *pmem)
{
    if (!os_scheduler_start)
        return;
    return vPortFree(pmem);
}

void *osal_malloc_noncache(size_t len)
{
#if CONFIG_SOC_CACHE_SUPPORT
    if (!os_scheduler_start)
        return NULL;
    return pvPortMallocNonCache(len);
#else
    /** Reuse the osal_malloc if host CPU does not support caching */
    return osal_malloc(len);
#endif
}

void osal_free_noncache(void *pmem)
{
#if CONFIG_SOC_CACHE_SUPPORT
    if (!os_scheduler_start)
        return;
    return vPortFreeNonCache(pmem);
#else
    /** Reuse the osal_free if host CPU does not support caching */
    return osal_free(pmem);
#endif
}

#endif /* CONFIG_FREERTOS */

#if CONFIG_BAREMETAL
void *osal_malloc(size_t len)
{
    return malloc(len);
}

void osal_free(void *pmem)
{
    return free(pmem);
}

void *osal_malloc_noncache(size_t len)
{
    return osal_malloc(len);
}

void osal_free_noncache(void *pmem)
{
    return osal_free(pmem);
}
#endif /* CONFIG_BAREMETAL */
