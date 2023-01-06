/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_imu.h
 * @brief Public head file of IMU HAL
 */
#ifndef _HALIMU_H_
#define _HALIMU_H_

/** @addtogroup IMU
 *  IMU hal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */
#include <stdbool.h>
#include "hal_spi.h"
#include "hal_i2c.h"
#include "hal_gpio.h"
#include "vsd_common.h"

#define FIFO_BUFF_SIZE (255)
#define FIFO_WATERMARK_LEVEL (49)

typedef void (*ImuDataReadyHandler)(void);

/**
 * @brief fifo type
 */
enum ImuFifoType {
    IMU_FIFO_GYRO     = 0x80, /**< gyro data */
    IMU_FIFO_ACCEL    = 0x40, /**< accel data */
    IMU_FIFO_AUX      = 0x20, /**< aux data */
    IMU_FIFO_TAG_IRQ1 = 0x08, /**< FIFO tag for INT1 pin */
    IMU_FIFO_TAG_IRQ2 = 0x04, /**< FIFO tag for INT2 pin*/
    IMU_FIFO_TIME     = 0x02, /**< sensortime frame */
    IMU_FIFO_HEADER   = 0x10, /**< header */
};

/**
 * @brief IMU sensor type
 */
enum ImuSensorType {
    IMU_ACCEL = 1, /**< accel sensor */
    IMU_GYRO, /**< gyro sensor*/
    IMU_ACCEL_GYRO, /**< accel and gyro sensors */
    IMU_MAG, /**< magnet sensor */
    IMU_ALL, /**< all sensor */
};

/**
 * @brief imu sensor configuration type
 *
 */
enum ImuSensorCfgType {
    IMU_SENSOR_RANGE = 0x01, /**< sensor range */
    IMU_SENSOR_ODR   = 0x02, /**< sensor output dadta rate */
    IMU_SENSOR_BWP   = 0x04, /**< sensor bandwidth parameter */
};

/**
 * @brief imu pin type
 */
enum ImuPinType {
    IMU_DATA_PIN = 1, /**< data pin */
    IMU_WAKE_PIN, /**< wake pin */
    IMU_POWER_PIN, /**< power pin */
};

/**
 * @brief imu bit value: 0-diable, 1-enable
 */
enum imu_bit_value {
    IMU_DISABLE = 0x00, /**< disable */
    IMU_ENABLE  = 0x01, /**< enable */
};

/**
 * @brief Latch Duration
 */
enum imu_latch_duration {
    IMU_LATCH_DUR_NONE            = 0x00, /**< non-latched */
    IMU_LATCH_DUR_312_5_MICRO_SEC = 0x01, /**< temporary, 312.5us */
    IMU_LATCH_DUR_625_MICRO_SEC   = 0x02, /**< temporary, 625us */
    IMU_LATCH_DUR_1_25_MILLI_SEC  = 0x03, /**< temporary, 1.25ms */
    IMU_LATCH_DUR_2_5_MILLI_SEC   = 0x04, /**< temporary, 2.5ms */
    IMU_LATCH_DUR_5_MILLI_SEC     = 0x05, /**< temporary, 5ms */
    IMU_LATCH_DUR_10_MILLI_SEC    = 0x06, /**< temporary, 10ms */
    IMU_LATCH_DUR_20_MILLI_SEC    = 0x07, /**< temporary, 20ms */
    IMU_LATCH_DUR_40_MILLI_SEC    = 0x08, /**< temporary, 40ns */
    IMU_LATCH_DUR_80_MILLI_SEC    = 0x09, /**< temporary, 80ms */
    IMU_LATCH_DUR_160_MILLI_SEC   = 0x0A, /**< temporary, 160ms */
    IMU_LATCH_DUR_320_MILLI_SEC   = 0x0B, /**< temporary, 320ms */
    IMU_LATCH_DUR_640_MILLI_SEC   = 0x0C, /**< temporary, 640ms */
    IMU_LATCH_DUR_1_28_SEC        = 0x0D, /**< temporary, 1.28s */
    IMU_LATCH_DUR_2_56_SEC        = 0x0E, /**< temporary, 2.56s */
    IMU_LATCHED                   = 0x0F, /**< latched */
};

/**
 * @brief interrupt channel selection structure
 */
enum interrupt_channel {
    IMU_INTERRUPT_CHANNEL_NONE, /**< Un-map both channels */
    IMU_INTERRUPT_CHANNEL_1, /**< interrupt Channel 1 */
    IMU_INTERRUPT_CHANNEL_2, /**< interrupt Channel 2 */
    IMU_INTERRUPT_CHANNEL_BOTH, /**< Map both channels */
};
/**
 * @brief interrupt type
 */
enum interrupt_types {
    IMU_ACC_ANY_MOTION_INTERRUPT, /**< Slope/Any-motion interrupt */
    IMU_ACC_SIG_MOTION_INTERRUPT, /**< Significant motion interrupt */
    IMU_STEP_DETECT_INTERRUPT, /**< Step detector interrupt */
    IMU_ACC_DOUBLE_TAP_INTERRUPT, /**< double tap interrupt */
    IMU_ACC_SINGLE_TAP_INTERRUPT, /**< single tap interrupt */
    IMU_ACC_ORIENT_INTERRUPT, /**< orientation interrupt */
    IMU_ACC_FLAT_INTERRUPT, /**< flat interrupt */
    IMU_ACC_HIGH_G_INTERRUPT, /**< high-g interrupt */
    IMU_ACC_LOW_G_INTERRUPT, /**< low-g interrupt */
    IMU_ACC_SLOW_NO_MOTION_INTERRUPT, /**< slow/no-motion interrupt */
    IMU_ACC_GYRO_DATA_RDY_INTERRUPT, /**< data ready interrupt */
    IMU_ACC_GYRO_FIFO_FULL_INTERRUPT, /**< fifo full interrupt */
    IMU_ACC_GYRO_FIFO_WATERMARK_INTERRUPT, /**< fifo watermark interrupt */
    IMU_FIFO_TAG_INTERRUPT_PIN, /**< fifo tagging feature support */
};
/**
 * @brief struct of high interrupt condition
 * @note can use `hal_imu_get_accel_range()` to get the range of accel.
 * The interrupt condition is cleared when the absolute value of acceleration
 * data of all selected axes falls below the threshold [threshold] minus the
 * hysteresis [hysteresis] or if the sign of the acceleration value changes
 */
typedef struct _IMUHighGCondition {
    /**
     * threshold of high-g interrupt according to
     * [threshold]*7.81mg (2g range), or [threshold]*15.63mg (4g range),
     * or [threshold]*31.25mg (8g range), or [threshold]*62.5mg (16g range)
     */
    uint8_t threshold;
    /**
     *  high-g range interrupt trigger delay according to
     * [duration]*2.5ms in a range from 2.5 to 640ms
     */
    uint8_t duration;
    /**
     * range is 0 to 3. hysteresis of high-g interrupt according
     * to [hysteresis]*125mg (2g range), or [hysteresis]*250mg (4g range), or
     * [hysteresis]*500mg (8g range), or [hysteresis]*1000mg (16g range)
     */
    uint8_t hysteresis;
} IMUHighGCondition;

/**
 * @brief interrupt pin setting
 */
typedef struct _IMUInterruptPinSetting {
    /** To enable either INT1 or INT2 pin as output.
     * 0- output disabled ,1- output enabled */
    uint16_t output_en : 1;

    /** 0 - push-pull 1- open drain,only valid if output_en is set 1 */
    uint16_t output_mode : 1;

    /** 0 - active low , 1 - active high level.
     * if output_en is 1,this applies to interrupts,else PMU_trigger */
    uint16_t output_type : 1;

    /** 0 - level trigger , 1 - edge trigger  */
    uint16_t edge_ctrl : 1;

    /** To enable either INT1 or INT2 pin as input.
     * 0 - input disabled ,1 - input enabled */
    uint16_t input_en : 1;

    /** latch duration*/
    uint16_t latch_dur : 4;
} IMUInterruptPinSetting;

/**
 * @brief accel tap interrupt configuration
 */
typedef struct _IMUAccTapInterruptCfg {
    /** tap threshold */
    uint16_t tap_thr : 5;

    /** tap shock */
    uint16_t tap_shock : 1;

    /** tap quiet */
    uint16_t tap_quiet : 1;

    /** tap duration */
    uint16_t tap_dur : 3;

    /** data source 0- filter & 1 pre-filter*/
    uint16_t tap_data_src : 1;

    /** tap enable, 1 - enable, 0 - disable */
    uint16_t tap_en : 1;
} IMUAccTapInterruptCfg;

/**
 * @brief accel any motion interrupt configuration
 */
typedef struct _IMUAccAnyMotInterruptCfg {
    /** 1 any-motion enable, 0 - any-motion disable */
    uint8_t anymotion_en : 1;

    /** slope interrupt x, 1 - enable, 0 - disable */
    uint8_t anymotion_x : 1;

    /** slope interrupt y, 1 - enable, 0 - disable */
    uint8_t anymotion_y : 1;

    /** slope interrupt z, 1 - enable, 0 - disable */
    uint8_t anymotion_z : 1;

    /** slope duration */
    uint8_t anymotion_dur : 2;

    /** data source 0- filter & 1 pre-filter*/
    uint8_t anymotion_data_src : 1;

    /** slope threshold */
    uint8_t anymotion_thr;
} IMUAccAnyMotInterruptCfg;

/**
 * @brief accel significant motion interrupt configuration
 */
typedef struct _IMUAccSigMotInterruptCfg {
    /** skip time of sig-motion interrupt */
    uint8_t sig_mot_skip : 2;

    /** proof time of sig-motion interrupt */
    uint8_t sig_mot_proof : 2;

    /** data source 0- filter & 1 pre-filter*/
    uint8_t sig_data_src : 1;

    /** 1 - enable sig, 0 - disable sig & enable anymotion */
    uint8_t sig_en : 1;

    /** sig-motion threshold */
    uint8_t sig_mot_thres;
} IMUAccSigMotInterruptCfg;

/**
 * @brief accel step detect interrupt configuration
 */
typedef struct _IMUAccStepDetectInterruptCfg {
    /** 1- step detector enable, 0- step detector disable */
    uint16_t step_detector_en : 1;

    /** minimum threshold */
    uint16_t min_threshold : 2;

    /** minimal detectable step time */
    uint16_t steptime_min : 3;

    /** enable step counter mode setting */
    uint16_t step_detector_mode : 2;

    /** minimum step buffer size*/
    uint16_t step_min_buf : 3;
} IMUAccStepDetectInterruptCfg;

/**
 * @brief accel no motion interrupt configuration
 */
typedef struct _IMUAccNoMotionInterruptCfg {
    /** no motion interrupt x */
    uint16_t no_motion_x : 1;

    /** no motion interrupt y */
    uint16_t no_motion_y : 1;

    /** no motion interrupt z */
    uint16_t no_motion_z : 1;

    /** no motion duration */
    uint16_t no_motion_dur : 6;

    /** no motion sel , 1 - enable no-motion ,0- enable slow-motion */
    uint16_t no_motion_sel : 1;

    /** data source 0- filter & 1 pre-filter*/
    uint16_t no_motion_src : 1;

    /** no motion threshold */
    uint8_t no_motion_thres;
} IMUAccNoMotionInterruptCfg;

/**
 * @brief accel orient interrupt configuration
 */
typedef struct _IMUAccOrientInterruptCfg {
    /** thresholds for switching between the different orientations */
    uint16_t orient_mode : 2;

    /** blocking_mode */
    uint16_t orient_blocking : 2;

    /** Orientation interrupt hysteresis */
    uint16_t orient_hyst : 4;

    /** Orientation interrupt theta */
    uint16_t orient_theta : 6;

    /** Enable/disable Orientation interrupt */
    uint16_t orient_ud_en : 1;

    /** exchange x- and z-axis in algorithm ,0 - z, 1 - x */
    uint16_t axes_ex : 1;

    /** 1 - orient enable, 0 - orient disable */
    uint8_t orient_en : 1;
} IMUAccOrientInterruptCfg;

/**
 * @brief accel flat detect interrupt configuration
 */
typedef struct _IMUAccFlatDetectInterruptCfg {
    /** flat threshold */
    uint16_t flat_theta : 6;

    /** flat interrupt hysteresis */
    uint16_t flat_hy : 3;

    /** delay time for which the flat value must remain stable for the
     * flat interrupt to be generated */
    uint16_t flat_hold_time : 2;

    /** 1 - flat enable, 0 - flat disable */
    uint16_t flat_en : 1;
} IMUAccFlatDetectInterruptCfg;

/**
 * @brief accel log-g interrupt configuration
 */
typedef struct _IMUAccLowGInterruptCfg {
    /** low-g interrupt trigger delay */
    uint8_t low_dur;

    /** low-g interrupt trigger threshold */
    uint8_t low_thres;

    /** hysteresis of low-g interrupt */
    uint8_t low_hyst : 2;

    /** 0 - single-axis mode ,1 - axis-summing mode */
    uint8_t low_mode : 1;

    /** data source 0- filter & 1 pre-filter */
    uint8_t low_data_src : 1;

    /** 1 - enable low-g, 0 - disable low-g */
    uint8_t low_en : 1;
} IMUAccLowGInterruptCfg;

/**
 * @brief accel high-g interrupt configuration
 */
typedef struct _IMUAccHighGInterruptCfg {
    /** High-g interrupt x, 1 - enable, 0 - disable */
    uint8_t high_g_x : 1;

    /** High-g interrupt y, 1 - enable, 0 - disable */
    uint8_t high_g_y : 1;

    /** High-g interrupt z, 1 - enable, 0 - disable */
    uint8_t high_g_z : 1;

    /** High-g hysteresis  */
    uint8_t high_hy : 2;

    /** data source 0- filter & 1 pre-filter */
    uint8_t high_data_src : 1;

    /** High-g threshold */
    uint8_t high_thres;

    /** High-g duration */
    uint8_t high_dur;
} IMUAccHighGInterruptCfg;

union interrupt_type_cfg {
    /** Tap interrupt structure */
    IMUAccTapInterruptCfg acc_tap_irq;

    /** Slope interrupt structure */
    IMUAccAnyMotInterruptCfg acc_any_motion_irq;

    /** Significant motion interrupt structure */
    IMUAccSigMotInterruptCfg acc_sig_motion_irq;

    /** Step detector interrupt structure */
    IMUAccStepDetectInterruptCfg acc_step_detect_irq;

    /** No motion interrupt structure */
    IMUAccNoMotionInterruptCfg acc_no_motion_irq;

    /** Orientation interrupt structure */
    IMUAccOrientInterruptCfg acc_orient_irq;

    /** Flat interrupt structure */
    IMUAccFlatDetectInterruptCfg acc_flat_irq;

    /** Low-g interrupt structure */
    IMUAccLowGInterruptCfg acc_low_g_irq;

    /** High-g interrupt structure */
    IMUAccHighGInterruptCfg acc_high_g_irq;
};

/**
 * @brief interrupt configuration
 */
typedef struct _IMUInterruptSetting {
    uint8_t irq_channel; /**< Interrupt channel */
    uint8_t irq_type; /**< Select Interrupt */
    /** Structure configuring Interrupt pins */
    IMUInterruptPinSetting irq_pin_settg;
    /** Union configures required interrupt */
    union interrupt_type_cfg irq_type_cfg;

    /** FIFO FULL INT 1-enable, 0-disable */
    uint8_t fifo_full_irq_en : 1;

    /** FIFO WTM INT 1-enable, 0-disable */
    uint8_t fifo_wtm_irq_en : 1;
} IMUInterruptSetting;

/**
 * @brief Structure of IMU sensor mode
 */
typedef struct _ImuPmuStatus {
    uint8_t accel_pmu_status; /**< accel sensor mode */
    uint8_t gyro_pmu_status; /**< gyro sensor mode */
    uint8_t aux_pmu_status; /**< aux sensor mode */
} ImuPmuStatus;

/**
 * @brief Structure of IMU sensor configuration
 */
typedef struct _ImuSensorConfig {
    uint8_t power; /**< power mode */
    uint8_t odr; /**< output data rate */
    uint8_t range; /**< range */
    uint8_t bw; /**< bandwidth */
} ImuSensorConfig;

/**
 * @brief Structure of IMU sensor data
 */
typedef struct _ImuSensorData {
    /** X-axis sensor data */
    int16_t x;

    /** Y-axis sensor data */
    int16_t y;

    /** Z-axis sensor data */
    int16_t z;

    /** sensor time */
    uint32_t sensortime;
} __attribute((__packed__)) ImuSensorData;

/**
 * @brief aux data structure which comprises of 8 bytes of accel data
 */
typedef struct _ImuAuxData {
    /** Auxiliary data */
    uint8_t data[8];
} ImuAuxData;

/**
 * @brief accel motion state
 */
enum accel_motion_state {
    ANY_SIG_DISABLE = -1, /**< disable anymotion and signifition motion */
    ANY_ENABLE      = 0, /**< enable anymotion */
    SIG_ENABLE      = 1, /**< enable signifition motion */
};

/**
 * @brief imu configuration
 */
typedef struct _ImuConfig {
    const GpioPort *power_pin; /**< power pin */
    const GpioPort *data_pin; /**< data pin */
    const GpioPort *wake_pin; /**< wake pin */
} ImuConfig;

/**
 * @brief imu FIFO configuration
 */
typedef struct _ImuFifoConfig {
    /** Data buffer of user defined length is to be mapped here */
    uint8_t *data;

    /** While calling the API  "get_fifo_data" , length stores
     *  number of bytes in FIFO to be read (specified by user as input)
     *  and after execution of the API ,number of FIFO data bytes
     *  available is provided as an output to user
     */
    uint16_t length;

    /** FIFO time enable */
    uint8_t fifo_time_enable;

    /** Enabling of the FIFO header to stream in header mode */
    uint8_t fifo_header_enable;

    /** Streaming of the Accelerometer, Gyroscope
     * sensor data or both in FIFO */
    uint8_t fifo_data_enable;

    /** Will be equal to length when no more frames are there to parse */
    uint16_t accel_byte_start_idx;

    /** Will be equal to length when no more frames are there to parse */
    uint16_t gyro_byte_start_idx;

    /** Will be equal to length when no more frames are there to parse */
    uint16_t aux_byte_start_idx;

    /** Value of FIFO sensor time time */
    uint32_t sensor_time;

    /** Value of Skipped frame counts */
    uint8_t skipped_frame_count;
} ImuFifoConfig;

/**
 * @brief Aux sensor configuration structure
 */
typedef struct _ImuAuxCfg {
    /** Aux sensor, 1 - enable 0 - disable */
    uint8_t aux_sensor_enable : 1;

    /** Aux manual/auto mode status */
    uint8_t manual_enable : 1;

    /** Aux read burst length */
    uint8_t aux_rd_burst_len : 2;

    /** output data rate */
    uint8_t aux_odr : 4;

    /** i2c addr of auxiliary sensor */
    uint8_t aux_i2c_addr;

    uint8_t power; /**< power mode */
} ImuAuxCfg;

/**
 * @brief FOC configuration structure
 */
typedef struct _ImuFocCfg {
    uint8_t foc_gyr_en;
    uint8_t foc_acc_x;
    uint8_t foc_acc_y;
    uint8_t foc_acc_z;
    uint8_t acc_off_en;
    uint8_t gyro_off_en;
} ImuFocCfg;

/**
 * @brief accel gyro offsets
 */
typedef struct _ImuOffsets {
    /** Accel offset for x axis */
    int8_t off_acc_x;

    /** Accel offset for y axis */
    int8_t off_acc_y;

    /** Accel offset for z axis */
    int8_t off_acc_z;

    /** Gyro offset for x axis */
    int16_t off_gyro_x;

    /** Gyro offset for y axis */
    int16_t off_gyro_y;

    /** Gyro offset for z axis */
    int16_t off_gyro_z;
} ImuOffsets;

typedef struct _ImuContext {
    uint8_t chip_id; /**< chip id */
    const BusDevice *bus_device; /**< bus device */
    const BusConfig *bus_config; /**< bus device */
    const GpioPort *power_pin; /**< power pin */
    const GpioPort *data_pin; /**< data pin */
    const GpioPort *wake_pin; /**< wake pin */
    uint8_t capability; /**< capability of the sensor */
    ImuFifoConfig *fifo_cfg; /**< fifo configuration */
    uint8_t accel_motion_state; /**< accel motion state */
    ImuSensorConfig accel_cfg; /**< structure to configure Accel sensor */
    ImuSensorConfig prev_accel_cfg; /**< structure to hold previous/old accel
                                       config parameters. */
    ImuSensorConfig gyro_cfg; /**< structure to configure gyro sensor */
    ImuSensorConfig prev_gyro_cfg; /**< structure to hold previous/old gyro
                                      config parameters. */
    ImuAuxCfg aux_cfg; /**< structure to configure auxiliary sensor */
    ImuAuxCfg prev_aux_cfg; /**< structure to hold previous/old aux
                                       config parameters. */
} ImuContext;

typedef struct ImuDevice {
    BusDevice bus_device; /**< bus device */
    BusConfig bus_config; /**< bus configure */
    /** flag set by sensor hal */
    uint8_t flag;
    /** capability of the sensor */
    uint8_t capability;
    /** FIFO buffer */
    uint8_t *fifo_buff;
    /** context of driver device */
    ImuContext *ctx;
    /** init device */
    int (*init)(ImuContext *ctx);
    /** deinit device */
    int (*deinit)(ImuContext *ctx);
    /** make soft reset */
    int (*soft_reset)(ImuContext *ctx);
    /** read registers from sensor */
    int (*read_reg)(ImuContext *ctx, uint8_t reg_addr, uint8_t *data,
                    uint16_t len);
    /** write register to sensor */
    int (*write_reg)(ImuContext *ctx, uint8_t reg_addr, uint8_t *data,
                     uint16_t len);
    /** read accel/gyro samples */
    int (*read_sample)(ImuContext *ctx, uint8_t select, ImuSensorData *accel,
                       ImuSensorData *gyro, uint8_t count);
    /** configures the default power mode, range and bandwidth of sensor */
    int (*set_sensor_default_cfg)(ImuContext *ctx);
    /** configures accelerometer sensor with actual val */
    int (*set_accel_cfg)(ImuContext *ctx, uint8_t range, uint8_t bwp,
                         uint16_t odr, uint8_t select_type);
    /** configures gyro sensor with actual val */
    int (*set_gyro_cfg)(ImuContext *ctx, uint16_t range, uint8_t bwp,
                        uint16_t odr, uint8_t select_type);
    /** configures the power mode, range and bandwidth of sensor */
    int (*set_sensor_cfg)(ImuContext *ctx, ImuSensorConfig *accel,
                          ImuSensorConfig *gyro, ImuAuxCfg *mag);
    /** get configuration of sensor */
    int (*get_sensor_cfg)(ImuContext *ctx, ImuSensorConfig *accel,
                          ImuSensorConfig *gyro, ImuAuxCfg *mag);
    /** get range of accel */
    int (*get_accel_range)(ImuContext *ctx, uint8_t *range);
    /** set the low power mode of the selected sensor */
    int (*set_low_power)(ImuContext *ctx, uint8_t sensor);
    /** set the normal mode of the sensor */
    int (*set_normal_mode)(ImuContext *ctx, uint8_t sensor);
    /** set the stop mode of the sensor */
    int (*set_stop_mode)(ImuContext *ctx, uint8_t sensor);
    int (*get_power_mode)(ImuContext *ctx, ImuPmuStatus *pmu_status);
    /** set FIFO watermark level */
    int (*set_fifo_wm)(ImuContext *ctx, uint8_t fifo_wm);
    /** set the down sampling ratios of the accel and gyro data for FIFO */
    int (*set_fifo_down)(ImuContext *ctx, uint8_t fifo_down);
    /** set FIFO configuration */
    int (*set_fifo_cfg)(ImuContext *ctx, uint8_t config, uint8_t enable);
    /** flush the sample buffer, discarding all unread samples */
    int (*flush_fifo)(ImuContext *ctx);
    /** read accel/gyro samples from fifo */
    int (*get_fifo_data)(ImuContext *ctx);
    /** read fifo data and packet with Accel format */
    int (*fifo_parsing_accel)(ImuContext *ctx, ImuSensorData *accel_data,
                              int8_t frame_request, uint16_t *available_frame);
    /** configure different interrupts */
    int (*set_interrupt_cfg)(ImuContext *ctx, bool enable, uint8_t irq_type,
                             IMUInterruptSetting *setting);
    /** check interrupt status */
    int (*check_interrupt_status)(ImuContext *ctx, uint8_t *check_type);
    /** set the threshold and duration of highg interrupt */
    int (*set_highg_threshold)(ImuContext *ctx,
                               IMUHighGCondition highg_condition);
    /** clear step count register value */
    int (*reset_step_counter)(ImuContext *ctx);
    /** enables/disable the step counter feature */
    int (*set_step_counter)(ImuContext *ctx, uint8_t step_cnt_enable);
    /** read the step counter value. */
    int (*get_step_counter)(ImuContext *ctx, uint16_t *step_val);
    /** enable/disable the pin's interrupt */
    int (*enable_interrupt)(ImuContext *dev, uint8_t pin, bool enable,
                            ImuDataReadyHandler data_ready_callback);
    /** enable/disable power */
    int (*enable_power)(ImuContext *dev, bool enable);
    /** perform self test of accel/gyro of the imu sensor */
    int (*selftest)(ImuContext *ctx, uint8_t sensor_select);
} ImuDevice;

/** Functions implemented at the HAL layer will follow these APIs. */

/**
 * @brief This API used to initialize imu device driver
 *
 * @param dev handle of imu device
 * @return 0 on success, error code on failure.
 */
int hal_imu_init(ImuDevice *dev);

/**
 *  @brief This API used to release IMU sensor context
 *
 *  @param dev handle of imu device
 *  @return none
 *
 */
int hal_imu_deinit(ImuDevice *dev);

/**
 *  @brief This API used for softreset the IMU sensor
 *
 *  @note User has to allocate the FIFO buffer along with
 *  corresponding fifo length from his side before calling this API
 *  as mentioned in the readme.md
 *
 *  @param dev handle of imu device
 *  @return 0 on success, error code on failure.
 *
 */
int hal_imu_soft_reset(ImuDevice *dev);

/**
 * @brief function to reads the data from the given register address of IMU
 * sensor.
 * @note  this API is for debug only
 *
 * @param dev handle of imu device
 * @param reg_addr register address from where the data to be read
 * @param data pointer to data buffer to store the read data
 * @param len no of bytes of data to be read.
 * @return 0 on success, error code on failure.
 */
int hal_imu_read_reg(ImuDevice *dev, uint8_t reg_addr, uint8_t *data,
                     uint16_t len);

/**
 * @brief function to writes the given data to the register address
 * of sensor.
 * @note  this API is for debug only
 *
 * @param dev handle of imu device
 * @param reg_addr register address from where the data to be written.
 * @param data pointer to data buffer which is to be written in the sensor.
 * @param len no of bytes of data to write..
 * @return 0 on success, error code on failure.
 */
int hal_imu_write_reg(ImuDevice *dev, uint8_t reg_addr, uint8_t *data,
                      uint16_t len);

/**
 * @brief This API reads sensor data, stores it in the ImuSensorData
 * structure pointer passed by the user. The user can ask for accel data
 * ,gyro data or both sensor data using ImuSensorType enum
 *
 * @param dev handle of imu device
 * @param select_sensor enum to choose accel,gyro or both sensor data,
 * including IMU_ACCEL, IMU_GYRO, IMU_ACCEL_GYRO
 * @param accel Structure pointer to store accel data
 * @param gyro Structure pointer to store gyro data
 * @param count
 * @return 0 on success, error code on failure.
 */
int hal_imu_read_samples(ImuDevice *dev, uint8_t select_sensor,
                         ImuSensorData *accel, ImuSensorData *gyro, int count);

/**
 * @brief this API configures the default power mode, range and bandwidth of
 * sensor.
 *
 * @param dev handle of imu device
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_sensor_default_cfg(ImuDevice *dev);

/**
 * @brief This API configures the range, bandwidth and output data rate
 * of accelerometer sensor with actual val
 *
 * @param dev handle of imu device
 * @param range actual accelerometer range
 * @param bwp actual accelerometer bandwidth parameters
 * @param odr actual accelerometer output data rate
 * @param select_type enum to choose configuration parameter options,
 * including IMU_SENSOR_RANGE, IMU_SENSOR_ODR, IMU_SENSOR_BWP
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_accel_cfg(ImuDevice *dev, uint8_t range, uint8_t bwp,
                          uint16_t odr, uint8_t select_type);

/**
 * @brief This API configures the range, bandwidth and output data rate
 * of gyro sensor with actual val
 *
 * @param dev handle of imu device
 * @param range actual gyro range
 * @param bwp actual gyro oversampling rate
 * @param odr actual gyro output data rate
 * @param select_type enum to choose configuration parameter options
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_gyro_cfg(ImuDevice *dev, uint16_t range, uint8_t bwp,
                         uint16_t odr, uint8_t select_type);

/**
 * @brief this API configures the power mode, range and bandwidth of sensor.
 *
 * @param dev handle of imu device
 * @param accel_cfg accel sensor configuration
 * @param gyro_cfg gyro sensor configuration
 * @param aux_cfg magnet sensor configuration
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_sensor_cfg(ImuDevice *dev, ImuSensorConfig *accel_cfg,
                           ImuSensorConfig *gyro_cfg, ImuAuxCfg *aux_cfg);

/**
 * @brief This API get the current power mode, range and bandwidth
 * configurations of sensor.
 *
 * @param dev handle of imu device
 * @param accel_cfg accel sensor configuration
 * @param gyro_cfg gyro sensor configuration
 * @param aux_cfg magnet sensor configuration
 * @return Result of API execution status
 * @retval 0 on success, error code on failure.
 */
int hal_imu_get_sens_cfg(ImuDevice *dev, ImuSensorConfig *accel_cfg,
                         ImuSensorConfig *gyro_cfg, ImuAuxCfg *aux_cfg);

/**
 * @brief This API get the range of accel.
 *
 * @param dev handle of imu device
 * @param range range val of accel
 * @return 0 on success, error code on failure.
 */
int hal_imu_get_accel_range(ImuDevice *dev, uint8_t *range);

/**
 * @brief This API sets the low power mode of the selected sensor.
 *
 * @param dev handle of imu device
 * @param sensor enum to select sensor, including IMU_ACCEL, IMU_GYRO,
 * IMU_ACCEL_GYRO, IMU_MAG, IMU_ALL. @see ImuSensorType
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_low_power(ImuDevice *dev, uint8_t sensor);

/**
 * @brief This API sets the normal mode of the sensor.
 *
 * @param dev handle of imu device
 * @param sensor enum to select sensor, including IMU_ACCEL, IMU_GYRO,
 * IMU_ACCEL_GYRO, IMU_MAG, IMU_ALL. @see ImuSensorType
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_normal(ImuDevice *dev, uint8_t sensor);

/**
 * @brief This API sets the stop mode of the sensor.
 *
 * @param dev handle of imu device
 * @param sensor enum to select sensor, including IMU_ACCEL, IMU_GYRO,
 * IMU_ACCEL_GYRO, IMU_MAG, IMU_ALL. @see ImuSensorType
 * @return 0 on success, error code on failure.
 */
int hal_imu_set_stop(ImuDevice *dev, uint8_t sensor);

/**
 * @brief Function to get the power mode of the sensor.
 *
 * @param dev  handle of imu device
 * @param pmu_status Power mode of the sensor
 *
 * @return 0 on success, error code on failure.
 */
int hal_imu_get_power_mode(ImuDevice *dev, ImuPmuStatus *pmu_status);

/**
 *  @brief This API sets the FIFO watermark level in the sensor.
 *
 *  @note The FIFO watermark is issued when the FIFO fill level is
 *  equal or above the watermark level and units of watermark is 4 bytes.
 *
 *  @param dev handle of imu device
 *  @param fifo_wm  Variable used to set the FIFO water mark level
 *
 *  @return 0 on success, error code on failure.
 *
 */
int hal_imu_set_fifo_wm(ImuDevice *dev, uint8_t fifo_wm);

/**
 *  @brief This API is used to configure the down sampling ratios of
 *  the accel and gyro data for FIFO.Also, it configures filtered or
 *  pre-filtered data for accel and gyro.
 *
 *  @param dev handle of imu device
 *  @param fifo_down the down sampling ratios of the accel and gyro data for
 * FIFO
 *
 *  @return 0 on success, error code on failure.
 *
 */
int hal_imu_set_fifo_down(ImuDevice *dev, uint8_t fifo_down);

/** @brief This API sets the FIFO configuration in the sensor.
 *
 *  @param dev handle of imu device
 *  @param config variable used to specify the FIFO
 *  configurations which are to be enabled or disabled in the sensor.
 *  @param enable Parameter used to enable or disable the above
 *  FIFO configuration
 *
 *  @return 0 on success, error code on failure.
 *
 */
int hal_imu_set_fifo_cfg(ImuDevice *dev, uint8_t config, uint8_t enable);

/**
 *  @brief This API writes fifo_flush command to command register.This
 *  action clears all data in the Fifo without changing fifo configuration
 *  settings.
 *
 *  @param dev handle of imu device
 *
 *  @return 0 on success, error code on failure.
 *
 */
int hal_imu_flush_fifo(ImuDevice *dev);

/**
 *  @brief This API reads data from the fifo buffer.
 *
 *  @note User has to allocate the FIFO buffer along with
 *  corresponding fifo length from his side before calling this API
 *  as mentioned in the readme.md
 *
 *  @note User must specify the number of bytes to read from the FIFO in
 *  dev->ctx->fifo_cfg->length , It will be updated by the number of
 *  bytes actually read from FIFO after calling this API
 *
 *  @param dev handle of imu device
 *
 *  @return 0 on success, error code on failure.
 *
 */
int hal_imu_get_fifo_data(ImuDevice *dev);

/**
 *  @brief This API package data from the fifo buffer to accel format.
 *
 *  @note User has to allocate the FIFO buffer along with
 *  corresponding fifo length from his side before calling this API
 *  as mentioned in the readme.md
 *
 *  @param dev handle of imu device
 *  @param accel_data accel buffer to save
 *  @param frame_request request frame number
 *  @param available_frame available accel frame number
 *
 *  @return 0 on success, error code on failure
 *
 */
int hal_imu_fifo_parsing_accel(ImuDevice *dev, ImuSensorData *accel_data,
                               int8_t frame_request, uint16_t *available_frame);

/**
 *  @brief This API used to set IMU sensor interrupt
 *
 *  @param dev handle of imu device
 *  @param enable  Sensor interrupt enable/disable
 *  @param irq_type Sensor interrupt type
 *  @param irq_settings interrupt configuration
 *
 *  @return 0 on success, error code on failure
 *
 */
int hal_imu_set_interrupt_cfg(ImuDevice *dev, bool enable, uint8_t irq_type,
                              IMUInterruptSetting *irq_settings);

/**
 * @brief This API checks if the interrupt is triggered
 *
 * @param dev handle of imu device
 * @param check_type enum to select interrupt, including
 * IMU_ACC_ANY_MOTION_INTERRUPT, IMU_ACC_DOUBLE_TAP_INTERRUPT,
 * IMU_ACC_SINGLE_TAP_INTERRUPT, IMU_ACC_HIGH_G_INTERRUPT
 * @return  0 on success, error code on failure
 */
int hal_imu_check_interrupt_status(ImuDevice *dev, uint8_t *check_type);

/**
 * @brief This API set the threshold and duration of highg interrupt
 *
 * @param dev handle of imu device
 * @param highg_condition struct of high interrupt condition
 * @return 0 on success, error code on failure
 */
int hal_imu_set_highg_threshold(ImuDevice *dev,
                                IMUHighGCondition highg_condition);

/**
 *  @brief This API writes step count reset command to command register.
 *  This action clears step count register value
 */
int hal_imu_reset_step_counter(ImuDevice *dev);

/**
 * @brief This API enables or disable the step counter feature.
 * 1 - enable step counter (0 - disable)
 */
int hal_imu_set_step_counter(ImuDevice *dev, uint8_t step_cnt_enable);

/**
 * @brief This API reads the step counter value.
 */
int hal_imu_get_step_counter(ImuDevice *dev, uint16_t *step_val);

/**
 * @brief This API use to enable/disable the pin's interrupt
 *
 * @param dev handle of imu device
 * @param pin selected pin, including IMU_DATA_PIN, IMU_WAKE_PIN,
 * IMU_POWER_PIN, @see ImuPinType
 * @param enable true to enable interrupt, false to disable interrupt for
 * specific pin
 * @param data_ready_callback call-back function when pin interrupt is triggered
 * @return 0 on success, error code on failure
 */
int hal_imu_enable_interrupt(ImuDevice *dev, uint8_t pin, bool enable,
                             ImuDataReadyHandler data_ready_callback);

/**
 * @brief This API use to enable/disable power
 *
 * @param dev handle of imu device
 * @param enable check if enable power
 * @return 0 on success, error code on failure
 */
int hal_imu_enable_power(ImuDevice *dev, bool enable);

/**
 * @brief This is used to perform self test of accel/gyro of the imu sensor
 *
 * @param dev handle of imu device
 * @param[in] select_sensor  : enum to choose accel or gyro for self test
 *
 * @note self test can be performed either for accel/gyro at any instant.
 *
 *     value of select_sensor       |  Inference
 *----------------------------------|--------------------------------
 *   IMU_ACCEL                      | Accel self test enabled
 *   IMU_GYRO                       | Gyro self test enabled
 *   IMU_ACCEL_GYRO                 | NOT TO BE USED
 *
 * @note The return value of this API gives us the result of self test.
 *
 * @note Performing self test does soft reset of the sensor, User can
 * set the desired settings after performing the self test.
 *
 * @return 0 on success, error code on failure
 */
int hal_imu_perform_self_test(ImuDevice *dev, uint8_t select_sensor);
/** @} */

#endif
