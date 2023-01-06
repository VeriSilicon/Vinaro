/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_gpio.c
 * @brief HAL for GPIO
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hal_gpio.h"
#include "vsd_error.h"
#include "osal_heap_api.h"
#include "vsd_common.h"

#define MAX_GPIO_GROUPS (4) /**< Limit group number */
#define MAX_GPIO_NUM (22)

static GpioDevice *gpio_grp_dev[MAX_GPIO_GROUPS] = { NULL };
static uint8_t group_num                         = 0;

static inline GpioOperations *get_ops(uint8_t group)
{
    return (GpioOperations *)gpio_grp_dev[group]->ops;
}

int hal_gpio_add_dev(GpioDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;
    for (i = 0; i < sizeof(gpio_grp_dev) / sizeof(gpio_grp_dev[0]); i++) {
        if (gpio_grp_dev[i] == NULL) {
            gpio_grp_dev[i] = device;
            group_num += 1;
            ret = VSD_SUCCESS;

            break;
        }
    }
    return ret;
}

int hal_gpio_remove_dev(GpioDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(gpio_grp_dev) / sizeof(gpio_grp_dev[0]); i++) {
        if (gpio_grp_dev[i] == device) {
            gpio_grp_dev[i] = NULL;
            group_num -= 1;
            ret = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

GpioDevice *hal_gpio_get_device(uint8_t group_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(gpio_grp_dev) / sizeof(gpio_grp_dev[0]); i++) {
        if (gpio_grp_dev[i]->group_id == group_id) {
            return gpio_grp_dev[i];
        }
    }
    return NULL;
}

int hal_gpio_destory(void)
{
    int i;
    for (i = 0; i < group_num; i++) {
        gpio_grp_dev[i] = NULL;
    }
    group_num = 0;
    return VSD_SUCCESS;
}

int hal_gpio_init(const GpioPort *gpio)
{
    bool is_irq     = false;
    bool is_out     = false;
    IoPullCtrl pull = GPIO_PULL_FLOAT;

    if (!gpio || gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (gpio->port > MAX_GPIO_NUM)
        return VSD_ERR_INVALID_PARAM;

    switch (gpio->type) {
    case IO_MODE_IRQ:
        is_out = false;
        is_irq = true;
        pull   = GPIO_PULL_FLOAT;
        break;

    case IO_MODE_IRQ_PU:
        is_out = false;
        is_irq = true;
        pull   = GPIO_PULL_UP;
        break;

    case IO_MODE_IRQ_PD:
        is_out = false;
        is_irq = true;
        pull   = GPIO_PULL_DOWN;
        break;

    case IO_MODE_IN_PU:
        is_out = false;
        pull   = GPIO_PULL_UP;
        break;

    case IO_MODE_IN_PD:
        is_out = false;
        pull   = GPIO_PULL_DOWN;
        break;

    case IO_MODE_IN_FLOAT:
        is_out = false;
        pull   = GPIO_PULL_FLOAT;
        break;

    case IO_MODE_OUT_PULL:
    case IO_MODE_OUT_OPEN_DRAIN:
        is_out = true;
        pull   = GPIO_PULL_FLOAT;
        break;

    default:
        return VSD_ERR_UNSUPPORTED;
    }
    if (get_ops(gpio->group)->io_dir)
        get_ops(gpio->group)
            ->io_dir(gpio_grp_dev[gpio->group], gpio->port, is_out, pull);
    if (get_ops(gpio->group)->io_irq)
        get_ops(gpio->group)
            ->io_irq(gpio_grp_dev[gpio->group], gpio->port, is_irq);

    return VSD_SUCCESS;
}

int hal_gpio_output_high(const GpioPort *gpio)
{
    if (!gpio || gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(gpio->group)->output_set)
        return VSD_ERR_INVALID_PARAM;

    get_ops(gpio->group)
        ->output_set(gpio_grp_dev[gpio->group], gpio->port,
                     gpio->invert ? false : true);
    return VSD_SUCCESS;
}

int hal_gpio_output_low(const GpioPort *gpio)
{
    if (!gpio || gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(gpio->group)->output_set)
        return VSD_ERR_INVALID_PARAM;

    get_ops(gpio->group)
        ->output_set(gpio_grp_dev[gpio->group], gpio->port,
                     gpio->invert ? true : false);
    return VSD_SUCCESS;
}

int hal_gpio_output_toggle(const GpioPort *gpio)
{
    bool is_high;

    if (gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if ((!get_ops(gpio->group)->output_get) ||
        (!get_ops(gpio->group)->output_set))
        return VSD_ERR_INVALID_PARAM;

    is_high =
        get_ops(gpio->group)->output_get(gpio_grp_dev[gpio->group], gpio->port);
    get_ops(gpio->group)
        ->output_set(gpio_grp_dev[gpio->group], gpio->port, !is_high);
    return VSD_SUCCESS;
}

int hal_gpio_input_get(const GpioPort *gpio, uint32_t *value)
{
    if (gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(gpio->group)->input_get)
        return VSD_ERR_INVALID_PARAM;

    bool in_lvl =
        get_ops(gpio->group)->input_get(gpio_grp_dev[gpio->group], gpio->port);
    in_lvl = gpio->invert ? !in_lvl : in_lvl;
    *value = in_lvl ? 1 : 0;
    return VSD_SUCCESS;
}

int hal_gpio_enable_irq(const GpioPort *gpio, GpioIrqHandler handler)
{
    if (!gpio || gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(gpio->group)->irq_enable)
        return VSD_ERR_INVALID_PARAM;

    return get_ops(gpio->group)
        ->irq_enable(gpio_grp_dev[gpio->group], gpio->port, gpio->irq_reload,
                     handler);
}

int hal_gpio_disable_irq(const GpioPort *gpio)
{
    if (!gpio || gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(gpio->group)->irq_disable)
        return VSD_ERR_INVALID_PARAM;

    return get_ops(gpio->group)
        ->irq_disable(gpio_grp_dev[gpio->group], gpio->port);
}

int hal_gpio_clear_irq(const GpioPort *gpio)
{
    if (!gpio || gpio->group >= group_num)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(gpio->group)->irq_clear)
        return VSD_ERR_INVALID_PARAM;

    return get_ops(gpio->group)
        ->irq_clear(gpio_grp_dev[gpio->group], gpio->port);
}

DRV_ISR_SECTION
void hal_gpio_irq_handler(const GpioDevice *device, uint8_t irq_id)
{
    if (!device || device->group_id >= group_num)
        return;
    switch (device->hw_config->irq_mode) {
    case IRQ_GROUP_MODE: {
        if (!get_ops(device->group_id)->irq_grp_handler)
            return;
        return get_ops(device->group_id)
            ->irq_grp_handler(gpio_grp_dev[device->group_id]);
    } break;
    case IRQ_IND_MODE: {
        if (!get_ops(device->group_id)->irq_ind_handler)
            return;
        return get_ops(device->group_id)
            ->irq_ind_handler(gpio_grp_dev[device->group_id], irq_id);
    } break;
    default:
        break;
    }
}
