/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_ecg.h
 * @brief public head file of ECG HAL
 */
#pragma once

/** @addtogroup ECG
 *  ECG sensor hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include "hal_common.h"
#include "hal_sensor_common.h"
#include "hal_gpio.h"
#include "vsd_common.h"

/**
 * @brief ecg gain up or down
 */
enum e_ecg_gain_status {
    ECG_GAIN_UP   = 0x0, /**< gain up */
    ECG_GAIN_DOWN = 0x1, /**< gain down */
};

/**
 * @brief ecg sensor lead-off status
 */
enum EcgLodState {
    ECG_LOD_ON   = 0x0, /**< lead-on */
    ECG_ONLYN_ON = 0x1, /**< negative lead-on */
    ECG_ONLYP_ON = 0x2, /**< positive lead-on */
    ECG_LOD_OFF  = 0x3, /**< lead-off */
};

/**
 * @brief generic ecg interrupt
 */
enum e_ecg_interrupt {
    ECG_NOT_INTERRUPT = 0x0, /**< no interrupt detected */
    ECG_DATA_READY    = 0x1, /**< data is ready interrupt*/
    ECG_FIFO_A_FULL   = 0x2, /**< fifo almost full interrupt */
};

/**
 * @brief ecg channel ids mask
 */
enum e_ecg_channel_id {
    ECG_CHANNEL_ID_0         = 0x1, /**< ecg channel 0 mask */
    ECG_CHANNEL_ID_1         = 0x1 << 1, /**< ecg channel 1 mask */
    ECG_CHANNEL_ID_2         = 0x1 << 2, /**< ecg channel 2 mask */
    ECG_CHANNEL_ID_3         = 0x1 << 3, /**< ecg channel 3 mask */
    ECG_CHANNEL_ID_4         = 0x1 << 4, /**< ecg channel 4 mask */
    ECG_CHANNEL_ID_5         = 0x1 << 5, /**< ecg channel 5 mask */
    ECG_CHANNEL_ID_6         = 0x1 << 6, /**< ecg channel 6 mask */
    ECG_CHANNEL_ID_7         = 0x1 << 7, /**< ecg channel 7 mask */
    ECG_CHANNEL_ALL4_WITHLOD = 0x800f, /**< ecg 4 channels with lead-off ch */
    ECG_CHANNEL_ALL8_WITHLOD = 0x80ff, /**< ecg 8 channels with lead-off ch */
    ECG_CHANNEL_LOD          = 0x8000, /**< ecg lead-off channel mask */
};

/**
 * @brief ecg input pin ids, need to map to specific ecg sensor in hal
 */
enum e_ecg_pin_id {
    ECG_PIN_ID_0 = 0, /**< ecg input pin id:0 */
    ECG_PIN_ID_1, /**< ecg input pin id:1 */
    ECG_PIN_ID_2, /**< ecg input pin id:2 */
    ECG_PIN_ID_3, /**< ecg input pin id:3 */
    ECG_PIN_ID_4, /**< ecg input pin id:4 */
    ECG_PIN_ID_5, /**< ecg input pin id:5 */
    ECG_PIN_ID_6, /**< ecg input pin id:6 */
    ECG_PIN_ID_7, /**< ecg input pin id:7 */
    ECG_PIN_ID_8, /**< ecg input pin id:8 */
    ECG_PIN_ID_9, /**< ecg input pin id:9 */
    ECG_PIN_ID_10, /**< ecg input pin id:10 */
    ECG_PIN_ID_11, /**< ecg input pin id:11 */
};

typedef struct EcgReadDataParam {
    XferCallback app_callback;
    uint16_t ch_mask;
    uint8_t *read_data;
    uint32_t *ecg_samples;
    uint32_t buf_size; // in sample
    uint16_t *out_sample_num;
} EcgReadDataParam;

typedef struct {
    const BusDevice *bus_device; /**< pointer to bus device  */
    const BusConfig *bus_config; /**< pointer to bus configuration  */
    const SensorHwConfig *sen_cfg; /**< Sensor relating hw config */
    uint32_t hw_capability; /**< ecg capability with supported channels,
                               definition @see e_ecg_channel_id*/
    uint8_t chl_map; /**< map of ecg channel to pin, e.g.(1 << channel_mask) */
    uint8_t started; /**< 0 for stopped, 1 for started */
    uint8_t device_id; /**< ecg device internal ID  */
    uint8_t lod_state; /**< ecg lead-off status @see EcgLodState  */
    uint8_t work_mode; /**< ecg work mode:1 standby,0 SDATAC,2 RDATAC*/
    uint8_t samplerate_config; /**< ecg samplerate configuration  */
    uint8_t read_ch_sum; /**< The sum of reading channels, including lead-off */
    uint16_t channel_mode; /**< ecg channel mode:find which channels
                              work,definition @see e_ecg_channel_id*/
    SensorDataReadyHandler data_ready_callback; /**< ecg interrupt function */
    void *combo_ctx; /**< pointer to combo_ctx */
    EcgReadDataParam read_data_param;
    /** ECG read buffer to store reading data from ECG data register */
    uint8_t *read_buffer;
} EcgContext;

typedef struct {
    const char *name; /**< name of the specific ecg sensor */
    BusDevice bus_device; /**< pointer to ecg bus device */
    BusConfig bus_config; /**< pointer to ecg bus configuration */
    uint8_t type; /**< type of sensor, @see e_sensor_type */
    uint8_t flag; /**< flag set by sensor hal */
    uint32_t capability; /**< capability of the sensor */
    uint32_t default_sr; /**< default sample rate of the sensor */
    bool dma_mode; /**< whether to use dma mode */
    EcgContext *ctx; /**< ecg context */
    /** power control the device */
    int (*power)(EcgContext *ecg_ctx, uint8_t enable);
    int (*init)(EcgContext *ecg_ctx); /**< init device */
    int (*deinit)(EcgContext *ecg_ctx); /**< deinit device */
    int (*reset)(EcgContext *ecg_ctx); /**< reset device */
    /**
     * get how many ecg channels are supported.
     */
    uint8_t (*get_data_channel_num)(EcgContext *ecg_ctx);
    /**
     * configure ecg channel "channelid", set pin p_pin to the positive
     * input and set pin n_pin to it's negative input
     */
    int (*configure_channel)(EcgContext *ecg_ctx, uint8_t channelid,
                             uint8_t p_pin, uint8_t n_pin);
    /** configure pinid as the right leg driver pin */
    int (*configure_rld_pin)(EcgContext *ecg_ctx, uint8_t pinid);
    int (*enable_channel)(EcgContext *ecg_ctx, uint16_t channel_mask);
    /** configure sample rate */
    int (*set_samplerate)(EcgContext *ecg_ctx, uint32_t samplerate);
    /** get sample rate */
    int (*get_samplerate)(EcgContext *ecg_ctx, uint32_t *samplerate);
    /**
     * @brief set fifo watermark to indicate how many new samples can be written
     * to the FIFO before the interrupt is asserted
     */
    int (*set_fifo_watermark)(EcgContext *ctx, uint8_t watermark);
    /**
     * @brief check whether sample is available
     * @param check_sensor indicate to check on sensor or cache
     * @param chmask which channel to check, each bit maps to a channel id
     * @param return number of available sample
     */
    int (*get_sample_available)(EcgContext *ecg_ctx, uint8_t check_sensor,
                                uint8_t *chmask);
    /**
     * read ECG samples from channel chid in raw format
     * raw format samples are read from ADC result without converting
     * to voltage. different ECG sensors may have different meaning
     */
    int (*read_sample_raw)(EcgContext *ecg_ctx, uint16_t channel_mask,
                           uint32_t *samples, uint32_t buf_size,
                           uint16_t *re_count);
    /**
     * read ECG samples from channel chid in raw format by DMA;
     * raw format samples are read from ADC result without converting
     * to voltage. different ECG sensors may have different meaning
     */
    int (*read_sample_dma)(EcgContext *ecg_ctx, uint16_t channel_mask,
                           uint32_t *samples, uint32_t buf_size,
                           uint16_t *re_count, XferCallback callback);
    int (*start)(EcgContext *ecg_ctx); /**< start ecg collecting */
    /**
     * @brief stop ecg collecting.
     * @note when stopped, the sensor enters low power mode automatically
     */
    int (*stop)(EcgContext *ecg_ctx);

    /** enable interrupt */
    int (*enable_interrupt)(EcgContext *ecg_ctx, bool enable,
                            SensorDataReadyHandler data_ready_callback);
    /** read interrupt */
    int (*read_interrupt)(EcgContext *ecg_ctx, uint8_t *intr);
    /** enable/disable lead off detection */
    int (*enable_lod_detect)(EcgContext *ecg_ctx, uint8_t enable);
    /**
     * @brief which input pins are lead off.
     * @return the bit map of lead off pins
     */
    int (*detect_lod)(EcgContext *ecg_ctx, uint8_t *connected);
    /** write ECG registers */
    int (*write_regs)(EcgContext *ecg_ctx, uint8_t reg, uint8_t val);
    /** read ECG registers */
    int (*read_regs)(EcgContext *ecg_ctx, uint8_t reg, uint8_t *val);
    /** adjust ECG gain */
    int (*auto_adjust_gain)(EcgContext *ecg_ctx, uint16_t channel_id,
                            uint8_t gain_direction);
    /** read ecg samples over callback */
    int (*read_sample_back)(SensorReadBackHandler read_ecg,
                            SensorReadBackData *raw_data, uint8_t *ecg_sample,
                            uint16_t ecg_size, uint16_t *re_count);
} EcgDevice;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ecg hal API to read samples
 * @param ecg_device The ecg sensor device
 * @param channel_mask ID of channels to read sample from, definition @see
 * e_ecg_channel_id
 * @param sample The buffer to store the sample data(format:channel0,1,2...)
 * @param buf_size Sample buffer memory size (count of uint32_t sample)
 * @param re_count return the length of sample read back in buffer (count of
 * uint32_t sample)
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_read_sample(EcgDevice *ecg_device, uint16_t channel_mask,
                        uint32_t *sample, uint32_t buf_size,
                        uint16_t *re_count);

/**
 * @brief ecg hal API to read samples by DMA
 * @param ecg_device The ecg sensor device
 * @param channel_mask ID of channels to read sample from, definition @see
 * e_ecg_channel_id
 * @param sample The buffer to store the sample data(format:channel0,1,2...)
 * @param buf_size Sample buffer memory size (count of uint32_t sample)
 * @param re_count return the length of sample read back in buffer (count of
 * uint32_t sample)
 * @param callback The callback for transfer finish
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_read_sample_dma(EcgDevice *ecg_device, uint16_t channel_mask,
                            uint32_t *sample, uint32_t buf_size,
                            uint16_t *re_count, XferCallback callback);

/**
 * @brief ecg hal API to enable lead off detect
 * @param ecg_device The ecg sensor device
 * @param enable 1: enable the lead-off detect 0: disable the lead-off detect
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_enable_lod_detect(EcgDevice *ecg_device, uint8_t enable);

/**
 * @brief ecg hal API to get lead off detect status
 * @note some ECG sensor doesn't support this feature
 * @param ecg_device The ecg sensor device
 * @param connected The connect status of lead-off, @see EcgLodState
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_lod_detect(EcgDevice *ecg_device, uint8_t *connected);

/**
 * @brief ecg hal API to configure channel with pin
 * @note hardware channel definition @see e_ecg_pin_id
 * @param ecg_device The ecg sensor device
 * @param channel_mask Configure ecg channel "channel_mask", definition @see
 * e_ecg_channel_id
 * @param p_pin Set pin p_pin to channel positive input
 * @param n_pin Set pin n_pin to channel negative input
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_configure_channel(EcgDevice *ecg_device, uint8_t channel_mask,
                              uint8_t p_pin, uint8_t n_pin);

/**
 * @brief ecg hal API to configure right leg driver with pin
 * @note mode/pin_id definition @see e_ecg_pin_id
 * @param ecg_device The ecg sensor device
 * @param pinid Configure pinid as the right leg driver pin,definition @see
 * e_ecg_pin_id
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_configure_rld_pin(EcgDevice *ecg_device, uint8_t pinid);

/**
 * @brief ecg hal API to enable channels
 * @note channel definition @see e_ecg_channel_id
 * @param ecg_device The ecg sensor device
 * @param channel_mask config channels id to be opened, definition @see
 * e_ecg_channel_id
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_enable_channel(EcgDevice *ecg_device, uint16_t channel_mask);

/**
 * @brief ecg hal API to set sample rate
 * @note the samplerate is physical value, e.g. 200 for 200Hz
 * @param ecg_device The ecg sensor device
 * @param samplerate The sample rate need to be set
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_set_samplerate(EcgDevice *ecg_device, uint32_t samplerate);

/**
 * @brief ecg hal API to set sample rate
 * @param ecg_device The ecg sensor device
 * @param samplerate Returned sample rate
 * @return Return result
 *   @retval 0 for succeed, others for failure
 */
int hal_ecg_get_samplerate(EcgDevice *ecg_device, uint32_t *samplerate);

/**
 * @brief ecg hal API to set fifo watermark
 * @note this function decide how many new samples can be written to the FIFO
 * before the interrupt is asserted. For example, if watermar set to 0xF, fifo
 * size is 0x20, the interrupt triggers when there is 15 empty space left (17
 * data samples), and so on.
 * @param ecg_device The ecg sensor device
 * @param watermark fifo watermark to be set
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_set_fifo_watermark(EcgDevice *ecg_device, uint8_t watermark);

/**
 * @brief ecg hal API to start conversion
 * @param ecg_device The ecg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_start(EcgDevice *ecg_device);

/**
 * @brief ecg hal API to stop conversion
 * @param ecg_device The ecg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_stop(EcgDevice *ecg_device);

/**
 * @brief ecg hal API to enable data ready interrupt
 * @param ecg_device The ecg sensor device
 * @param enable 1:enable the ecg interrupt 0: disable the ecg interrupt
 * @param data_ready_callback function to be called by interrupt
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_enable_interrupt(EcgDevice *ecg_device, uint8_t enable,
                             SensorDataReadyHandler data_ready_callback);

/**
 * @brief ecg hal API to read interrupt status
 * @param ecg_device The ecg sensor device
 * @param intr The interrupt type, definition @see e_ecg_interrupt
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_read_interrupt(EcgDevice *ecg_device, uint8_t *intr);

/**
 * @brief ecg hal API to init
 * @param ecg_device The ecg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_init(EcgDevice *ecg_device);

/**
 * @brief ecg hal API to reset device
 * @param ecg_device The ecg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_reset(EcgDevice *ecg_device);

/**
 * @brief ecg hal API to destroy
 * @param ecg_device The ecg sensor device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_deinit(EcgDevice *ecg_device);

/**
 * @brief ecg hal API to power on/off
 * @param ecg_device The ecg sensor device
 * @param enable 1:power on , 0: power off
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_power(EcgDevice *ecg_device, uint8_t enable);

/**
 * @brief ecg hal API to write register
 * @param ecg_device The ecg sensor device
 * @param reg register address
 * @param val write register value
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_write_register(EcgDevice *ecg_device, uint8_t reg, uint8_t val);

/**
 * @brief ecg hal API to read register
 * @param ecg_device The ecg sensor device
 * @param reg register address
 * @param val read register value
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_read_register(EcgDevice *ecg_device, uint8_t reg, uint8_t *val);

/**
 * @brief ecg hal API to auto change ecg gain
 * @param ecg_device The ecg sensor device
 * @param channel_id The id of channels to adjust gain,definition @see
 * e_ecg_channel_id
 * @param gain_direction gain up or down, definition @see e_ecg_gain_status
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_auto_gain_adjust(EcgDevice *ecg_device, uint16_t channel_id,
                             uint8_t gain_direction);

/**
 * @brief ecg hal API to read sample back with SensorReadBackHandler
 * @param ecg_device The ecg sensor device
 * @param read_ecg ecg readback function
 * @param raw_data raw data to be read back
 * @param ecg_sample ecg sample buffer
 * @param ecg_size ecg sample buffer size
 * @param re_count ecg sample count
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_ecg_read_sample_callback(EcgDevice *ecg_device,
                                 SensorReadBackHandler read_ecg,
                                 SensorReadBackData *raw_data,
                                 uint8_t *ecg_sample, uint16_t ecg_size,
                                 uint16_t *re_count);

/** @} */

#ifdef __cplusplus
}
#endif
