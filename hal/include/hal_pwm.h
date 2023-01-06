/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pwm.h
 * @brief Public head file of PWM HAL
 */

#ifndef _HAL_PWM_H
#define _HAL_PWM_H

#include <stdint.h>

/** @addtogroup PWM
 *  Pulse Width Modulation hal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @enum PwmResolutionType
 * @brief PWM resolution select
 */
typedef enum { PWM_MODE_16_BIT, PWM_MODE_8_BIT } PwmResolutionType;

/**
 * @enum PwmClockType
 * @brief PWM clock select
 */
typedef enum { PWM_APB_CLK, PWM_LOW_CLK } PwmClockType;

/**
 * @enum PwmMultSampleConfig
 * @brief PWM whether enable multi sample
 */
typedef enum { PWM_MULTI_DIS, PWM_MULTI_EN } PwmMultSampleConfig;

/**
 * @enum PwmPolarityType
 * @brief PWM Polarity Type
 */
typedef enum { PWM_POLARITY_NORMAL = 0, PWM_POLARITY_INVERT } PwmPolarityType;

/** Play sample once */
#define PWM_REPEAT_DEFAULT (0)

/**
 * @struct PwmHwConfig
 * @brief PWM hardware configuration
 */
typedef struct PwmHwConfig {
    uint32_t addr; /**< Base-address of PWM */
    uint16_t clk_id; /**< Clock ID of this PWM device */
    uint16_t irq_id; /**< IRQ ID of this PWM device */
    uint8_t ch_num; /**< Number of PWM channels */
} PwmHwConfig;

/**
 * @struct PwmSettings
 * @brief PWM software configuration
 */
typedef struct PwmSettings {
    uint8_t channel; /**< Indicates which channel will be used */
    uint8_t multi_en : 1; /**< Use multi sample, @see PwmMultSampleConfig */
    uint8_t clk_sel : 2; /**< Clock source, @see PwmClockType */
    uint8_t resolution : 2; /**< 8/16bit resolution, @see PwmResolutionType */
    uint8_t intr_en : 1; /**< Whether to use interrupt mode */
    uint8_t invert : 1; /**< Whether to invert output polarity, @see
                         * PwmPolarityType  */
    uint8_t reserved : 1; /**< Reserved */
    uint16_t repeat; /**< The number of times the waveform is repeated in repeat
                        mode */
    uint32_t period_us; /**< The period you want to reach, in microsecond */
    uint32_t duty_cycle_us; /**< High level period. Configured in non multi
                               sample mode */
    uint32_t duty_sample[4]; /**< The high level period of each sample */
} PwmSettings;

/**
 * @struct PwmDevice
 * @brief PWM device, which is extensible to support context
 */
typedef struct PwmDevice {
    uint8_t device_id; /**< Id of a PWM device, @see PwmIdDef */
    const PwmHwConfig *hw_config; /**< PWM hardware configuration */
    const void *ops; /**< Operations of PWM driver */
} PwmDevice;

/**
 * @struct PwmOperation
 * @brief Prototype declaration of operations of the PWM device
 */
typedef struct PwmOperation {
    int (*init)(const PwmDevice *device, const PwmSettings *param);
    int (*start)(const PwmDevice *device, const PwmSettings *param);
    int (*stop)(const PwmDevice *device, uint16_t channel);
    void (*irq_handler)(const PwmDevice *device);
} PwmOperation;

/**
 * @enum PwmIdDef
 * @brief ID definition of PWM device
 */
typedef enum PwmIdDef {
    PWM_ID_0 = 0,
    PWM_ID_MAX,
} PwmIdDef;

/** limit PWM id number */
#define HAL_PWM_DEV_MAX (PWM_ID_MAX)

/**
 * @brief Add a PWM device instance
 *
 * @param device an instance of PWM device
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pwm_add_dev(PwmDevice *device);

/**
 * @brief Remove a PWM device
 *
 * @param device an PwmDevice instance to be removed
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pwm_remove_dev(PwmDevice *device);

/**
 * @brief Get reference of a PWM device
 *
 * @param device_id Id of a PWM device, @see PwmIdDef
 * @return PwmDevice* a PwmDevice instance for succeed, NULL for failure
 */
PwmDevice *hal_pwm_get_device(uint8_t device_id);

/**
 * @brief Initialize and config the channel for PWM device
 *
 * @param device an instance of PWM device
 * @param param parameter setting for a channel
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pwm_init(const PwmDevice *device, const PwmSettings *param);

/**
 * @brief Start the channel for PWM device
 *
 * @param device an instance of PWM device
 * @param channel param parameter setting for a channel
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pwm_start(const PwmDevice *device, const PwmSettings *param);

/**
 * @brief Stop the channel for PWM device
 *
 * @param device an instance of PWM device
 * @param channel channel id
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pwm_stop(const PwmDevice *device, uint16_t channel);

/**
 * @brief PWM interrupt handler
 *
 * @param device an instance of PWM device
 */
void hal_pwm_irq_handler(const PwmDevice *device);

/** @} */

#endif /* _HAL_PWM_H */
