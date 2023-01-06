/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_screen.h
 * @brief Public head file of HAL for screen
 */

#pragma once
#include "hal_gpio.h"

/** @addtogroup SCREEN
 *  Screen HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

typedef enum {
    grey_screen   = 0,
    colour_screen = 1,
} ScreenType;

typedef struct _ScreenDevice {
    /** data */
    uint16_t screen_length;
    uint16_t screen_width;
    uint16_t page_size;
    ScreenType screen_type;

    /** power on and init screen */
    int (*init)(void);
    /** send command to screen */
    int (*send_command)(uint8_t *command, uint16_t size);
    /** send uint8_t data to screen */
    int (*send_data8)(uint8_t data);
    /** send uint16_t data to screen */
    int (*send_data16)(uint16_t data);
    int (*send_data)(uint8_t *data, uint16_t size);
    /** set start&end address and be ready for memory write */
    void (*address_set)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    /** set page address */
    void (*page_set)(unsigned char page);
    /** set column address */
    void (*column_set)(unsigned char page);
} ScreenDevice;

typedef struct _OledBusConfig {
    uint8_t bus_type;
    uint8_t bus_id;
} OledBusConfig;

typedef struct _ScreenConfig {
    OledBusConfig bus_config;
    const GpioPort *rst_pin;
    const GpioPort *dc_pin;
} ScreenConfig;

/**
 * @brief init screen and use default settings
 * @param screen_device ref to screen device
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_init(ScreenDevice *screen_device);

/**
 * @brief send command array to screen device
 * @param screen_device ref to screen device
 * @param command ref to command array to be sent
 * @param size size of command array
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_send_command(ScreenDevice *screen_device, uint8_t *command,
                            uint16_t size);

/**
 * @brief send uint8_t data to screen device
 * @param screen_device ref to screen device
 * @param data uint8_t data to be sent
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_send_data8(ScreenDevice *screen_device, uint8_t data);

/**
 * @brief send uint16_t data to screen device
 * @param screen_device ref to screen device
 * @param data uint16_t data to be sent
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_send_data16(ScreenDevice *screen_device, uint8_t data);

/**
 * @brief send data array to screen device
 * @param screen_device ref to screen device
 * @param data ref to data array to be sent
 * @param size size of data array
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_send_data(ScreenDevice *screen_device, uint8_t *data,
                         uint16_t size);

/**
 * @brief set page number of memory position to screen device
 * @param screen_device ref to screen device
 * @param page_num page number to be set
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_set_page(ScreenDevice *screen_device, uint8_t page_num);

/**
 * @brief set page number of memory position to screen device with fixed point
 * @param screen_device ref to screen device
 * @param x x-axis of point
 * @param y y-axis of point
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_set_page_with_point(ScreenDevice *screen_device, uint8_t x,
                                   uint8_t y);

/**
 * @brief set column number of memory position to screen device
 * @param screen_device ref to screen device
 * @param column_num column number to be set
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_set_column(ScreenDevice *screen_device, uint8_t column_num);

/**
 * @brief set start and end position to screen device with 2 fixed points
 * @param screen_device ref to screen device
 * @param xstart x-axis of start point
 * @param ystart y-axis of start point
 * @param xend x-axis of end point
 * @param yend y-axis of end point
 *   @retval 0 for succeed, others for failure
 */
int hal_screen_set_address(ScreenDevice *screen_device, uint16_t xstart,
                           uint16_t ystart, uint16_t xend, uint16_t yend);
/** @} */
