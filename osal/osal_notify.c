/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_notify.c
 * @brief OSAL notify APIs
 */

#include <string.h>
#include <stdint.h>
#include "vs_conf.h"
#include "vs_debug.h"
#include "osal_notify_api.h"

#if CONFIG_FREERTOS
int osal_task_notify(OsalNotify *notify)
{
    return xTaskGenericNotify(notify->task_to_notify, notify->index_to_notify,
                              notify->notify_value, notify->action,
                              notify->pre_ntfy_val);
}

int osal_task_notify_from_isr(OsalNotify *notify)
{
    return xTaskGenericNotifyFromISR(notify->task_to_notify,
                                     notify->index_to_notify,
                                     notify->notify_value, notify->action,
                                     notify->pre_ntfy_val,
                                     notify->higher_task_woken);
}

int osal_task_notify_wait(OsalNotifyWait *notify_wait)
{
    return xTaskGenericNotifyWait(notify_wait->index_to_wait,
                                  notify_wait->bits_to_clr_on_in,
                                  notify_wait->bits_to_clr_on_out,
                                  &notify_wait->notify_value,
                                  notify_wait->ticks_to_wait);
}

#endif /* CONFIG_FREERTOS */
