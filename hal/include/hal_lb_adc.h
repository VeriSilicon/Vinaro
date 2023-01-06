/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_lb_adc.h
 * @brief Public head file of HAL for low band ADC
 * @author Alex Lin <alex.lin@verisilicon.com>
 */
#ifndef _HAL_LB_ADC_H_
#define _HAL_LB_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ADC
 *  Low band ADC HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>

/**
 * @brief Structure of operations for HAL of low band ADC
 */
typedef struct LbAdcOperations {
    /** Init the LB ADC */
    int (*init)(uint8_t port, uint32_t sampling_cycle);
    /** Read value from ADC */
    int (*read)(uint8_t port, uint32_t *value);
    /** Finalize the ADC port */
    int (*finalize)(uint8_t port);
} LbAdcOperations;

/**
 * @brief Define config for low band ADC
 */
typedef struct {
    uint32_t sampling_cycle; /**< adc clock cycles for sampling period */
} LbAdcCfg;

/**
 * @brief Define device hal handle for low band ADC
 */
typedef struct {
    uint8_t port; /**< adc port */
    LbAdcCfg config; /**< adc config */
} LbAdcDev;

/**
 * @brief Create low band ADC driver
 * @param ops The ops of lb adc driver
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_lb_adc_create(const LbAdcOperations *ops);

/**
 * @brief Initialize the low band ADC
 * @param adc the adc which need to be initialized
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_lb_adc_init(LbAdcDev *adc);

/**
 * @brief Read value from the low band ADC
 * @param adc the adc which need to be read
 * @param value the pointer of read value
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_lb_adc_read(LbAdcDev *adc, uint32_t *value);

/**
 * @brief De-initialises the low band ADC, turn off an ADC
 * @param adc the adc which need to be de-initialised
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_lb_adc_finalize(LbAdcDev *adc);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // _HAL_LB_ADC_H_
