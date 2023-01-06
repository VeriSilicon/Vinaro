/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_ppg_ecg_combo.h
 * @brief Public head file of PPG&ECG combo HAL
 */

#pragma once
#include "hal_ecg.h"
#include "hal_ppg.h"
#include "hal_sensor_common.h"
#include "sensor_log.h"
#include "osal_lock_api.h"

/** @addtogroup PPG_ECG_COMBO
 *  PPG & ECG combo sensor HAL definition and API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @brief combo device work mode
 */
enum e_active_sensor_type {
    ACTIVE_NO_SENSOR = 0, /**< sensor mode:no sensor is on */
    ACTIVE_ECG       = 0x1, /**< sensor mode:ECG is on */
    ACTIVE_PPG       = 0x2, /**< sensor mode:PPG is on */
    ACTIVE_PPG_ECG   = 0x3, /**< sensor mode:PPG&ECG is on */
};
enum e_int_enable_sensor_type {
    DISABLE_SENSOR_INT = 0, /**< disable all sensor int */
    ENABLE_ECG_INT     = 0x1, /**< enable ECG interrupt */
    ENABLE_PPG_INT     = 0x2, /**< enable PPG interrupt */
    ENABLE_PPG_ECG_INT = 0x3, /**< enable PPG&ECG interrupt */
};
enum e_sensor_read_flag_type {
    NO_SENSOR_READ    = 0, /**< no sensor readback flag */
    ECG_READ_FLAG     = 0x1, /**< ECG readback flag */
    PPG_READ_FLAG     = 0x2, /**< PPG readback flag */
    PPG_ECG_READ_FLAG = 0x3, /**< PPG&ECG readback flag */
};

/**
 * @brief combo device context
 */
typedef struct _ComboContext {
    const BusDevice *bus_device; /**< pointer to bus device */
    const BusConfig *bus_config; /**< pointer to bus configuration */
    PpgContext *ppg_ctx; /**< pointer to ppg context */
    EcgContext *ecg_ctx; /**< pointer to ecg context */
    const SensorHwConfig *sen_cfg; /**< pointer to sensor hw config */
    uint8_t config_sensor; /**< sensor work mode configuration */
    uint8_t active_sensor; /**< sensor work mode in real time */
    uint8_t int_on_sensor; /**< which sensor enable interrupt */
    bool init_status; /**< if the combo sensor has been initialized */
    uint8_t config_led_level; /**< ppg led level configuration value */
    uint8_t readback_flag; /**< which sensor data is in readback buffer*/
    SensorReadBackData raw_data; /**< raw data read back from fifo*/
    SensorDataReadyHandler ppg_ready_handle; /**< ppg data ready callback func*/
    SensorDataReadyHandler ecg_ready_handle; /**< ecg data ready callback func*/
    uint32_t device_id; /**< device id in multi device case*/
    uint32_t ppg_rate; /**< ppg sample rate*/
    uint32_t ecg_rate; /**< ecg sample rate*/
    bool fifo_ready; /**< check if fifo is ready*/
    OsalMutex mutex; /**< mutex to protect data accessing */
} PpgEcgComboContext;

typedef struct _PpgEcgComboDevice {
    BusDevice bus_device; /**< pointer to bus device */
    BusConfig bus_config; /**< pointer to bus configuration */
    EcgDevice *ecg_device; /**< pointer to internal ECG device */
    PpgDevice *ppg_device; /**< pointer to internal PPG device */

    PpgEcgComboContext *ctx; /**< pointer to combo context */

    int (*init)(PpgEcgComboContext *combo_ctx); /**< init device */
    int (*deinit)(PpgEcgComboContext *combo_ctx); /**< deinit device */
    int (*reset)(PpgEcgComboContext *combo_ctx); /**< reset to the same status
                                                  as inited */
    int (*wake_up)(PpgEcgComboContext *combo_ctx); /**< wake up device */
    int (*shut_down)(PpgEcgComboContext *combo_ctx); /**< shut down device */
    /** read sample to different buffer */
    int (*read_sample)(PpgEcgComboContext *combo_ctx, uint8_t *red, uint8_t *ir,
                       uint8_t *ecg, uint32_t count, int samplesize,
                       bool bigendian, uint16_t *return_count);
    /** read mixed sample in bytes to one buffer */
    int (*read_sample_bytes)(PpgEcgComboContext *combo_ctx, uint8_t *samples,
                             uint32_t count, int samplesize, bool bigendian,
                             uint16_t *return_count);
    /** read interrupt status */
    int (*read_interrupt)(PpgEcgComboContext *combo_ctx, uint8_t *val);
    /** set active sensor mode */
    int (*set_work_mode)(PpgEcgComboContext *combo_ctx, uint8_t sensor);
    /**set fifo watermark to indicate how many new samples can be written
     * to the FIFO before the interrupt is asserted */
    int (*set_fifo_watermark)(PpgEcgComboContext *combo_ctx, uint8_t watermark);
    /** enable interrupt with callback function */
    int (*enable_interrupt)(PpgEcgComboContext *combo_ctx, bool enable,
                            SensorDataReadyHandler data_ready_callback);
    int (*get_active_channel)(PpgEcgComboContext *combo_ctx,
                              uint8_t *cnt); /**< get sum of active channels */

} PpgEcgComboDevice;

/**
 * @brief combo device hal API to init device
 * @param combo_device The combo sensor device
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_init(PpgEcgComboDevice *combo_device);

/**
 * @brief combo hal API to wake up device
 * @param combo_device The combo sensor device
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_wake_up(PpgEcgComboDevice *combo_device);

/**
 * @brief combo hal API to shut down device
 * @param combo_device The combo sensor device
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_shut_down(PpgEcgComboDevice *combo_device);

/**
 * @brief combo hal API to reset device
 * @param combo_device The combo sensor device
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_reset(PpgEcgComboDevice *combo_device);

/**
 * @brief combo hal API to read sample to 3 diff channels in
 *        4_BYTE_LITTLEENDIAN format
 * @param combo_device The combo sensor device
 * @param red buffer to store red light data
 * @param ir buffer to store ir light data
 * @param ecg buffer to store ecg light data
 * @param count (red/ir/ecg) buffer memory size(count of uint32_t sample).If
 * not NULL,red,ir,ecg buffer have the same memory size
 * @param sample_format sample format in buffer, definition @see
 * e_ppg_sample_format
 * @param return_count the length of samples read back to buffer
 * (count of uint32_t sample).If not NULL,red,ir,ecg samples have the same
 * length
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_read_sample(PpgEcgComboDevice *combo_device, uint8_t *red,
                            uint8_t *ir, uint8_t *ecg, uint16_t count,
                            int sample_format, uint16_t *return_count);

/**
 * @brief combo hal API to read mixed buffer in diff sample format
 * @param combo_device The combo sensor device
 * @param samples buffer to store light data in mixed order (red)+(ir)+(ecg)...
 * @param count  buffer memory size(count of uint8_t sample)
 * @param sample_format sample format in buffer, definition @see
 * e_ppg_sample_format
 * @param return_count he length of samples read back to buffer(count of
 * uint8_t sample)
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_read_sample_bytes(PpgEcgComboDevice *combo_device,
                                  uint8_t *samples, uint16_t count,
                                  int sample_format, uint16_t *return_count);

/**
 * @brief combo hal API to enable/disable interrupt
 * @param combo_device The combo sensor device
 * @param enable check if enable interrupt
 * @param data_ready_callback call-back function when interrupt is triggered
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_enable_interrupt(PpgEcgComboDevice *combo_device, bool enable,
                                 SensorDataReadyHandler data_ready_callback);

/**
 * @brief combo hal API to get ppg data ready status
 * @param combo_device The combo sensor device
 * @param val ppg data ready status
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_read_interrupt(PpgEcgComboDevice *combo_device, uint8_t *val);

/**
 * @brief combo hal API to set combo device work mode
 * @param combo_device The combo sensor device
 * @param sensor_mode combo sensor work mode configuration
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_set_work_mode(PpgEcgComboDevice *combo_device,
                              uint8_t sensor_mode);

/**
 * @brief combo hal API to get active channel num
 * @param combo_device The combo sensor device
 * @param cnt active channel count
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ppg_ecg_get_active_channel_num(PpgEcgComboDevice *combo_device,
                                       uint8_t *cnt);

/** @} */
