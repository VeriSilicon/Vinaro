/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_dmac.c
 * @brief HAL for DMAC
 */

#include <stdlib.h>
#include <string.h>
#include "hal_dmac.h"
#include "vsd_error.h"
#include "osal_task_api.h"

static DmacHw *dmac_config = NULL;

static inline DmacOperation *get_ops(void)
{
    return (DmacOperation *)dmac_config->ops;
}

static inline DmacHwConfig *get_hw_cfg(void)
{
    return (DmacHwConfig *)dmac_config->hw_config;
}

static inline DmacChannelCfg *get_channel_cfg(int8_t channel_num)
{
    return (DmacChannelCfg *)dmac_config->channel_config[channel_num];
}

// get dma hw_config from soc init and init mutex
int hal_dmac_create(const DmacHw *hw_config)
{
    if (!hw_config)
        return VSD_ERR_INVALID_POINTER;
    dmac_config = (DmacHw *)hw_config;

    return VSD_SUCCESS;
}

// select channel num available and set the busy_flag to true
static int8_t alloc_dmac_channel(DmacDevType src, DmacDevType dst,
                                 int8_t *channel_num)
{
    uint8_t i                     = 0;
    int ret                       = VSD_ERR_BUSY;
    int channel_sum               = get_channel_sum();
    DmacChannelList *channel_list = get_channel_list();
    if (!channel_list)
        return VSD_ERR_GENERIC;

    for (i = 0; i < channel_sum; i++) {
        if ((channel_list[i].type.src == src) &&
            (channel_list[i].type.dst == dst) &&
            (channel_list[i].busy_flag == false)) {
            channel_list[i].busy_flag = true;
            *channel_num              = channel_list[i].channel_num;
            ret                       = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

// free channel num to the list, set the busy_flag to false
static int8_t free_dmac_channel(int8_t channel_num)
{
    uint8_t i                     = 0;
    int ret                       = VSD_ERR_INVALID_STATE;
    int channel_sum               = get_channel_sum();
    DmacChannelList *channel_list = get_channel_list();
    if (!channel_list)
        return VSD_ERR_GENERIC;

    for (i = 0; i < channel_sum; i++) {
        if (channel_list[i].channel_num == channel_num) {
            channel_list[i].busy_flag = false;
            ret                       = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_dmac_channel_select(DmacDevType src, DmacDevType dst,
                            int8_t *channel_num)
{
    return alloc_dmac_channel(src, dst, channel_num);
}

int hal_dmac_channel_free(int8_t channel_num)
{
    return free_dmac_channel(channel_num);
}

int hal_dmac_init(int8_t channel_num, uint32_t src_addr, uint32_t dst_addr,
                  uint32_t len)
{
    DmacChannelCfg *channel_cfg;
    int ret;
    if (!get_ops()->init || !get_ops()->open)
        return VSD_ERR_UNSUPPORTED;
    if (channel_num < 0)
        return VSD_ERR_INVALID_PARAM;

    channel_cfg = get_channel_cfg(channel_num);
    if (!channel_cfg)
        return VSD_ERR_INVALID_PARAM;
    channel_cfg->src_addr = src_addr;
    channel_cfg->dst_addr = dst_addr;
    channel_cfg->len      = len;

    ret = get_ops()->init(get_hw_cfg(), channel_cfg);
    if (ret != VSD_SUCCESS)
        return ret;
    return get_ops()->open(get_hw_cfg(), channel_cfg);
}

int hal_dmac_start(int8_t channel_num, DmaCbAndParam *xfer_cb)
{
    if (!get_ops()->start)
        return VSD_ERR_GENERIC;
    if (channel_num < 0)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops()->start(get_hw_cfg(), get_channel_cfg(channel_num), xfer_cb) !=
        VSD_SUCCESS)
        return VSD_ERR_GENERIC;

    return VSD_SUCCESS;
}

void hal_dmac_isr_handler(void)
{
    if (get_ops()->isr_handler)
        get_ops()->isr_handler(get_hw_cfg());
}

int hal_dmac_checkdone(int8_t channel_num)
{
    if (!get_ops()->check_done)
        return VSD_ERR_UNSUPPORTED;
    if (channel_num < 0)
        return VSD_ERR_INVALID_PARAM;

    return get_ops()->check_done(get_channel_cfg(channel_num));
}
