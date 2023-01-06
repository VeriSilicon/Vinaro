/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_ppg.h
 * @brief Public head file of PPG HAL
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "hal_spi.h"
#include "hal_i2c.h"
#include "hal_gpio.h"
#include "device.h"
#include "board.h"
#include "vsd_common.h"
#include "hal_sensor_common.h"

/** @addtogroup PPG
 *  PPG sensor HAL definition and API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define PPG_SAMPLE_2_BYTE_LITTLEENDIAN (SENSOR_SAMPLE_2_BYTE_LITTLEENDIAN)
#define PPG_SAMPLE_4_BYTE_LITTLEENDIAN (SENSOR_SAMPLE_4_BYTE_LITTLEENDIAN)
#define PPG_SAMPLE_3_BYTE_LITTLEENDIAN (SENSOR_SAMPLE_3_BYTE_LITTLEENDIAN)
#define PPG_SAMPLE_2_BYTE_BIGENDIAN (SENSOR_SAMPLE_2_BYTE_BIGENDIAN)
#define PPG_SAMPLE_4_BYTE_BIGENDIAN (SENSOR_SAMPLE_4_BYTE_BIGENDIAN)
#define PPG_SAMPLE_3_BYTE_BIGENDIAN (SENSOR_SAMPLE_3_BYTE_BIGENDIAN)

/**
 * @brief Definition for PPG device ID
 */
typedef enum PpgDeviceId {
    PPG_DEV_ID_UNKNOWN = 0, /**< Unknown PPG */
    PPG_DEV_ID_MAX3010X, /**< MAX3010X */
    PPG_DEV_ID_MAX86150, /**< MAX86150 */
} PpgDeviceId;

/**
 * @brief ppg data ready call back function
 */
typedef void (*PPGDataReadyHandler)(void);

/**
 * @brief ppg context in driver
 */
typedef struct _PpgContext {
    const BusDevice *bus_device; /**< pointer to ppg bus device */
    const BusConfig *bus_config; /**< pointer to ppg bus configuration */
    const SensorHwConfig *sen_cfg; /**< pointer to sensor hw config */
    uint8_t ppg_mode; /**< current ppg work mode, definition @see
                             e_ppg_workmode*/
    uint32_t hw_capability; /**< ppg sensor hardware capability, e.g. supported
                               light mode, definition @see e_ppg_workmode */
    void *combo_ctx; /**< pointer to combo context */
} PpgContext;

/**
 * @brief ppg device struct for hal
 */
typedef struct _PpgDevice {
    BusDevice bus_device; /**< pointer to ppg bus device */
    BusConfig bus_config; /**< pointer to ppg bus configuration */
    uint8_t device_id; /**< PPG device ID, definition @see PpgDeviceId */
    uint32_t capability; /**< sensor capability set by user,definition @see
                            e_ppg_workmode */
    PpgContext *ctx; /**< pointer to ppg context */

    int (*init)(PpgContext *ctx); /**< init device */
    int (*wake_up)(PpgContext *ctx); /**< wake up device */
    int (*shut_down)(PpgContext *ctx); /**< shut down device */
    int (*reset)(PpgContext *ctx); /**< reset device to default status*/
    /**
     * @brief turn on/off ppg power
     */
    int (*power_on)(PpgContext *ctx, bool is_power_on);
    int (*set_led_amplitude)(PpgContext *ctx, uint8_t powerLevel,
                             uint8_t light_type); /**< set led amplitude */
    int (*get_led_amplitude)(PpgContext *ctx, uint8_t *powerLevel,
                             uint8_t light_type); /**< get led amplitude */
    /**
     * @brief set led adc range
     */
    int (*set_adc_range)(PpgContext *ctx, uint32_t adcRange);

    /**
     * @brief set ppg sample rate
     */
    int (*set_samplerate)(PpgContext *ctx, uint32_t rate);

    /**
     * @brief get ppg sample rate
     */
    int (*get_samplerate)(PpgContext *ctx, uint32_t *rate);

    /**
     * @brief set ppg pulse width
     */
    int (*set_pulsewidth)(PpgContext *ctx, uint16_t pulseWidth);

    /**
     * @brief set ppg work mode with (red/ir/green) light mode
     */
    int (*set_work_mode)(PpgContext *ctx, uint8_t ledMode);

    /**
     * @brief set fifo watermark to indicate how many new samples can be written
     * to the FIFO before the interrupt is asserted
     */
    int (*set_fifo_watermark)(PpgContext *ctx, uint8_t watermark);

    /**
     * @brief read interrupt status
     */
    int (*read_interrupt)(PpgContext *ctx, uint8_t *int_status);
    /**
     * @brief read sample to 3 different buffer in diff sample format
     */
    int (*read_sample)(PpgContext *ctx, uint8_t *red, uint8_t *ir,
                       uint8_t *green, uint16_t count, int samplesize,
                       bool bigendian, uint16_t *return_count);
    /**
     * @brief read sample to mixed one buffer in diff sample format
     */
    int (*read_sample_bytes)(PpgContext *ctx, uint8_t *samples, uint16_t count,
                             int samplesize, bool bigendian,
                             uint16_t *return_size);
    /**
     * @brief enable/disable interrupt
     */
    int (*enable_interrupt)(PpgContext *ctx, bool enable,
                            SensorDataReadyHandler data_ready_callback);
    /**
     * @brief read register
     */
    int (*read_register)(PpgContext *ctx, uint8_t reg, uint8_t *val);
    /**
     * @brief write register
     */
    int (*write_register)(PpgContext *ctx, uint8_t reg, uint8_t val);
    /**
     * @brief read sample back with SensorReadBackHandler
     */
    int (*read_sample_back)(SensorReadBackHandler read_ppg,
                            SensorReadBackData *raw_data, uint8_t *ppg_sample,
                            uint16_t ppg_size, uint16_t *re_count);
} PpgDevice;

/**
 * @brief PPG work mode
 *
 */
enum e_ppg_workmode {
    PPG_MODE_IDLE       = 0, /**< ppg idle mode */
    PPG_MODE_RED        = 0x1, /**< ppg red light mode */
    PPG_MODE_IR         = 0x2, /**< ppg ir light mode */
    PPG_MODE_REDIR      = 0x3, /**< ppg red/ir light mode */
    PPG_MODE_GREEN      = 0x4, /**< ppg green light mode */
    PPG_MODE_REDIRGREEN = 0x7, /**< ppg red/ir/green light mode */
};

/**
 * @brief PPG led light type, one bit stands for one light type
 *
 */
enum e_ppg_light_type {
    PPG_NO_LIGHT    = 0, /**< ppg no light type */
    PPG_RED_LIGHT   = 0x1, /**< ppg red light type, 0x1 is red light*/
    PPG_IR_LIGHT    = 0x2, /**< ppg ir light type, 0x1<<1 is ir light */
    PPG_REDIR_LIGHT = 0x3, /**< ppg red/ir light type */
    PPG_GREEN_LIGHT = 0x4, /**< ppg green light type, 0x1<<2 is green light */
    PPG_REDIRGREEN_LIGHT = 0x7, /**< ppg red/ir/green light type */
};

/**
 * @brief PPG interrupt status
 *
 */
enum PpgIntrStatus {
    PPG_NOT_INTERRUPT = 0x0, /**< ppg has no interrupt */
    PPG_DATA_READY    = 0x1, /**< ppg data-ready interrupt */
    PPG_FIFO_ALFULL   = 0x2, /**< ppg fifo almost full interrupt */
};

/**
 * @brief ppg hal API to init device
 * @param ppg_device The ppg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_init(PpgDevice *ppg_device);

/**
 * @brief ppg hal API to wake up device
 * @param ppg_device The ppg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_wake_up(PpgDevice *ppg_device);

/**
 * @brief ppg hal API to shut down device
 * @param ppg_device The ppg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_shut_down(PpgDevice *ppg_device);

/**
 * @brief ppg hal API to to reset device
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_reset(PpgDevice *ppg_device);

/**
 * @brief ppg hal API to enable or disable power
 * @param ppg_device The ppg sensor device
 * @param is_power_on 0 is disable power,1 is enable power
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_enable_power(PpgDevice *ppg_device, bool is_power_on);

/**
 * @brief ppg hal API to set led amplitude level
 * @param ppg_device The ppg sensor device
 * @param powerLevel amplitude level to be set
 * @param light_type led light type to be set amplitude level for, definition
 * @see e_ppg_workmode
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_set_led_amplitude(PpgDevice *ppg_device, uint8_t powerLevel,
                              uint8_t light_type);

/**
 * @brief ppg hal API to get led amplitude level
 * @param ppg_device The ppg sensor device
 * @param powerLevel pointer to where amplitude level will be read back
 * @param light_type led light type to get amplitude level from, definition @see
 * e_ppg_workmode
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_get_led_amplitude(PpgDevice *ppg_device, uint8_t *powerLevel,
                              uint8_t light_type);

/**
 * @brief ppg hal API to set led work mode
 * @param ppg_device The ppg sensor device
 * @param ledMode led work mode to be set, definition @see e_ppg_workmode
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_set_work_mode(PpgDevice *ppg_device, uint8_t ledMode);

/**
 * @brief ppg hal API to set led ADC range
 * @param ppg_device The ppg sensor device
 * @param range ppg ADC range to be set
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_set_adc_range(PpgDevice *ppg_device, uint32_t range);

/**
 * @brief ppg hal API to set sample rate
 * @param ppg_device The ppg sensor device
 * @param rate ppg sample rate to be set
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_set_samplerate(PpgDevice *ppg_device, uint32_t rate);

/**
 * @brief ppg hal API to set sample rate
 * @param ppg_device The ppg sensor device
 * @param rate pointer to where sample rate will be read back
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_get_samplerate(PpgDevice *ppg_device, uint32_t *rate);

/**
 * @brief ppg hal API to set pulse width
 * @param ppg_device The ppg sensor device
 * @param width ppg pulse width to be set
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_set_pulsewidth(PpgDevice *ppg_device, uint16_t width);

/**
 * @brief ppg hal API to set fifo watermark
 * @note this function decide how many new samples can be written to the FIFO
 * before the interrupt is asserted. For example, if watermar set to 0xF, fifo
 * size is 0x20, the interrupt triggers when there is 15 empty space left (17
 * data samples), and so on.
 * @param ppg_device The ppg sensor device
 * @param watermark fifo watermark to be set
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_set_fifo_watermark(PpgDevice *ppg_device, uint8_t watermark);

/**
 * @brief ppg hal API to read sample into 3 buffer in diff sample format
 * @note
 * - the exp_size should be no larger than buffer size;
 * - exp_size = (size of one sample in byte) * (expected reading sample numbers
 * with one light type). If not NULL,red,ir,green buffer have the same exp_size
 * - act_size: If not NULL,red,ir,green buffer have the same act_size
 * @param ppg_device The ppg sensor device
 * @param red buffer to store red light data
 * @param ir buffer to store ir light data
 * @param green buffer to store green light data
 * @param exp_size expected to read data from sensor in byte
 * @param sample_format sample format in buffer, definition @see
 * e_ppg_sample_format
 * @param act_size the actual length of samples in byte which read from sensor
 * and stored in the buffer
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_read_sample(PpgDevice *ppg_device, uint8_t *red, uint8_t *ir,
                        uint8_t *green, uint16_t exp_size, int sample_format,
                        uint16_t *act_size);

/**
 * @brief ppg hal API to mixed one buffer in diff sample format
 * @note
 * - data format likes: RED,IR,RED,IR for RED+IR mode;
 * - the exp_size should be no larger than buffer size;
 * - exp_size = (enabled LED number) * (size of one sample in byte) * (expected
 * reading sample numbers)
 * @param ppg_device The ppg sensor device
 * @param samples buffer to store light data in mixed order one by one,
 * e.g.(red)+(ir)+(green),(red)+(ir)+(green),....
 * @param exp_size expected to read data from sensor in byte
 * @param sample_format sample format in buffer, definition @see
 * e_ppg_sample_format
 * @param act_size the actual length of samples in byte which read from sensor
 * and stored in the buffer
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_read_sample_bytes(PpgDevice *ppg_device, uint8_t *samples,
                              uint16_t exp_size, int sample_format,
                              uint16_t *act_size);

/**
 * @brief ppg hal API to enable/disable interrupt
 * @param ppg_device The ppg sensor device
 * @param enable 0 is disable interrupt, 1 is enable interrupt
 * @param data_ready_callback call-back function when interrupt is triggered
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_enable_interrupt(PpgDevice *ppg_device, bool enable,
                             SensorDataReadyHandler data_ready_callback);

/**
 * @brief ppg hal API to get ppg data ready status
 * @param ppg_device The ppg sensor device
 * @param int_status ppg data ready status, definition @see PpgIntrStatus
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_read_interrupt(PpgDevice *ppg_device, uint8_t *int_status);

/**
 * @brief ppg hal API to read register
 * @param ppg_device The ppg sensor device
 * @param reg register address to be read
 * @param val register value
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_read_register(PpgDevice *ppg_device, uint8_t reg, uint8_t *val);

/**
 * @brief ppg hal API to write register
 * @param ppg_device The ppg sensor device
 * @param reg register address to be written
 * @param val register value to be written
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_write_register(PpgDevice *ppg_device, uint8_t reg, uint8_t val);

/**
 * @brief ppg hal API to read sample over callback
 * @param ppg_device The ppg sensor device
 * @param read_ppg ppg readback function
 * @param raw_data raw data to be read back
 * @param ppg_sample ppg sample buffer
 * @param ppg_size ppg sample buffer memory size
 * @param re_count the length of read back ppg sample
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ppg_read_sample_callback(PpgDevice *ppg_device,
                                 SensorReadBackHandler read_ppg,
                                 SensorReadBackData *raw_data,
                                 uint8_t *ppg_sample, uint16_t ppg_size,
                                 uint16_t *re_count);

/** @} */
