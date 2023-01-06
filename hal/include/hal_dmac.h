/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_dmac.h
 * @brief public head file of DMAC driver
 */

#ifndef HAL_DMAC_H
#define HAL_DMAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

/* Peripheral type */
typedef enum DmacDevType {
    DMA_MEM   = 0,
    DMA_UART  = 1,
    DMA_SPI   = 2,
    DMA_I2C   = 3,
    DMA_FLASH = 4,
} DmacDevType;

typedef struct DmacChannelType {
    DmacDevType src;
    DmacDevType dst;
} DmacChannelType;

typedef struct DmacChannelList {
    DmacChannelType type;
    int8_t channel_num;
    bool busy_flag;
} DmacChannelList;

/* describe dmac channel info */
typedef struct DmacChannelInfo {
    uint32_t fifo_depth;
    /*
     * Limitation for CTLx.SRC_MSIZE and CTLx.DEST_MSIZE
     * For Memory to Memory, this parameter will be ignored
     */
    uint32_t max_burst_size;
    /*
     * Limitation for CTLx.BLOCK_TS
     * Also, It limits channel scatter and gather count registers
     */
    uint32_t max_trans_blocksize;
} DmacChannelInfo;

/* describe dmac hardware config */
typedef struct DmacHwConfig {
    /* The system interrupt NO. to corresponding DMAC */
    uint32_t intr_id;

    /* Index how many channel the DMAC holds */
    uint8_t chn_num;

    /* The base address of DMAC */
    uint32_t base_addr;

    /* Generally, SoC team will design each channel with the same
     * configuration. But in some special cases(oa1801 project),
     * these channels may be configured with difference value,
     * so we need look up file supplied by SoC to find out the correct
     * information.
     */
    const DmacChannelInfo *chnl;
} DmacHwConfig;

/**
 * @brief Callback handler for DMA transfer complete
 * @param device The device instance of peripheral, NULL for M2M
 */
typedef void (*DmaXferCallback)(const void *device);

typedef struct DmaCbAndParam {
    DmaXferCallback callback;
    const void *param;
} DmaCbAndParam;

/*
 * NOTES:
 *	In general, configuring the following items, then you can using DMA
 *		correctly. if you don't know what's means of other parameters,
 *		please leave them alone.
 */
typedef struct DmacChannelCfg {
    /*
     * chn_id will be assigned as the same with the handshake id
     * please refer to the DmacHandShakeId
     * NOTE: channel number starts from 0,1...
     */
    uint8_t chn_id;

    /*
     * which priority level(0~7) do you want to assign to channel.
     * A priority of 7 is the highest priority, and 0 is the lowest.
     */
    uint8_t prio;

    /* handshake signal id. Only used when peripherals are non-memory */
    uint8_t src_hs_id;
    uint8_t dst_hs_id;

    /* dma_intr: 0 disable dmac interrupt, 1 enable dmac interrupt. */
    uint8_t dma_intr;

    /*
     * the direction of this transfer.
     * DMA_MEM_TO_MEM: Memory to Memory.
     * DMA_MEM_TO_DEV: Memory to Device.
     * DMA_DEV_TO_MEM: Device to Memory.
     * DMA_DEV_TO_DEV: Device to Device.
     */
    uint8_t dir;

    /*
     * The change way of source or destination address, after every source
     *  or destination transfer.
     * 00 = Increment; 01 = Decrement; 1x = No change
     */
    uint8_t sinc;
    uint8_t dinc;

    /*
     * Source or destination peripherals transfer width.
     * Generally, we assign separately src_tr_width and dst_tr_width with
     * the width of peripherals FIFO.
     */
    uint8_t src_tr_width;
    uint8_t dst_tr_width;

    /*
     * Flow Control Mode
     * Determines when source transaction requests are serviced when
     * the Destination Peripheral is the flow controller.
     * 0x0 (FCMODE_0): Data pre-fetching is enabled(default)
     * 0x1 (FCMODE_1): Data pre-fetching is disabled
     */
    uint8_t fc_mode;

    /*
     * how many bytes you want to transfer.
     */
    uint32_t len;

    /*
     * source or destination addresses.
     */
    uint32_t src_addr;
    uint32_t dst_addr;

    /*
     * Source or destination Burst Transaction Length, instead of AHB burst
     * transfer length.
     * it's meaning that Number of data items, each of width src_tr_width
     * or dst_tr_width (NOT BYTE NUMBER), to be read/write from/to the
     * source/destination every time a source burst transaction.
     */
    uint32_t src_msize;
    uint32_t dst_msize;

    /*
     * Configure items of linked list multi-blocks transfer.
     * you can separately enable blocking chaining either source or
     * destination side by assigning with llp_src_en and llp_dst_en.
     *    true(1): enable    false(0): disable
     * if you don't want to use linked list mode, please leave llpx = 0.
     */
    void *llpx;
    bool llp_src_en;
    bool llp_dst_en;

    /*
     * these parameters are used to config the gather feature of SNPS AHB
     * DMAC. if you don't what you are doing, please leave these arguments
     * with 0.
     * src_gth_cnt: Source contiguous transfer count between
     *		successive gather boundaries.
     * src_gth_intvl: interval of contiguous source gather block.
     */
    uint32_t src_gth_cnt;
    uint32_t src_gth_intvl;

    /*
     * these parameters are used to config the scatter feature of SNPS AHB
     * DMAC. if you don't what you are doing, please leave these arguments
     * with 0.
     * dst_sct_cnt: Destination contiguous transfer count between
     *		successive scatter boundaries.
     * dst_sct_intvl: interval of contiguous destination scatter block.
     */
    uint32_t dst_sct_cnt;
    uint32_t dst_sct_intvl;

    /*
     * Whether enable auto-reload multi-blocks transfer.
     * 0 = disable     1 = enable
     */
    bool src_auto_reload;
    bool dst_auto_reload;

    /*
     * Driver will allocate automatically LLI array to finish this dma
     * xfer depending on the length of this transfer
     */
    void *lli;

    /*
     * Record the number of Linked list items of current transfer.
     */
    uint32_t lli_num;
} DmacChannelCfg;

/* describe dmac operation */
typedef struct DmacOperation {
    int (*init)(DmacHwConfig *dmac, DmacChannelCfg *xfer);
    int (*open)(DmacHwConfig *dmac, DmacChannelCfg *xfer);
    int (*start)(DmacHwConfig *dmac, DmacChannelCfg *xfer,
                 DmaCbAndParam *xfer_cb);
    int (*check_done)(DmacChannelCfg *xfer);
    int (*close_channel)(DmacHwConfig *dmac, DmacChannelCfg *xfer);
    int (*close)(DmacHwConfig *dmac);
    void (*isr_handler)(DmacHwConfig *dma);
} DmacOperation;

/**
 * @brief This struct define DMAC hardware
 */
typedef struct DmacHw {
    const DmacHwConfig *hw_config;
    const DmacChannelCfg **channel_config;
    const DmacOperation *ops;
} DmacHw;

/**
 * @brief Create dmac instance
 * @param[in] hw_config Pointer to hardware configuration of dmac
 * @return Return result
 * @return VSD_SUCCESS for succeed, others for failure
 */
int hal_dmac_create(const DmacHw *hw_config);

/**
 * brief alloc available dmac channel
 *
 * @param DmacDevType src  - source Peripheral
 * @param DmacDevType dest - destination Peripheral
 * @param channel_num      - assigned damc channel num
 *
 * @return available channel num on success, <0 on failure.
 */
int hal_dmac_channel_select(DmacDevType src, DmacDevType dst,
                            int8_t *channel_num);

/**
 * brief free assigned dmac channel
 *
 * @param channel_num      - assigned damc channel num
 *
 * @return VSD_SUCCESS for succeed, others for failure.
 */
int hal_dmac_channel_free(int8_t channel_num);
/**
 * init DMA channel
 *
 * @param channel_num - the DMA channel selected
 * @param src_addr - source address(set 0 if the source is a Peripheral)
 * @param dst_addr - destination address(set 0 if the dest is a Peripheral)
 * @param len - the length of transfer data
 *
 * @return VSD_SUCCESS on success, error code on failure.
 */
int hal_dmac_init(int8_t channel_num, uint32_t src_addr, uint32_t dst_addr,
                  uint32_t len);

/**
 * start DMA channel
 *
 * @param channel_num - the DMA channel selected
 * @param xfer_cb DMA transfer callback and param, the param of callback should
 * be device instance of peripheral, NULL for M2M
 * @return VSD_SUCCESS on success, error code on failure.
 */
int hal_dmac_start(int8_t channel_num, DmaCbAndParam *xfer_cb);

/**
 * check assigned dmac channel transfer complete
 *
 * @param channel_num - the DMA channel selected
 * @return VSD_SUCCESS on success, error code on failure.
 */
int hal_dmac_checkdone(int8_t channel_num);

/**
 * @brief DMA ISR handler
 */
void hal_dmac_isr_handler(void);

/**
 * get dmac channel list
 */
DmacChannelList *get_channel_list(void);

/**
 * get dmac channel sum
 */
int get_channel_sum(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_DMAC_H */
