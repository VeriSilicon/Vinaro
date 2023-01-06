/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_lb_adc.c
 * @brief HAL for Low band ADC
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#include <stdint.h>
#include <stdlib.h>
#include "vsd_common.h"
#include "hal_lb_adc.h"
#include "vsd_error.h"

static const LbAdcOperations *adc_ops = NULL;

int hal_lb_adc_create(const LbAdcOperations *ops)
{
    if (ops) {
        adc_ops = ops;
        return VSD_SUCCESS;
    } else {
        return VSD_ERR_INVALID_POINTER;
    }
}

int hal_lb_adc_init(LbAdcDev *adc)
{
    if (!adc_ops || !adc_ops->init)
        return VSD_ERR_INVALID_POINTER;

    return adc_ops->init(adc->port, adc->config.sampling_cycle);
}

int hal_lb_adc_read(LbAdcDev *adc, uint32_t *value)
{
    if (!adc_ops || !adc_ops->read)
        return VSD_ERR_INVALID_POINTER;

    return adc_ops->read(adc->port, value);
}

int hal_lb_adc_finalize(LbAdcDev *adc)
{
    if (!adc_ops || !adc_ops->finalize)
        return VSD_ERR_INVALID_POINTER;

    return adc_ops->finalize(adc->port);
}
