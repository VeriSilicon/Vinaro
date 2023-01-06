/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_screen.c
 * @brief HAL interface for screen driver
 */
#include "hal_screen.h"
#include "vsd_error.h"
#include "vsd_common.h"

int hal_screen_init(ScreenDevice *screen_device)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (screen_device->init) {
        ret = screen_device->init();
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }
    return ret;
}

int hal_screen_send_command(ScreenDevice *screen_device, uint8_t *command,
                            uint16_t size)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (screen_device->send_command) {
        ret = screen_device->send_command(command, size);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }
    return ret;
}

int hal_screen_send_data8(ScreenDevice *screen_device, uint8_t data)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (screen_device->send_data8) {
        ret = screen_device->send_data8(data);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }
    return ret;
}

int hal_screen_send_data16(ScreenDevice *screen_device, uint8_t data)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (screen_device->send_data16) {
        ret = screen_device->send_data16(data);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }
    return ret;
}

int hal_screen_send_data(ScreenDevice *screen_device, uint8_t *data,
                         uint16_t size)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (screen_device->send_data) {
        ret = screen_device->send_data(data, size);
    } else {
        ret = VSD_ERR_INVALID_PARAM;
    }
    return ret;
}

int hal_screen_set_page_with_point(ScreenDevice *screen_device, uint8_t x,
                                   uint8_t y)
{
    int ret = VSD_SUCCESS;
    uint16_t page_num;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!screen_device->page_set) {
        return VSD_ERR_INVALID_POINTER;
    }
    page_num = y / (screen_device->page_size);
    if (page_num < (screen_device->screen_length / screen_device->page_size)) {
        screen_device->page_set(page_num);
        ret = VSD_SUCCESS;
    } else {
        ret = VSD_ERR_INVALID_STATE;
    }
    return ret;
}

int hal_screen_set_page(ScreenDevice *screen_device, uint8_t page_num)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!screen_device->page_set) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (page_num < (screen_device->screen_length)) {
        screen_device->page_set(page_num);
        ret = VSD_SUCCESS;
    } else {
        ret = VSD_ERR_INVALID_STATE;
    }
    return ret;
}

int hal_screen_set_column(ScreenDevice *screen_device, uint8_t column_num)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!screen_device->column_set) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (column_num < (screen_device->screen_width)) {
        screen_device->column_set(column_num);
        ret = VSD_SUCCESS;
    } else {
        ret = VSD_ERR_INVALID_STATE;
    }
    return ret;
}

int hal_screen_set_address(ScreenDevice *screen_device, uint16_t xstart,
                           uint16_t ystart, uint16_t xend, uint16_t yend)
{
    int ret = VSD_SUCCESS;
    if (!screen_device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!screen_device->page_set) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (xstart < xend && xend < screen_device->screen_width && ystart < yend &&
        yend < screen_device->screen_length) {
        screen_device->address_set(xstart, ystart, xend, yend);
        ret = VSD_SUCCESS;
    } else {
        ret = VSD_ERR_INVALID_STATE;
    }
    return ret;
}
