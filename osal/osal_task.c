/**
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_task.c
 * @brief OSAL task APIs
 */

#include <string.h>
#include <stdint.h>
#include "vs_conf.h"
#include "osal_task_api.h"
#include "vs_debug.h"

#if CONFIG_FREERTOS
uint8_t os_scheduler_start = 0;

void *osal_create_task(void *func, char *name, uint32_t stack_size,
                       uint32_t task_priority, void *param)
{
    TaskHandle_t xHandle_SysMgr;
    BaseType_t error;
    vs_logging(LOG_LVL_DEBUG, "[%s]:start\n", __func__);
    error = xTaskCreate((TaskFunction_t)func, /* The function that implements
                                                 the task. */
                        name, /* The text name assigned to the task - for debug
                                 only as it is not used by the kernel. */
                        stack_size, /* The size of the stack to allocate to the
                                       task. */
                        param, /* The parameter passed to the task - not used in
                                  this case. */
                        task_priority, /* The priority assigned to the task. */
                        &xHandle_SysMgr); /* The task handle is not required, so
                                             NULL is passed. */
    if (error != pdPASS) {
        vs_logging(LOG_LVL_ERROR, "Error: create task %s failed %ld\n", name,
                   error);
        return NULL;
    }
    return (void *)xHandle_SysMgr;
}
void osal_delete_task(void *task)
{
    vTaskDelete(task);
}
void osal_start_scheduler(void)
{
    vTaskStartScheduler();
}
void osal_end_scheduler(void)
{
    vTaskEndScheduler();
}
void osal_suspend_all(void)
{
    vTaskSuspendAll();
}
void osal_resume_all(void)
{
    xTaskResumeAll();
}
void osal_sleep(int32_t ms)
{
    /*vTaskDelay's sleep unit is a tick(10ms).
      So osal_sleep's ms need convert to ticks.
      We think 1 ms still need sleep 1 tick as task want to sleep.
      So here is the sleep ticks: (ms+9)/10
    */
    if (os_scheduler_start > 0)
        vTaskDelay((ms + 9) / 10);
}

bool osal_started(void)
{
    return (os_scheduler_start == 0) ? false : true;
}
#endif /* CONFIG_FREERTOS */

#if CONFIG_BAREMETAL
void *osal_create_task(void *func, char *name, uint32_t stack_size,
                       uint32_t task_priority, void *param)
{
    return NULL;
}

void osal_delete_task(void *task)
{
    return;
}

void osal_sleep(int32_t ms)
{
    return;
}

void osal_start_scheduler(void)
{
    return;
}

void osal_setup_timer(void)
{
    return;
}

void osal_update_systick(void)
{
    return;
}

bool osal_started(void)
{
    return false;
}
#endif /* CONFIG_BAREMETAL */
