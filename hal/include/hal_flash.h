/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_flash.h
 * @brief public head file of Flash HAL
 * @author Ma.Shaohui <Ma.Shaohui@verisilicon.com>
 */

#ifndef HAL_FLASH_H
#define HAL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup FLASH
 *  Flash hal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include "hal_spi.h"

/**< Max supported SPI flash device number */
#define FLASH_DEVICE_MAX (2)

/**
 * @enum FlashRole
 * @brief Flash role definition, the system supports multiple flash devices
 */
typedef enum {
    FLASH_MAIN = 0, /**< the major flash which stores code and data */
    FLASH_EXT, /**< the extended flash which stores data */
} FlashRole;

/* Define for options set */
#define PAR_OPT_READ_POS (0) /**< Read attribute bit in options */
#define PAR_OPT_WRITE_POS (1) /**< write attribute bit in options */

#define PAR_OPT_READ_MASK \
    (0x1u << PAR_OPT_READ_POS) /**< Read attribute mask bit */
#define PAR_OPT_WRITE_MASK \
    (0x1u << PAR_OPT_WRITE_POS) /**< Write attribute mask bit */

#define PAR_OPT_READ_EN (0x1u << PAR_OPT_READ_POS) /**< Read enable bit set */
#define PAR_OPT_WRITE_EN \
    (0x1u << PAR_OPT_WRITE_POS) /**< write enable bit set */

/* hal flash partition define */
typedef enum {
    PARTITION_ID_INVALID = -1,
    PARTITION_ID_FACTORY, /**< manufactory info & settings */
    PARTITION_ID_PLATFORM, /**< platform configuration & status */
    PARTITION_ID_BOOTLOADER, /**< bootloader */
    PARTITION_ID_FIRMWARE, /**< single firmware image */
    PARTITION_ID_IMAGE_A, /**< image A for dual image system */
    PARTITION_ID_IMAGE_B, /**< image B for dual image system */
    PARTITION_ID_BLE_SETTINGS, /**< BLE settings */
    PARTITION_ID_DATA, /**< user data */
    PARTITION_ID_DATA_EXT, /**< extended user data */
    PARTITION_ID_MAX,
} PartitionId;

/* Define for partition owner */
typedef enum {
    HAL_FLASH_EMBEDDED,
    HAL_FLASH_SPI,
    HAL_FLASH_QSPI,
    HAL_FLASH_MAX,
    HAL_FLASH_NONE,
} HalFlash;

/**
 * @brief Define for flash types
 */
typedef enum {
    FLASH_TYPE_NOR, /**< Nor flash */
    FLASH_TYPE_NAND, /**< Nand flash */
    FLASH_TYPE_MAX
} FlashType;

typedef enum {
    FLASH_PWR_DYNAMIC, /**< Dynamic control the flash power in driver */
    FLASH_PWR_SLEEP, /**< Flash keeps on deep sleep mode */
    FLASH_PWR_ON, /**< Flash keeps on normal working mode */
    FLASH_PWR_DEFAULT = FLASH_PWR_ON, /**< Default power mode */
    FLASH_PWR_MAX
} FlashPowerMode;

#define NOR_MAX_ID_LEN 2

/**
 * @brief Structure of flash hardware configuration
 */
typedef struct FlashHwConfig {
    const void *flash_table; /**< NOR flash table */
} FlashHwConfig;

/* Hal flash partition manage struct */
typedef struct FlashPartition {
    PartitionId id; /**< Partition ID, definition @see PartitionId */
    HalFlash owner; /**< The owner of this partition, @see HalFlash */
    const char *description; /**< Partition description */
    uint32_t start_addr; /**< Start address of partition */
    uint32_t length; /**< Length of partition in bytes */
    uint32_t options; /**< Read/write enable or disable */
} FlashPartition;

typedef struct FlashDevice {
    uint8_t dev_id; /**< Device ID */
    const FlashHwConfig *hw_cfg; /**< Flash hardware configuration */
    SpiDevice *qspi_dev; /**<  Instance of QSPI/SPI device */
    const void *ops; /**< Flash operations */
    const FlashPartition *partitions; /**< Pointer of flash partitions */
    uint32_t partition_num; /**< Flash partition numbers */
    void *ctx; /**< Private context */
} FlashDevice;

/**
 * @brief Structure of operations for HAL of FLASH
 */
typedef struct FlashOperations {
    int (*init)(const FlashDevice *device);
    int (*program)(const FlashDevice *device, uint32_t adr, uint32_t sz,
                   const uint8_t *buf);
    int (*read)(const FlashDevice *device, uint32_t adr, uint32_t sz,
                uint8_t *buf);
    int (*erase)(const FlashDevice *device, uint32_t adr, uint32_t size);
    int (*erase_chip)(const FlashDevice *device);
    int (*power_mode)(const FlashDevice *device, uint8_t mode);
    int (*get_factory_info)(const FlashDevice *device, uint8_t *uid,
                            uint8_t *csid);
    int (*reset)(const FlashDevice *device);
} FlashOperations;

/**
 * @brief This struct define Flash hardware
 */
typedef struct {
    const FlashHwConfig *hw_config;
    const FlashOperations *ops;
} FlashHw;

/**
 * @brief Structure of Nor flash info
 */
typedef struct NorFlashInfo {
    const char *name; /**< nor flash name */
    uint8_t id[NOR_MAX_ID_LEN]; /**< chip id, get it by reading databook */
    uint8_t id_len; /**< the len of chip id is generally equal to 3 */
    uint8_t type; /**< @see FlashType */
    uint8_t addr_width; /** address width */
    uint32_t pagesize; /**< page size in byte */
    uint32_t sectorsize; /**< sector size in byte */
    uint32_t blocksize; /**< block size in byte */
    uint32_t devicesize; /**< device size in byte */
} NorFlashInfo;

typedef struct NorFlashDev {
    const NorFlashInfo *chip;
} NorFlashDev;

typedef struct NorFactoryInfo {
    uint8_t uid[16]; /**< get norflash uid by read */
    uint8_t csid[2]; /**< get norflash csid by read */
} NorFactoryInfo;

typedef struct NorFactoryDev {
    NorFactoryInfo *chip;
} NorFactoryDev;

/**
 * @brief Add flash device instance
 * @note This API should be called by flash IP driver to add initialized
 * instance to HAL, not for application usage
 * @param device The device of flash will be added
 * @return Return VSD_SUCCESS for succeed, others for failure
 */
int hal_flash_add_dev(FlashDevice *device);

/**
 * @brief Get flash device instance
 * @param dev_id The device id of flash
 * @return Return the instance of flash device
 */
const FlashDevice *hal_flash_get_device(uint8_t dev_id);

/**
 * @brief init of flash logical partition
 * @param[in] device The instance of flash device
 * @return Return result
 * @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_init(const FlashDevice *device);

/**
 * @brief Init the partition for flash device
 * @param device The instance of flash device
 * @param partitions The pointer to flash partition table
 * @param partition_num The number of partitions
 * @return Return result
 * @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_init_partition(FlashDevice *device,
                             const FlashPartition *partitions,
                             uint32_t partition_num);

/**
 * @brief Get the information of the specified flash area
 *
 * @param[in]  device The instance of flash device
 * @param[in]  in_partition     The target flash logical partition
 * @param[out]  partition       The buffer to store partition info
 * @return Return result
 * @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_info_get(const FlashDevice *device, PartitionId in_partition,
                       FlashPartition **partition);

/**
 * @brief Erase an area on a Flash logical partition
 *
 * @note  Erase on an address will erase all data on a sector that the
 *        address is belonged to, this function does not save data that
 *        beyond the address area but in the affected sector, the data
 *        will be lost.
 *
 * @param[in]  device The instance of flash device
 * @param[in]  in_partition  The target flash logical partition which should be
 * erased
 * @param[in]  offset       Start address of the erased flash area
 * @param[in]  size          Size of the erased flash area
 * @return Return result
 *   @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_erase(const FlashDevice *device, PartitionId in_partition,
                    uint32_t offset, uint32_t size);

/**
 * @brief Erase a sector
 * @param[in] device The instance of flash device
 * @param[in] addr start address of the sector
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_flash_erase_sector(const FlashDevice *device, uint32_t addr);

/**
 * @brief Write data to an area on a flash logical partition without erase
 *
 * @param[in]  device The instance of flash device
 * @param  in_partition    The target flash logical partition which should
 * be read which should be written
 * @param  offset  Point to the start address that the data is written to
 * @param[in] in_buf point to the data buffer that will be written to flash
 * @param[in] in_buf_len The length of the buffer
 * @return Return result
 *   @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_write(const FlashDevice *device, PartitionId in_partition,
                    uint32_t offset, const void *in_buf, uint32_t in_buf_len);

/**
 * @brief Write data to a page
 * @param[in] device The instance of flash device
 * @param addr start address of the page
 * @param[in] size The size of the buffer in byte
 * @param[in] in_buf point to the data buffer which will be written to flash
 * @return Return result
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_flash_write_page(const FlashDevice *device, uint32_t addr,
                         uint32_t size, const void *in_buf);

/**
 * @brief Write data to an area on a flash logical partition with erase first
 *
 * @param[in]  device The instance of flash device
 * @param[in]  in_partition The target flash logical partition which should
 * be read which should be written
 * @param  offset Point to the start address that the data is written to
 * @param[in]  in_buf point to the data buffer that will be written to
 * flash
 * @param[in]  in_buf_len  The length of the buffer
 * @return Return result
 *   @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_erase_write(const FlashDevice *device, PartitionId in_partition,
                          uint32_t offset, const void *in_buf,
                          uint32_t in_buf_len);

/**
 * @brief Read data from an area on a Flash to data buffer in RAM
 *
 * @param[in]  device The instance of flash device
 * @param[in]  in_partition The target flash logical partition which should
 * be read
 * @param offset  Point to the start address that the data is read
 * @param[in]  out_buf Point to the data buffer that stores the data
 * read from flash
 * @param[in]  in_buf_len  The length of the buffer
 * @return Return result
 *   @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_read(const FlashDevice *device, PartitionId in_partition,
                   uint32_t offset, void *out_buf, uint32_t in_buf_len);

/**
 * @brief Read data from a page
 * @param[in] device The instance of flash device
 * @param addr start address of the page
 * @param[in] size The size of page to read
 * @param[in] out_buf point to the data buffer which will be read from flash
 * @return Return result
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_flash_read_page(const FlashDevice *device, uint32_t addr, uint32_t size,
                        void *out_buf);

/**
 * @brief Control power mode of flash
 * @param[in] device The instance of flash device
 * @param mode new power mode for flash
 * @return Return result
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_flash_power_mode(const FlashDevice *device, FlashPowerMode mode);

/**
 * @brief get flash page/sector from nor
 *
 * @param[in]  device The instance of flash device
 * @param[out]  nor pagezie and sectoresize of every flash
 * @return Return result
 *   @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_get_size(const FlashDevice *device, NorFlashDev *nor);

/**
 * @brief get flash uid/csid from nor
 *
 * @param[in]  device The instance of flash device
 * @param[out]  nor uid and csid of every flash
 * @return Return result
 *   @retval   VSD_SUCCESS: On success,  otherwise is error
 */
int hal_flash_get_factory_info(const FlashDevice *device, NorFactoryDev *nor);
/** @} */

/**
 * @brief reset flash
 * @param device device instance of flash device
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_reset(const FlashDevice *device);

#ifdef __cplusplus
}
#endif

#endif /* HAL_FLASH_H */
