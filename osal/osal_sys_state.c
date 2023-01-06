/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 *
 * @file osal_sys_state.c
 * @brief OSAL system state APIs
 */

#include "osal_sys_state_api.h"
#include "osal_task_api.h"
#include "osal_heap_api.h"
#include "vs_debug.h"

#if CONFIG_FREERTOS
#if ((configUSE_TRACE_FACILITY == 1) && \
     (configUSE_STATS_FORMATTING_FUNCTIONS > 0))
APP_SECTION
static char *write_task_name_to_buffer(char *buffer, const char *task_name)
{
    size_t x;

    strcpy(buffer, task_name);

    for (x = strlen(buffer); x < (size_t)(configMAX_TASK_NAME_LEN - 1); x++) {
        buffer[x] = ' ';
    }

    buffer[x] = (char)0x00;

    return &(buffer[x]);
}
#endif

#if ((configGENERATE_RUN_TIME_STATS == 1) &&       \
     (configUSE_STATS_FORMATTING_FUNCTIONS > 0) && \
     (configSUPPORT_DYNAMIC_ALLOCATION == 1))
APP_SECTION
static void get_os_state_info(char *write_buffer)
{
    TaskStatus_t *task_status_array;
    UBaseType_t array_size, x;
    uint32_t total_time, stats_percentage;
    char exec_status, surv_status;

#if (configUSE_TRACE_FACILITY != 1)
    {
#error configUSE_TRACE_FACILITY must also be set to 1 in FreeRTOSConfig.h
    }
#endif

        *write_buffer = (char)0x00;
    array_size        = uxTaskGetNumberOfTasks();
    task_status_array =
        pvPortMalloc(uxTaskGetNumberOfTasks() * sizeof(TaskStatus_t));

    if (task_status_array != NULL) {
        /* Generate the (binary) data. */
        array_size =
            uxTaskGetSystemState(task_status_array, array_size, &total_time);

        /* For percentage calculations. */
        total_time /= 100UL;

        /* Avoid divide by zero errors. */
        if (total_time > 0UL) {
            /* Create a human readable table from the binary data. */
            for (x = 0; x < array_size; x++) {
                /*Get task execution status*/
                switch (task_status_array[x].eCurrentState) {
                case eRunning:
                    exec_status = TASK_RUNNING_CHAR;
                    break;

                case eReady:
                    exec_status = TASK_READY_CHAR;
                    break;

                case eBlocked:
                    exec_status = TASK_BLOCKED_CHAR;
                    break;

                case eSuspended:
                    exec_status = TASK_SUSPENDED_CHAR;
                    break;

                case eDeleted:
                    exec_status = TASK_DELETED_CHAR;
                    break;

                case eInvalid:
                default:
                    exec_status = (char)0x00;
                    break;
                }

                /*Get task CPU loading(%)*/
                stats_percentage =
                    task_status_array[x].ulRunTimeCounter / total_time;

                /*Get task survival status*/
                surv_status = TASK_ALIVE_CHAR; // TO DO

                write_buffer =
                    write_task_name_to_buffer(write_buffer,
                                              task_status_array[x].pcTaskName);

                /* Write the rest of the string. */
                if (stats_percentage > 0UL) {
                    sprintf(write_buffer,
                            "\t%c\t%u\t%-4u\t%u\t%-10lu\t%u\t%c\r\n",
                            exec_status,
                            (unsigned int)task_status_array[x]
                                .uxCurrentPriority,
                            (unsigned int)task_status_array[x]
                                .usStackHighWaterMark,
                            (unsigned int)task_status_array[x].xTaskNumber,
                            (unsigned long)task_status_array[x]
                                .ulRunTimeCounter,
                            (unsigned int)stats_percentage, surv_status);
                } else {
                    sprintf(write_buffer,
                            "\t%c\t%u\t%-4u\t%u\t%-10lu\t<1\t%c\r\n",
                            exec_status,
                            (unsigned int)task_status_array[x]
                                .uxCurrentPriority,
                            (unsigned int)task_status_array[x]
                                .usStackHighWaterMark,
                            (unsigned int)task_status_array[x].xTaskNumber,
                            (unsigned long)task_status_array[x]
                                .ulRunTimeCounter,
                            surv_status);
                }

                write_buffer += strlen(write_buffer);
            }
        } else {
            mtCOVERAGE_TEST_MARKER();
        }

        vPortFree(task_status_array);
    } else {
        mtCOVERAGE_TEST_MARKER();
    }
}
#endif
#endif /* CONFIG_FREERTOS */

APP_SECTION
void osal_dump_os_state(void)
{
#if CONFIG_FREERTOS
    char *InfoBuffer = NULL;
    InfoBuffer =
        (char *)osal_malloc(uxTaskGetNumberOfTasks() * EACH_TASK_INFO_SIZE);

    vs_logging(LOG_LVL_CRITICAL, "\r\nTask_name     Status Priority  Stack  "
                                 "Init_Seq  Occupy(ms)    CPU(%%)  Alive");
    vs_logging(LOG_LVL_CRITICAL, "---------------------------------------------"
                                 "------------------------------");
    get_os_state_info(InfoBuffer);
    vs_logging(LOG_LVL_CRITICAL, "%s", InfoBuffer);

    osal_free(InfoBuffer);
#else
    return;
#endif /* CONFIG_FREERTOS */
}

APP_SECTION
void osal_dump_heap_size(void)
{
#if CONFIG_FREERTOS
    vs_logging(LOG_LVL_CRITICAL, "\r\nRTOS remains heap: %" PRIu32 "",
               (uint32_t)xPortGetFreeHeapSize());
#else
    return;
#endif /* CONFIG_FREERTOS */
}

APP_SECTION
uint32_t osal_get_free_heap(void)
{
#if CONFIG_FREERTOS
    return xPortGetFreeHeapSize();
#else
    return 0;
#endif /* CONFIG_FREERTOS */
}
