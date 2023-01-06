/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_event_api.h
 * @brief OSAL event(message queue) APIs
 */

#ifndef __OSAL_EVENT_H__
#define __OSAL_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <inttypes.h>
#include "vs_conf.h"
#include "vpi_list.h"
#include "osal_lock_api.h"
#include "osal_adapter.h"

/** @addtogroup EVENT
 *  OSAL Event API and definition
 *  @ingroup OSAL
 *  @{
 */

typedef struct OsalEvent {
    uint32_t event_id;
    void *data;
} OsalEvent;

/**
 * @brief - event item of event system
 */
typedef struct OsalEventEntry {
    struct list_head event_entry; /*list hook*/
    OsalEvent event_data; /*event on OS*/
#if CONFIG_DEBUG_EVENT_POOL_EMPTY
    char *owner_name;
#endif
} OsalEventEntry;

/**
 * @brief - For event system, create a new event queue and returns a event queue
 * handle, which the event queue can be referenced
 *
 * @param length event queue length
 * @param item_size every event queue item size
 * @return Queue handle or NULL
 */
void *osal_create_event_queue(int length, uint32_t item_size);

/**
 * @brief - Send event id and data to event queue of event system
 *
 * @param queue event queue handle
 * @param event_id event ID
 * @param data event data that needs to be sent
 * @param timeout timeout time
 * @return EVENT_OK on success, EVENT_ERROR on fail
 */
int osal_send_event(void *queue, uint32_t event_id, void *data,
                    uint32_t timeout);

/**
 * @brief - Waiting for event data from event queue of event system
 *
 * @param queue event queue handle
 * @param data event data that needs to be received
 * @param timeout timeout time
 * @return event ID on success, EVENT_INVALID on fail.
 */
int osal_wait_event(void *queue, void **data, uint32_t timeout);

/**
 * @brief - Creates a new OS raw queue and returns a handle by which the queue
 * can be referenced
 * @note The default size of queue is 4 for FreeRTOS case
 * @todo API should be updated later to be customizable
 *
 * @return Newly created queue handle or NULL for error
 */
void *osal_create_queue_raw(void);

/**
 * @brief - Send 32bits data to the OS raw queue
 *
 * @param queue OS raw queue handle
 * @param data data that needs to be sent
 * @return EVENT_OK on success, EVENT_ERROR on fail
 */
int osal_send_event_raw(void *queue, void *data);

/**
 * @brief - Send 32bits data to the OS raw queue from ISR
 * @note This API is another version of the osal_send_event_raw() API which can
 * be called from an ISR
 *
 * @param queue OS raw queue handle
 * @param data data that needs to be sent
 * @return EVENT_OK on success, EVENT_ERROR on fail
 */
int osal_send_event_raw_from_isr(void *queue, void *data);

/**
 * @brief - Waiting and receive data from OS raw queue
 *
 * @param queue OS raw queue handle
 * @param data data that needs to be received
 */
void osal_wait_event_raw(void *queue, void **data);

/**
 * @brief - Get event system's event pool free buffer length
 *
 * @return free buffer length
 */
uint32_t osal_get_event_pool_length(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_EVENT_H__ */
