/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_sensor_common.h
 * @brief Public head file of HAL for sensor common
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup SENSOR
 *  Sensor definition and common APIs
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define SENSOR_NAME_SIZE 16

/** hal sensor types */
enum e_sensor_type {
    SENSOR_TYPE_NONE = 0, /**< sensor type of hal is none */
    SENSOR_TYPE_PPG  = 1, /**< sensor type of hal is ECG */
    SENSOR_TYPE_ECG  = 2, /**< sensor type of hal is PPG */
    SENSOR_TYPE_TMP  = 3, /**< sensor type of hal is Temperature */
    SENSOR_TYPE_IMU  = 4, /**< sensor type of hal is IMU */
};

/**
 * @brief sensor sample format
 *
 */
enum e_ppg_sample_format {
    /** sample format: sample size is 2 bytes, and little endian */
    SENSOR_SAMPLE_2_BYTE_LITTLEENDIAN = 0,
    /** sample format: sample size is 4 bytes, and little endian */
    SENSOR_SAMPLE_4_BYTE_LITTLEENDIAN = 0x1,
    /** sample format: sample size is 3 bytes, and little endian */
    SENSOR_SAMPLE_3_BYTE_LITTLEENDIAN = 0x2,
    /** sample format: sample size is 2 bytes, and big endian */
    SENSOR_SAMPLE_2_BYTE_BIGENDIAN = 0x3,
    /** sample format: sample size is 4 bytes, and big endian */
    SENSOR_SAMPLE_4_BYTE_BIGENDIAN = 0x4,
    /** sample format: sample size is 3 bytes, and big endian */
    SENSOR_SAMPLE_3_BYTE_BIGENDIAN = 0x5,
};

/** sensor dataformat read by read_sample_back function */
typedef struct _SensorReadBackData {
    uint8_t *raw_sample; /**< raw sample buffer */
    uint16_t raw_size; /**< raw buffer memory size */
    uint16_t raw_count; /**< actual sample count read and stored in buffer */
    int sample_status; /**< ppg sample or ecg sample or both, @see
                          e_active_sensor_type*/
} SensorReadBackData;

/** hal sensor dataready function pointer */
typedef void (*SensorDataReadyHandler)(void);

/** hal sensor readback function pointer */
typedef int (*SensorReadBackHandler)(SensorReadBackData *raw_data, uint8_t *ecg,
                                     uint16_t ecg_size, uint16_t *re_count);

/** @} */
