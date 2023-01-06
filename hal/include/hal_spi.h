/**
 *
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_spi.h
 * @brief Public head file of SPI HAL
 * @author Jinghua Yu<jinghua.yu@verisilicon.com>
 *
 */

#ifndef HAL_SPI_H
#define HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SPI
 *  - SPI HAL API and definition
 * @ingroup HAL
 * Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include "hal_common.h"

#define SPI_NAME_SIZE 16
#define SPI_DMA_BUFF_SIZE 32

/**
 * @enum SpiHwType
 * @brief Hardware type of SPI interface
 */
typedef enum SpiHwType {
    SPI_HW_TYPE_QSPI,
    SPI_HW_TYPE_SPI,
} SpiHwType;

/**
 * @enum SpiIdDef
 * @brief ID definition of SPI interface
 */
typedef enum SpiIdDef {
    SPI_ID_QSPI,
    SPI_ID_SPI0,
    SPI_ID_SPI1,
    SPI_ID_SPI2,
    SPI_ID_SPI3,
    SPI_ID_MAX,
} SpiIdDef;

#define HAL_SPI_DEV_MAX (SPI_ID_MAX)

/**
 * @enum SPI mode
 * @brief SPI interface work as master or slave
 */
typedef enum SpiMode {
    SPI_MODE_MASTER, /**< spi interface works as master */
    SPI_MODE_SLAVE, /**< spi interface works as slave */
} SpiMode;

typedef enum SpiWorkMode {
    SPI_WORK_MODE_TX,
    SPI_WORK_MODE_RX,
    SPI_WORK_MODE_FULL_DUPLEX,
    SPI_WORK_MODE_NORFLASH_READ,
} SpiWorkMode;

typedef enum SpiBatchCtrl {
    SPI_BATCH_DISABLE = 0x00, /**< disable batch mode*/
    SPI_BATCH_ENABLE  = 0x01, /**< enable batch mode */
} SpiBatchCtrl;

typedef enum SpiCpolCphaMode {
    SPI_CPOL_CPHA_MODE0,
    SPI_CPOL_CPHA_MODE1,
    SPI_CPOL_CPHA_MODE2,
    SPI_CPOL_CPHA_MODE3,
} SpiCpolCphaMode;

typedef enum SpiBusMode {
    SPI_BUS_STANDARD, /**< one line spi */
    SPI_BUS_DUAL, /**< two lines spi */
    SPI_BUS_QUAD, /**< four lines spi */
} SpiBusMode;

typedef enum SpiFlashCmdFormat {
    SPIFLASH_INST_ADDR_IN_STAND,
    SPIFLASH_INST_IN_STAND,
    SPIFLASH_INST_ADDR_DATA_IN_STD_SEL,
} SpiFlashCmdFormat;

/**
 * @brief transfer frame of SPI
 * @note D: data I: instruction A: address U: dummy C: continue
 */
typedef enum SpiTransFrame {
    SPI_TRANS_FRAME_NONE,
    SPI_TRANS_FRAME_D,
    SPI_TRANS_FRAME_I,
    SPI_TRANS_FRAME_IA,
    SPI_TRANS_FRAME_ID,
    SPI_TRANS_FRAME_IAD,
    SPI_TRANS_FRAME_IAUD,
    SPI_TRANS_FRAME_ACUD,
    SPI_TRANS_FRAME_AUD,
    SPI_TRANS_FRAME_IACUD,
} SpiTransFrame;

/**
 * @brief spi device select ID
 */
typedef enum SpiCsId {
    SPI_CS_ID_0,
    SPI_CS_ID_1,
    SPI_CS_ID_2,
    SPI_CS_ID_3,
    SPI_CS_ID_4,
    SPI_CS_ID_5,
    SPI_CS_ID_6,
    SPI_CS_ID_7,
    SPI_CS_ID_MAX,
} SpiCsId;

/**
 * @struct SpiTransConfig
 * @brief The configuration for SPI interface
 */
typedef struct SpiTransConfig {
    uint8_t cs_chip; /**< transfer cs chip  device */
    uint8_t xfer_mode; /**< transfer mode, @see XferModeDef */
    uint8_t cpol_cpha_mode; /**< work mode of SPI interface */
    uint8_t delay_in_us; /**< Delay in us for continue transmission */
    uint8_t batch_enable; /**< controller use of batch mode  */
    uint32_t speed_hz; /**< transfer speed_hz in Hz */
    uint32_t timeout; /**< transfer timeout */
} SpiTransConfig;

/**
 * @struct SpiMessage
 * @brief The strcut of SPI message
 */
typedef struct SpiMessage {
    uint8_t *tx_buf; /**< pointer of tx buffer */
    uint32_t tx_buf_len; /**< data size in tx buffer */
    uint8_t *rx_buf; /**< pointer of rx buffer */
    uint32_t rx_buf_len; /**< data size in rx buffer */
    /** specific xfer mode, SPI driver uses this mode instead of the one defined
       in SpiTransConfig, only effective for this message */
    uint8_t spec_xfer_mode;
    bool rx_cont; /**< Continue receiving, CS keeps low until manual stop */
    XferCallback callback; /**< Callback for transfer complete */
    void *cb_param; /**< parameters of XferCallback */
} SpiMessage;

typedef struct SpiFlashTransInfo {
    uint32_t addr_width : 6; /**< for flash, address width */
    uint32_t dummy_nums : 8; /**< for flash, dummy nums */
    uint32_t instruction : 8; /**< for flash, instruction */
    uint32_t cont_data : 8; /**< for flash, continue data */
    uint32_t cmd_format : 2; /**< command format of QSPI, see
                              * @SpiFlashCmdFormat */
    uint8_t inst_width; /**< width of instruction */
    uint8_t data_width; /**< width of data */
    uint8_t cont_width; /**< width of continut data */
    uint8_t trans_frame; /**< transfer frame, see @SpiTransFrame */
    uint8_t trans_dir; /**< transfer direction */
    uint8_t bus_mode; /**< bus mode of interface, main for QSPI */
    uint32_t address; /**< for flash, address */
} SpiFlashTransInfo;

/**
 * @brief Structure of spi hardware configuration
 */
typedef struct SpiHwConfig {
    uint8_t bus_num; /**< bus number of spi controller */
    uint8_t cs_number; /**< cs_number of spi controller device */
    uint8_t hw_type; /**< SPI hardware type, @see SpiHwType */
    uint8_t spi_mode; /**< SPI work mode */
    uint8_t clk_id; /**< Clock ID of SPI interfce */
    uint8_t rx_trig_level; /**< SPI interrupt rx trigger level*/
    uint8_t tx_trig_level; /**< SPI interrupt tx trigger level*/
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */
    uint16_t fifo_depth; /**< depth of Rx FIFO and Tx FIFO */
    uint32_t register_base; /**< register base address */
    uint32_t min_speed_hz; /**< min speed of transfer */
    uint32_t max_speed_hz; /**< max speed of transfer */
} SpiHwConfig;

/**
 * @struct SpiDevice
 * @brief Define SPI controller driver
 * @note Necessary attributes and functions have been added. User can add
 * more spi driver attributes here
 */
typedef struct SpiDevice {
    uint8_t bus_num; /**< bus number of spi controller */
    const char *name; /**< controller name of spi controller */
    const SpiHwConfig *hw_cfg; /**< Spi hardware configuration */
    const void *ops; /**< Spi operations */
    void *ctx; /**< Context of device */
} SpiDevice;

/**
 * @brief Structure of operations for SPI
 */
typedef struct SpiOperations {
    int (*prepare_msg)(const SpiDevice *device, SpiMessage **msg);
    int (*transfer_one_msg)(const SpiDevice *device, SpiMessage *msg,
                            const SpiTransConfig *trans_conf);
    int (*spi_flash_transfer)(const SpiDevice *device, SpiMessage *msg,
                              const SpiTransConfig *config,
                              SpiFlashTransInfo *info);
    void (*irq_ins_handler)(const SpiDevice *device);
} SpiOperations;

/**
 * @brief Add the SPI controller device
 * @param[in]  spi  the spi device
 * @return VSD_SUCCESS on success, otherwise is error
 * @note A SPI controller should be added before using
 */
int hal_spi_add_dev(SpiDevice *spi);

/**
 * @brief Remove the SPI device
 * @param[in]  spi  the spi device
 * @return VSD_SUCCESS on success, otherwise is error
 */
int hal_spi_remove_dev(SpiDevice *spi);

/**
 * @brief Get the SPI device by the id
 * @param[in] bus_num the spi device id, @see SpiIdDef
 * @return  SpiDevice pointer: on success,  NULL : error
 */
SpiDevice *hal_spi_get_dev_by_id(uint8_t bus_num);

/**
 * @brief Fill in the SPI message for transport, receive or both
 * @param[in]  spi_msg  msg to be transfer @see SpiMessage
 * @param[in]  tx_data  the data to be sent,NULL for no transport operation
 * @param[in]  tx_size  data size to be sent
 * @param[in]  rx_data  spi recv data,NULL for no receive operation
 * @param[in]  rx_size  data size to be received
 * @return VSD_SUCCESS on success, otherwise is error
 */
int hal_spi_fill_msg(SpiMessage *spi_msg, uint8_t *tx_data, uint32_t tx_size,
                     uint8_t *rx_data, uint32_t rx_size);

/**
 * @brief Fill in the SPI message for DMA transport, receive or both
 * @param[in]  spi_msg  msg to be transfer @see SpiMessage
 * @param[in]  tx_data  the data to be sent,NULL for no transport operation
 * @param[in]  tx_size  data size to be sent
 * @param[in]  rx_data  spi recv data,NULL for no receive operation
 * @param[in]  rx_size  data size to be received
 * @param[in]  callback  callback for transfer complete
 * @return VSD_SUCCESS on success, otherwise is error
 */
int hal_spi_fill_msg_dma(SpiMessage *spi_msg, uint8_t *tx_data,
                         uint32_t tx_size, uint8_t *rx_data, uint32_t rx_size,
                         XferCallback callback);
/**
 * @brief SPI transfer can be used to send,receive or both
 * @param[in]  spi  the spi device
 * @param[in]  msg  the data to be sent and recv
 * @param[in]  trans_config  the config of transfer
 * @return VSD_SUCCESS on success, otherwise is error
 */
int hal_spi_transfer(const SpiDevice *spi, SpiMessage *spi_msg,
                     const SpiTransConfig *trans_config);

/**
 * @brief spi trannsfer data to flash according to frame
 * @param device spi instance of spi device
 * @param msg transfer message
 * @param config transfer configuration
 * @param info spi flash transfer information
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_spiflash_transfer(const SpiDevice *device, SpiMessage *msg,
                          const SpiTransConfig *config,
                          SpiFlashTransInfo *info);

/**
 * @brief Interrupt handler for an External Interrupt
 * @param device The spi device instance
 */
void hal_spi_irq_handler(const SpiDevice *device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* HAL_SPI_H */
