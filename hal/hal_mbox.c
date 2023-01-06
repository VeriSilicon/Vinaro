/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_mbox.c
 * @brief HAL interface for mailbox
 */

#include "hal_mbox.h"

static MboxDevice *g_mbox_dev[MBOX_DEVICE_MAX] = { NULL };

static inline MboxOperations *get_ops(const MboxDevice *device)
{
    return (MboxOperations *)device->ops;
}

int hal_mbox_add_dev(MboxDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;
    for (i = 0; i < sizeof(g_mbox_dev) / sizeof(g_mbox_dev[0]); i++) {
        if (g_mbox_dev[i] == NULL) {
            g_mbox_dev[i] = device;
            ret           = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_mbox_remove_dev(MboxDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_mbox_dev) / sizeof(g_mbox_dev[0]); i++) {
        if (g_mbox_dev[i] == device) {
            g_mbox_dev[i] = NULL;
            ret           = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

MboxDevice *hal_mbox_get_device(uint8_t hw_id)
{
    uint8_t i;
    for (i = 0; i < sizeof(g_mbox_dev) / sizeof(g_mbox_dev[0]); i++) {
        if (g_mbox_dev[i]->hw_id == hw_id) {
            return g_mbox_dev[i];
        }
    }
    return NULL;
}

int hal_mbox_init(const MboxDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->init)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->init(device);
}

int hal_mbox_enable(const MboxDevice *device, bool enable,
                    MboxHandler req_cb_handler)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->enable)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->enable(device, enable, req_cb_handler);
}

int hal_mbox_request(const MboxDevice *device, MboxHandler ack_cb_handler)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->request)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->request(device, ack_cb_handler);
}

DRV_ISR_SECTION
void hal_mbox_req_irq_handler(const MboxDevice *device)
{
    if (!device || !get_ops(device)->req_irq_handler)
        return;

    return get_ops(device)->req_irq_handler(device);
}

DRV_ISR_SECTION
void hal_mbox_ack_irq_handler(const MboxDevice *device)
{
    if (!device || !get_ops(device)->ack_irq_handler)
        return;

    return get_ops(device)->ack_irq_handler(device);
}
