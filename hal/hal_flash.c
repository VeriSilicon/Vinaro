/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_flash.c
 * @brief HAL for FLASH
 * @author Ma.Shaohui <Ma.Shaohui@verisilicon.com>
 */

#include <stdint.h>
#include <string.h>
#include "vsd_error.h"
#include "vsd_common.h"
#include "hal_flash.h"
#include "vs_spiflash.h"
#include "uart_printf.h"
#include "bsp_sysctl.h"

static FlashDevice *g_flash_dev[FLASH_DEVICE_MAX] = { NULL };

static inline FlashOperations *get_ops(const FlashDevice *device)
{
    return (FlashOperations *)device->ops;
}

int hal_flash_add_dev(FlashDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_flash_dev) / sizeof(g_flash_dev[0]); i++) {
        if (g_flash_dev[i] == NULL) {
            g_flash_dev[i] = device;
            ret            = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_flash_remove_dev(FlashDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_flash_dev) / sizeof(g_flash_dev[0]); i++) {
        if (g_flash_dev[i] == device) {
            g_flash_dev[i] = NULL;
            ret            = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

const FlashDevice *hal_flash_get_device(uint8_t dev_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_flash_dev) / sizeof(g_flash_dev[0]); i++) {
        if (g_flash_dev[i]->dev_id == dev_id) {
            return g_flash_dev[i];
        }
    }
    return NULL;
}

int hal_flash_info_get(const FlashDevice *device, PartitionId in_partition,
                       FlashPartition **partition)
{
    uint32_t i;

    if (in_partition > PARTITION_ID_MAX)
        return VSD_ERR_INVALID_PARAM;
    if (!device->partitions)
        return VSD_ERR_UNSUPPORTED;

    *partition = NULL;
    for (i = 0; i < device->partition_num; i++) {
        if (device->partitions[i].id == in_partition) {
            *partition = (FlashPartition *)&device->partitions[i];
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_NON_EXIST;
}

int hal_flash_init(const FlashDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->init)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->init(device);
}

int hal_flash_init_partition(FlashDevice *device,
                             const FlashPartition *partitions,
                             uint32_t partition_num)
{
    if (!device || !partitions)
        return VSD_ERR_INVALID_POINTER;
    if (partition_num == 0)
        return VSD_ERR_INVALID_PARAM;

    device->partitions    = partitions;
    device->partition_num = partition_num;
    return VSD_SUCCESS;
}

int hal_flash_write(const FlashDevice *device, PartitionId pno, uint32_t offset,
                    const void *buf, uint32_t buf_size)
{
    uint32_t start_addr;
    FlashPartition *partition_info = NULL;
    int ret                        = 0;
    int remain                     = buf_size;
    uint8_t *src_addr              = (uint8_t *)buf;

    ret = hal_flash_info_get(device, pno, &partition_info);
    if (ret != VSD_SUCCESS)
        return ret;

    if ((partition_info->options & PAR_OPT_WRITE_EN) == 0) {
        // the partition is read only
        return VSD_ERR_UNSUPPORTED;
    }
    start_addr = partition_info->start_addr + offset;
    if (!buf || !partition_info ||
        (offset + buf_size > partition_info->length)) {
        return VSD_ERR_INVALID_PARAM;
    }

    if (!get_ops(device)->program)
        return VSD_ERR_INVALID_POINTER;

    do {
        uint32_t fl_addr = ROUND_DOWN(start_addr, FLASH_PAGE_SIZE);
        int fl_offset    = start_addr - fl_addr;
        int len          = MIN(FLASH_PAGE_SIZE - fl_offset, buf_size);
        if (remain < FLASH_PAGE_SIZE) {
            ret = get_ops(device)->program(device, fl_addr, remain, src_addr);
        } else {
            ret = get_ops(device)->program(device, fl_addr, FLASH_PAGE_SIZE,
                                           src_addr);
        }
        if (ret != 0) {
            return ret;
        } else {
            start_addr += len;
            src_addr += len;
            remain -= len;
        }
    } while (remain > 0);
    return VSD_SUCCESS;
}

int hal_flash_write_page(const FlashDevice *device, uint32_t addr,
                         uint32_t size, const void *in_buf)
{
    if (!get_ops(device)->program)
        return VSD_ERR_INVALID_POINTER;
    return get_ops(device)->program(device, addr, size, in_buf);
}

int hal_flash_read(const FlashDevice *device, PartitionId pno, uint32_t offset,
                   void *buf, uint32_t buf_size)
{
    uint32_t start_addr;
    FlashPartition *partition_info = NULL;
    int ret                        = 0;
    int remaining                  = buf_size;
    uint8_t *src_addr              = (uint8_t *)buf;

    ret = hal_flash_info_get(device, pno, &partition_info);
    if (ret != VSD_SUCCESS)
        return ret;

    if (!buf || !partition_info ||
        (offset + buf_size > partition_info->length)) {
        return VSD_ERR_INVALID_PARAM;
    }

    start_addr = partition_info->start_addr + offset;
    if (!get_ops(device)->read)
        return VSD_ERR_INVALID_POINTER;
    do {
        uint32_t fl_addr = ROUND_DOWN(start_addr, FLASH_PAGE_SIZE);
        int fl_offset    = start_addr - fl_addr;
        int len          = MIN(FLASH_PAGE_SIZE - fl_offset, buf_size);

        if (remaining < FLASH_PAGE_SIZE) {
            ret = get_ops(device)->read(device, fl_addr, remaining, src_addr);
        } else {
            ret = get_ops(device)->read(device, fl_addr, FLASH_PAGE_SIZE,
                                        src_addr);
        }
        if (ret != 0) {
            return ret;
        } else {
            start_addr += len;
            src_addr += len;
            remaining -= len;
        }
    } while (remaining > 0);
    return VSD_SUCCESS;
}

int hal_flash_read_page(const FlashDevice *device, uint32_t addr, uint32_t size,
                        void *out_buf)
{
    if (!get_ops(device)->read)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(device)->read(device, addr, size, out_buf);
}

int hal_flash_erase(const FlashDevice *device, PartitionId pno, uint32_t offset,
                    uint32_t size)
{
    uint32_t start_addr;
    FlashPartition *partition_info = NULL;
    int ret                        = 0;
    int remaining                  = size;
    int len                        = FLASH_SECTOR_SIZE;

    ret = hal_flash_info_get(device, pno, &partition_info);
    if (ret != VSD_SUCCESS)
        return ret;

    if ((partition_info->options & PAR_OPT_WRITE_EN) == 0) {
        // the partition is read only
        return VSD_ERR_UNSUPPORTED;
    }
    if (!partition_info || (size + offset > partition_info->length)) {
        return VSD_ERR_INVALID_PARAM;
    }

    start_addr = (partition_info->start_addr + offset);
    if (!get_ops(device)->erase)
        return VSD_ERR_INVALID_POINTER;
    do {
        uint32_t fl_addr = ROUND_DOWN(start_addr, FLASH_SECTOR_SIZE);
        ret = get_ops(device)->erase(device, fl_addr, FLASH_SECTOR_SIZE);
        if (ret != VSD_SUCCESS) {
            return ret;
        } else {
            start_addr += len;
            remaining -= len;
        }
    } while (remaining > 0);
    return VSD_SUCCESS;
}

int hal_flash_erase_sector(const FlashDevice *device, uint32_t addr)
{
    if (!get_ops(device)->erase)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(device)->erase(device, addr, FLASH_SECTOR_SIZE);
}

int hal_flash_power_mode(const FlashDevice *device, FlashPowerMode mode)
{
    if (!get_ops(device)->power_mode)
        return VSD_ERR_INVALID_POINTER;

    return get_ops(device)->power_mode(device, mode);
}

int hal_flash_get_size(const FlashDevice *device, NorFlashDev *nor)
{
    if (device->hw_cfg->flash_table) {
        nor->chip = (const NorFlashInfo *)device->hw_cfg->flash_table;
        return VSD_SUCCESS;
    }
    return VSD_ERR_GENERIC;
}

int hal_flash_get_factory_info(const FlashDevice *device, NorFactoryDev *nor)
{
    NorFactoryInfo factory = { { 0x00 }, { 0x00 } };
    int ret                = 0;

    if (!get_ops(device)->get_factory_info)
        return VSD_ERR_INVALID_POINTER;
    ret = get_ops(device)->get_factory_info(device, factory.uid, factory.csid);
    if (ret != VSD_SUCCESS) {
        return ret;
    }
    nor->chip = &factory;
    return VSD_SUCCESS;
}

int hal_flash_reset(const FlashDevice *device)
{
    int ret;

    if (!get_ops(device)->reset) {
        return VSD_ERR_INVALID_POINTER;
    }
    ret = get_ops(device)->reset(device);
    return ret;
}
