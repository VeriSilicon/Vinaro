/**
 * Copyright (C) 2022. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_adapter.h
 * @brief Compatible with bare metal and different real-time operating systems,
 * such as FreeRTOS, RT-Thread, etc.
 */

#ifndef __OSAL_ADAPTER__
#define __OSAL_ADAPTER__

#ifdef __cplusplus
extern "C" {
#endif

#include "vs_conf.h"

#if CONFIG_FREERTOS
#include "freertos_adapter.h"
#endif
#if CONFIG_BAREMETAL
#include "baremental_adapter.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_ADAPTER__ */
