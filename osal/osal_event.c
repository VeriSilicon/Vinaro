/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_event.c
 * @brief OSAL event(message queue) APIs
 */

#include <string.h>
#include <stdint.h>
#include "vs_conf.h"
#include "osal_event_api.h"
#include "osal_heap_api.h"
#include "vs_debug.h"
#include "osal_lock_api.h"
#include "osal_task_api.h"
#include "list_pool.h"
#include "vpi_event.h"

#if CONFIG_EVENT_SYS_ENABLE
/*#define EVENT_POOL_SIZE CONFIG_EVENT_POOL_SIZE*/
static ListPool *vs_event_pool;
uint32_t osal_get_event_pool_length(void)
{
    if (vs_event_pool)
        return vs_event_pool->length;
    return 0;
}
#endif

#if CONFIG_DEBUG_EVENT_POOL_EMPTY && !CONFIG_BAREMETAL
/*Here is debug feature of event system.
The purpose is dump how many events are pending on each event queue.
Then we could know which tasks eat the event pool.*/
typedef struct OsalQueueItem {
    struct list_head entry;
    void *queue;
    char *owner_name;
} OsalQueueItem;
static struct list_head osal_event_queue_list;
static int osal_event_queue_debug_start;

static void osal_event_queue_init(void)
{
    if (!osal_event_queue_debug_start) {
        init_list(&osal_event_queue_list);
        osal_event_queue_debug_start = 1;
        vs_logging(LOG_LVL_DEBUG, "%s Initialize queue debug\r\n", __func__);
    }
}

/*Call it when event pool is empty!*/
static void osal_event_queue_dump(uint32_t event_id)
{
    OsalQueueItem *queue_item = NULL;
#if CONFIG_FREERTOS
    QueueHandle_t os_queue = NULL;
#endif
    list_for_each_entry(queue_item, &osal_event_queue_list, entry) {
#if CONFIG_FREERTOS
        os_queue = queue_item->queue;
        vs_logging(LOG_LVL_ERROR,
                   "os_queue %p has %d events, belongs to task %s\r\n",
                   os_queue, (int)uxQueueMessagesWaiting(os_queue),
                   queue_item->owner_name);

#endif
    }
}

static void osal_event_queue_add(void *queue)
{
#if CONFIG_FREERTOS
    TaskHandle_t current = NULL;
#endif
    OsalQueueItem *queue_item =
        (OsalQueueItem *)osal_malloc(sizeof(OsalQueueItem));
    if (queue_item == NULL) {
        vs_logging(LOG_LVL_DEBUG, "Error:malloc queue_item fail\r\n");
        return;
    }
    init_list(&queue_item->entry);
    queue_item->queue = queue;
#if CONFIG_FREERTOS
    current                = xTaskGetCurrentTaskHandle();
    queue_item->owner_name = pcTaskGetName(current);
#endif
    list_add(&osal_event_queue_list, &queue_item->entry);
    vs_logging(LOG_LVL_DEBUG, "%s new queue %p\r\n", __func__,
               queue_item->queue);
}

static void osal_event_get_task_name(void *queue, void *event)
{
    OsalEventEntry *osal_ev = NULL;
#if CONFIG_FREERTOS
    TaskHandle_t current = NULL;
    current              = xTaskGetCurrentTaskHandle();
#endif
    osal_ev = (OsalEventEntry *)((uint32_t)event - sizeof(struct list_head));
#if CONFIG_FREERTOS
    osal_ev->owner_name = pcTaskGetName(current);
#endif
    vs_logging(LOG_LVL_DEBUG, "%s current is %s\r\n", __func__,
               osal_ev->owner_name);
}
#endif /* CONFIG_DEBUG_EVENT_POOL_EMPTY && !CONFIG_BAREMETAL */

#if CONFIG_FREERTOS
#if CONFIG_EVENT_SYS_ENABLE
void *osal_create_event_queue(int length, uint32_t item_size)
{
    if (vs_event_pool == NULL) {
        vs_event_pool = vpi_pool_init(length, item_size);
        if (vs_event_pool == NULL) {
            return NULL;
        }
    }
    /*queue length must < pool size.
    Or queue will eat all pool buffer and won't wakeup consumer.
    1 means consumer need wakeup immediately.*/
    QueueHandle_t queue = xQueueCreate(length - 2, sizeof(OsalEvent *));
#if CONFIG_DEBUG_EVENT_POOL_EMPTY
    osal_event_queue_init();
    osal_event_queue_add(queue);
#endif
    return (void *)queue;
}

int osal_wait_event(void *queue, void **data, uint32_t timeout)
{
    uint32_t recv_event_id = EVENT_INVALID;
    OsalEvent *event       = NULL;
    BaseType_t ret         = EVENT_ERROR;
    while (1) {
        ret = xQueueReceive((QueueHandle_t)queue, &event,
                            (timeout == 0) ? 0xFFFFFFFF : timeout);
        if (ret == pdFALSE) {
            vs_logging(LOG_LVL_ERROR, "%s: XQueueReceive Fail\r\n", __func__);
            continue;
        }
        vs_logging(LOG_LVL_DEBUG, "%s:queue at %p,event is %d,data is %p\n",
                   __func__, queue, event->event_id, event->data);
        recv_event_id   = event->event_id;
        event->event_id = EVENT_INVALID;
        if (event->data) {
            *data       = event->data;
            event->data = NULL;
        }

        vpi_pool_put_buf_back(vs_event_pool, (uint8_t *)event);
        break;
    }
    return recv_event_id;
}

int osal_send_event(void *queue, uint32_t event_id, void *data,
                    uint32_t timeout)
{
    OsalEvent *event = NULL;
    BaseType_t ret   = EVENT_OK;
    vs_logging(LOG_LVL_DEBUG, "FreeRTOS %s\n", __func__);

    if (queue == NULL) {
        vs_logging(LOG_LVL_ERROR, "ERROR: %s, no tx queue\n", __func__);
        return EVENT_ERROR;
    }
    event = (OsalEvent *)vpi_pool_get_buf(vs_event_pool);
    if (event == NULL) {
        if (timeout) {
            /*delay a while to try to get event again*/
        }
        vs_logging(LOG_LVL_ERROR, "ERROR: event buffer is empty\r\n");
#if CONFIG_DEBUG_EVENT_POOL_EMPTY
        osal_event_queue_dump(event_id);
#endif
        return EVENT_ERROR;
    }
#if CONFIG_DEBUG_EVENT_POOL_EMPTY
    osal_event_get_task_name(queue, event);
#endif
    event->event_id = event_id;
    event->data     = data;
    vs_logging(LOG_LVL_DEBUG, "%s:queue at %p,event is %d,data is %p\n",
               __func__, queue, event->event_id, event->data);
    ret = xQueueSend((QueueHandle_t)queue, &event, 10);
    if (ret != pdPASS) {
        vs_logging(LOG_LVL_ERROR, "Queue(%p) is FULL", queue);
        return EVENT_ERROR;
    }
    return EVENT_OK;
}
#endif /* CONFIG_EVENT_SYS_ENABLE */

void *osal_create_queue_raw(void)
{
    QueueHandle_t queue = xQueueCreate(4, sizeof(void *));
    return (void *)queue;
}

int osal_send_event_raw(void *queue, void *data)
{
    uint32_t raw_data = (uint32_t)data;
    if (xQueueSend((QueueHandle_t)queue, (void *)&raw_data, 10) != pdTRUE) {
        vs_logging(LOG_LVL_ERROR, "Raw Event Error:raw event send %p error\r\n",
                   (void *)raw_data);
        return EVENT_ERROR;
    }

    return EVENT_OK;
}

int osal_send_event_raw_from_isr(void *queue, void *data)
{
    uint32_t raw_data = (uint32_t)data;
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    if (xQueueSendFromISR((QueueHandle_t)queue, (void *)&raw_data,
                          &xHigherPriorityTaskWoken) != pdTRUE) {
        vs_logging(LOG_LVL_ERROR, "Raw Event Error:raw event send %p error\r\n",
                   (void *)raw_data);
        return EVENT_ERROR;
    }

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR(pdTRUE);
    }

    return EVENT_OK;
}

void osal_wait_event_raw(void *queue, void **data)
{
    xQueueReceive((QueueHandle_t)queue, data, 0xFFFFFFFF);
}
#endif /* CONFIG_FREERTOS */

#if CONFIG_BAREMETAL
void *osal_create_queue_raw(void)
{
    return NULL;
}

void osal_wait_event_raw(void *queue, void **data)
{
    return;
}

int osal_send_event_raw_from_isr(void *queue, void *data)
{
    return EVENT_OK;
}
#endif /* CONFIG_BAREMETAL */
