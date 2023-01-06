/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_heap_api.h
 * @brief OSAL heap APIs
 */

#ifndef __OSAL_HEAP_H__
#define __OSAL_HEAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <string.h>
#include "osal_adapter.h"
#include "vs_conf.h"

/** @addtogroup HEAP
 *  OSAL Heap API
 *  @ingroup OSAL
 *  @{
 */

#if CONFIG_APP_SOC_QEMU
extern void osal_memset(char *buf, char byte, int len);
#else
#define osal_memset memset
#endif

/**
 * @brief Allocate memory
 * @note Allocate cacheable memory for CPU which supports caching; Allocate
 * non-cacheable memory for CPU which does not support caching.
 *
 * @param len Memory size
 * @return Pointer of memory block, NULL for failure
 */
void *osal_malloc(size_t len);

/**
 * @brief Free the memory
 * @note Free cacheable memory for CPU which supports caching; Free
 * non-cacheable memory for CPU which does not support caching.
 *
 * @param pmem Pointer of memory block
 */
void osal_free(void *pmem);

/**
 * @brief Allocate non-cacheable memory
 * @param len Memory size
 * @return void* Pointer of memory block, NULL for failure
 */
void *osal_malloc_noncache(size_t len);

/**
 * @brief Free the non-cacheable memory
 * @param pmem Pointer of memory block
 */
void osal_free_noncache(void *pmem);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_HEAP_H__ */
