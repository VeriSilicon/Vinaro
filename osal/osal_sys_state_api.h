/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 *
 * @file osal_sys_state_api.h
 * @brief OSAL system state APIs
 */

#ifndef __OSAL_SYS_STATE_H__
#define __OSAL_SYS_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "vs_conf.h"
#include "osal_adapter.h"

/** @addtogroup STATE
 *  OSAL System State API and definition
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief Dump the status of tasks in the RTOS
 */
void osal_dump_os_state(void);

/**
 * @brief Dump system heap size
 */
void osal_dump_heap_size(void);

/**
 * @brief Get system free heap size
 * @return Return free heap size
 */
uint32_t osal_get_free_heap(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_SYS_STATE_H__ */
