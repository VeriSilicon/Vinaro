/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_spi.c
 * @brief HAL for SPI driver
 * @author Jinghua Yu<jinghua.yu@verisilicon.com>
 */

#include <stddef.h>
#include <string.h>
#include "hal_spi.h"
#include "vsd_error.h"
#include "uart_printf.h"

//#define HAL_SPI_DEBUG_ENABLE

#define HAL_SPI_DEBUG_IN 0x0001
#define HAL_SPI_DEBUG_OUT 0x0002
#define HAL_SPI_DEBUG_ERR 0x0004
#define HAL_SPI_DEBUG_ALL 0xFFFF
#ifdef HAL_SPI_DEBUG_ENABLE
unsigned int hal_spi_flags = HAL_SPI_DEBUG_ERR | HAL_SPI_DEBUG_OUT;
#define HAL_SPI_PRINT(flag, fmt, ...)                                          \
    do {                                                                       \
        if (hal_spi_flags & flag) {                                            \
            uart_printf("%s:%d:" fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
        }                                                                      \
    } while (0)
#else
#define HAL_SPI_PRINT(flag, fmt, ...)
#endif

static SpiDevice *g_spi_dev[HAL_SPI_DEV_MAX] = { NULL };

static inline SpiOperations *get_ops(const SpiDevice *device)
{
    return (SpiOperations *)device->ops;
}

int hal_spi_add_dev(SpiDevice *device)
{
    int ret = VSD_ERR_FULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_spi_dev) / sizeof(g_spi_dev[0]); i++) {
        if (g_spi_dev[i] == NULL) {
            g_spi_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_spi_remove_dev(SpiDevice *device)
{
    int ret = VSD_ERR_NON_EXIST;
    uint8_t i;

    for (i = 0; i < sizeof(g_spi_dev) / sizeof(g_spi_dev[0]); i++) {
        if (g_spi_dev[i] == device) {
            g_spi_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

SpiDevice *hal_spi_get_dev_by_id(uint8_t bus_num)
{
    SpiDevice *spi = NULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_spi_dev) / sizeof(g_spi_dev[0]); i++) {
        if (g_spi_dev[i]->bus_num == bus_num) {
            spi = g_spi_dev[i];
            break;
        }
    }
    return spi;
}

int hal_spi_fill_msg_dma(SpiMessage *spi_msg, uint8_t *tx_data,
                         uint32_t tx_size, uint8_t *rx_data, uint32_t rx_size,
                         XferCallback callback)
{
    int ret = VSD_SUCCESS;
    if (!tx_data && !rx_data) {
        HAL_SPI_PRINT(HAL_SPI_DEBUG_ERR, "the msg is NULL!");
        return VSD_ERR_INVALID_POINTER;
    }
    if (rx_size > SPI_DMA_BUFF_SIZE) {
        // TODO: unsupported now
        return VSD_ERR_UNSUPPORTED;
    }
    memset(spi_msg, 0, sizeof(SpiMessage));
    spi_msg->tx_buf         = tx_data;
    spi_msg->tx_buf_len     = tx_size;
    spi_msg->rx_buf         = rx_data;
    spi_msg->rx_buf_len     = rx_size;
    spi_msg->spec_xfer_mode = XFER_MODE_DMA;
    // the spi_msg->rx_cont will be updated for necessary after fill_msg
    spi_msg->callback = callback;
    return ret;
}

int hal_spi_fill_msg(SpiMessage *spi_msg, uint8_t *tx_data, uint32_t tx_size,
                     uint8_t *rx_data, uint32_t rx_size)
{
    int ret = VSD_SUCCESS;
    if (!tx_data && !rx_data) {
        HAL_SPI_PRINT(HAL_SPI_DEBUG_ERR, "the msg is NULL!");
        return VSD_ERR_INVALID_POINTER;
    }
    memset(spi_msg, 0, sizeof(SpiMessage));
    spi_msg->tx_buf         = tx_data;
    spi_msg->tx_buf_len     = tx_size;
    spi_msg->rx_buf         = rx_data;
    spi_msg->rx_buf_len     = rx_size;
    spi_msg->spec_xfer_mode = XFER_MODE_DEFAULT;
    spi_msg->rx_cont        = false;
    spi_msg->callback       = NULL;
    return ret;
}

int hal_spi_transfer(const SpiDevice *device, SpiMessage *spi_msg,
                     const SpiTransConfig *trans_config)
{
    int ret;

    if (!spi_msg->tx_buf && !spi_msg->rx_buf) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (get_ops(device)->transfer_one_msg) {
        ret = get_ops(device)->transfer_one_msg(device, spi_msg, trans_config);
    } else {
        HAL_SPI_PRINT(HAL_SPI_DEBUG_ERR, "transfer_one_msg is unsupported");
        ret = VSD_ERR_INVALID_POINTER;
    }
    if (ret) {
        HAL_SPI_PRINT(HAL_SPI_DEBUG_ERR, "transfer_one_msg error(ret:%d)", ret);
    }

    return ret;
}

int hal_spiflash_transfer(const SpiDevice *device, SpiMessage *msg,
                          const SpiTransConfig *config, SpiFlashTransInfo *info)
{
    int ret;

    if (device == NULL || msg == NULL || config == NULL || info == NULL) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (get_ops(device)->spi_flash_transfer) {
        ret = get_ops(device)->spi_flash_transfer(device, msg, config, info);
    } else {
        HAL_SPI_PRINT(HAL_SPI_DEBUG_ERR, "flash transfer is unsupported");
        ret = VSD_ERR_INVALID_POINTER;
    }
    if (ret) {
        HAL_SPI_PRINT(HAL_SPI_DEBUG_ERR, "flash transfer error(ret:%d)", ret);
    }

    return ret;
}

DRV_ISR_SECTION
void hal_spi_irq_handler(const SpiDevice *device)
{
    if (!device)
        return;

    if (!get_ops(device)->irq_ins_handler)
        return;

    return get_ops(device)->irq_ins_handler(device);
}
