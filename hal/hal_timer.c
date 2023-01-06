/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_timer.c
 * @brief HAL for timer driver
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vsd_common.h"
#include "vsd_error.h"
#include "hal_timer.h"

/**
 * @brief Define the state of each timer port
 */
typedef struct TimerPortState {
    uint8_t hw_id; /**< The ID of timer device */
    uint8_t type; /**< Timer type, @see TimerType */
    uint8_t cnt_id; /**< The counter ID in this timer device */
    uint8_t is_using; /**< 0 for available, 1 for using */
} TimerPortState;

static TimerDevice *g_timer_dev[TIMER_DEVICE_MAX] = { NULL };
static TimerPortState *gp_port_state              = NULL;
static uint8_t g_total_ports                      = 0;

static inline TimerOperations *get_ops(uint8_t hw_id)
{
    return (TimerOperations *)g_timer_dev[hw_id]->ops;
}

static inline int validate_state(uint8_t port, uint8_t *hw_id)
{
    if (port >= g_total_ports)
        return VSD_ERR_INVALID_PARAM;

    *hw_id = gp_port_state[port].hw_id;
    if (g_timer_dev[*hw_id] == NULL)
        return VSD_ERR_INVALID_PARAM;

    return VSD_SUCCESS;
}

static int get_available_timer(uint8_t type, uint8_t *hw_id, uint8_t *cnt_id)
{
    uint8_t i, port;
    uint8_t idx;

    if (!gp_port_state || (g_total_ports == 0))
        return VSD_ERR_INVALID_STATE;

    uint8_t port_sta[g_total_ports];

    // scan all timer devices
    for (i = 0; i < TIMER_DEVICE_MAX; i++) {
        if (!g_timer_dev[i] || (g_timer_dev[i]->type != type)) {
            continue;
        }
        // assume all counters are available
        memset(port_sta, 0, sizeof(port_sta));
        *hw_id = i;
        // update timer device hw_id is i port occupied state
        for (port = 0; port < g_total_ports; port++) {
            // check right timer device occupied port
            if (gp_port_state[port].type == TIMER_TYPE_NONE ||
                gp_port_state[port].hw_id != i)
                continue;
            // update actual port using state
            port_sta[gp_port_state[port].cnt_id] = gp_port_state[port].is_using;
        }
        // find the 1st available counter
        for (idx = 0; idx < g_timer_dev[i]->hw_config->counters; idx++) {
            if (port_sta[idx] == 0) {
                *cnt_id = idx;
                return VSD_SUCCESS;
            }
        }
    }
    return VSD_ERR_EMPTY;
}

int hal_timer_add_dev(TimerDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;
    for (i = 0; i < sizeof(g_timer_dev) / sizeof(g_timer_dev[0]); i++) {
        if (g_timer_dev[i] == NULL) {
            g_timer_dev[i] = device;
            ret            = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_timer_remove_dev(TimerDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_timer_dev) / sizeof(g_timer_dev[0]); i++) {
        if (g_timer_dev[i] == device) {
            g_timer_dev[i] = NULL;
            ret            = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

uint8_t hal_timer_get_ports(void)
{
    uint8_t total = 0;
    uint8_t i;

    for (i = 0; i < TIMER_DEVICE_MAX; i++) {
        if (g_timer_dev[i] && g_timer_dev[i]->hw_config) {
            total += g_timer_dev[i]->hw_config->counters;
        }
    }
    return total;
}

TimerDevice *hal_timer_get_device(uint8_t hw_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_timer_dev) / sizeof(g_timer_dev[0]); i++) {
        if (g_timer_dev[i]->hw_id == hw_id) {
            return g_timer_dev[i];
        }
    }
    return NULL;
}

int hal_timer_init(TimerType type, uint8_t *port)
{
    uint8_t hw_id  = 0;
    uint8_t cnt_id = 0;
    uint8_t i;
    int ret;

    if (!port)
        return VSD_ERR_INVALID_POINTER;

    g_total_ports = hal_timer_get_ports();
    if (g_total_ports == 0)
        return VSD_ERR_EMPTY;

    if (!gp_port_state) {
        // first use, malloc buffer to maintain port status
        gp_port_state = malloc(sizeof(TimerPortState) * g_total_ports);
        if (!gp_port_state) {
            return VSD_ERR_NO_MEMORY;
        }
        memset(gp_port_state, 0, sizeof(TimerPortState) * g_total_ports);
    }
    ret = get_available_timer(type, &hw_id, &cnt_id);
    if (ret != VSD_SUCCESS) {
        // all timer counters are in using, no available now
        return ret;
    }
    // find an available port, save the hw_id and cnt_id to port state
    for (i = 0; i < g_total_ports; i++) {
        if (gp_port_state[i].is_using == 0) {
            // occupy the port
            gp_port_state[i].hw_id    = hw_id;
            gp_port_state[i].cnt_id   = cnt_id;
            gp_port_state[i].type     = type;
            gp_port_state[i].is_using = 1;
            *port                     = i;
            break;
        }
    }
    // now we got available timer device and counter id
    if (!get_ops(hw_id)->init)
        return VSD_ERR_GENERIC;
    return get_ops(hw_id)->init(g_timer_dev[hw_id], cnt_id);
}

int hal_timer_free(uint8_t port)
{
    if (!gp_port_state)
        return VSD_ERR_INVALID_STATE;
    if (port >= g_total_ports)
        return VSD_ERR_INVALID_PARAM;

    memset(&gp_port_state[port], 0, sizeof(TimerPortState));
    return VSD_SUCCESS;
}

/**
 * @brief timer HAL enable function, used to enable timer interrupt
 * @param port the timer port
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_enable_interrupt(uint8_t port)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(hw_id)->enable)
        return VSD_ERR_GENERIC;
    return get_ops(hw_id)->enable(g_timer_dev[hw_id],
                                  gp_port_state[port].cnt_id);
}

/**
 * @brief timer HAL disable function, used to disable timer interrupt
 * @param port the timer port
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_disable_interrupt(uint8_t port)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(hw_id)->disable)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->disable(g_timer_dev[hw_id],
                                   gp_port_state[port].cnt_id);
}

/**
 * @brief timer HAL start function, used to start timer
 * @param port the timer port
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_start(uint8_t port)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;

    if (!get_ops(hw_id)->start)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->start(g_timer_dev[hw_id],
                                 gp_port_state[port].cnt_id);
}

/**
 * @brief timer HAL stop function, used to stop timer
 * @param port the timer port
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_stop(uint8_t port)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(hw_id)->stop)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->stop(g_timer_dev[hw_id], gp_port_state[port].cnt_id);
}

/**
 * @brief timer HAL reset function, used to reset timer
 * @param port the timer port
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_reset(uint8_t port)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(hw_id)->reset)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->reset(g_timer_dev[hw_id],
                                 gp_port_state[port].cnt_id);
}

/**
 * @brief timer HAL set period function, used to set timer's period mode
 * @param port the timer port
 * @param count Time until count should expire
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_set_period(uint8_t port, uint32_t count)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(hw_id)->periodset)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->periodset(g_timer_dev[hw_id],
                                     gp_port_state[port].cnt_id, count);
}

/**
 * @brief timer HAL set single function, used to set timer's single mode
 * @param port the timer port
 * @param count Time until count should expire
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_set_single(uint8_t port, uint32_t count)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(hw_id)->singleset)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->singleset(g_timer_dev[hw_id],
                                     gp_port_state[port].cnt_id, count);
}

/**
 * @brief timer HAL get current count function, used to get timer current count
 * @param port the timer port
 * @param curcount the timer current count
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_get_count(uint8_t port, uint64_t *curcount)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if ((!get_ops(hw_id)->getcurcount) || (!curcount))
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->getcurcount(g_timer_dev[hw_id],
                                       gp_port_state[port].cnt_id, curcount);
}

/**
 * @brief timer HAL time2count function, used to get timer current count
 * @param port the timer port
 * @param timeout_ms the timer current count
 * @param time2count the timeout_ms to count
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_time2count(uint8_t port, uint64_t *timeout_ms,
                         uint64_t *time2count)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;
    if ((!get_ops(hw_id)->time2count) || (!timeout_ms) || (!time2count))
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->time2count(g_timer_dev[hw_id],
                                      gp_port_state[port].cnt_id, timeout_ms,
                                      time2count);
}

/**
 * @brief timer HAL callbackset function, used to set timer callback function
 * @param port the timer port
 * @param hook callback hook function to register
 * @param context  Pointer to send to callback function
 * @param callback_memory Pointer to memory where callback structure can be
 * allocated
 * @return Return result, VSD_SUCCESS for success, VSD_ERR_GENERIC for error
 */
int hal_timer_callbackset(uint8_t port,
                          void (*hook)(TimerCallbackArgs *arg, void *context),
                          void *context, TimerCallbackArgs *callback_memory)
{
    uint8_t hw_id = 0;
    if (VSD_SUCCESS != validate_state(port, &hw_id))
        return VSD_ERR_INVALID_PARAM;

    if (!get_ops(hw_id)->callbackset)
        return VSD_ERR_GENERIC;

    return get_ops(hw_id)->callbackset(g_timer_dev[hw_id],
                                       gp_port_state[port].cnt_id, hook,
                                       context, callback_memory);
}

/**
 * @brief
 *
 * @param device timer device
 * @param cnt_id timer interrupt counter id
 * @return int
 */
DRV_ISR_SECTION
void hal_timer_irq_handler(TimerDevice *device, uint8_t cnt_id)
{
    if (!device)
        return;
    if (!get_ops(device->hw_id)->irq_hanlder)
        return;
    return get_ops(device->hw_id)->irq_hanlder(device, cnt_id);
}
