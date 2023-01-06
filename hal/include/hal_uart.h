/**
 * Copyright (C) 2013. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @author: zhihui.ye@verisilicon.com, IoT Connectivity Platform
 * Date: 2022-05-12
 * @brief UART HAL interface
 */

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup UART
 *  UART HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define HAL_UART_DEV_MAX 4

typedef enum UartDevIdDef {
    UART_DEV_ID_0,
    UART_DEV_ID_1,
    UART_DEV_ID_2,
    UART_DEV_ID_3,
    UART_DEV_MAX,
} UartDevIdDef;

/* UART trigger level */
#define UART_RX_TRIGLVL0 0x00 /* 1 char in the fifo */
#define UART_RX_TRIGLVL1 0x01 /* fifo 1/4 full */
#define UART_RX_TRIGLVL2 0x02 /* fifo 1/2 full */
#define UART_RX_TRIGLVL3 0x03 /* fifo 1/8 less full */
#define UART_TX_TRIGLVL0 0x00 /* fifo empty */
#define UART_TX_TRIGLVL1 0x01 /* 2 chars in the fifo */
#define UART_TX_TRIGLVL2 0x02 /* fifo 1/4 full */
#define UART_TX_TRIGLVL3 0x03 /* fifo 1/2 full */

/* UART baud rate setting */
#define UART_BAUD_115200 115200
#define UART_BAUD_76800 76800
#define UART_BAUD_57600 57600
#define UART_BAUD_38400 38400
#define UART_BAUD_19200 19200
#define UART_BAUD_9600 9600
#define UART_BAUD_2M 2000000
#define UART_BAUD_1M 1000000
#define UART_BAUD_DIV1 1500000
#define UART_BAUD_DIV32 3125000

/** @brief Parity modes */
enum HalUartParity {
    UART_CFG_PARITY_NONE,
    UART_CFG_PARITY_ODD,
    UART_CFG_PARITY_EVEN,
    UART_CFG_PARITY_MARK,
    UART_CFG_PARITY_SPACE,
};

/** @brief Number of stop bits */
enum HalUartStopBits {
    UART_CFG_STOP_BITS_0_5,
    UART_CFG_STOP_BITS_1,
    UART_CFG_STOP_BITS_1_5,
    UART_CFG_STOP_BITS_2,
};

/** @brief Number of data bits */
enum HalUartDataBits {
    UART_CFG_DATA_BITS_5,
    UART_CFG_DATA_BITS_6,
    UART_CFG_DATA_BITS_7,
    UART_CFG_DATA_BITS_8,
    UART_CFG_DATA_BITS_9,
};

/** @brief Hardware flow control options */
enum HalUartFlowCtl {
    UART_CFG_FLOW_CTRL_NONE,
    UART_CFG_FLOW_CTRL_RTS_CTS,
    UART_CFG_FLOW_CTRL_DTR_DSR,
};

/**
 * @brief Structure of UART hardware configuration
 */
typedef struct UartHwConfig {
    uint32_t base; /**< Base address of UART device */
    uint8_t id; /**< Device ID of this UART device, @see UartDevIdDef */
    uint8_t xfer_capability; /**< UART capability */
    uint8_t rx_trig_lvl; /**< RX trigger level */
    uint8_t tx_trig_lvl; /**< TX trigger level */
    uint8_t dma_mode; /**< DMA mode, 0 for disable, 1 for enable */
    uint16_t clk_id; /**< Clock ID of this UART device */
    uint16_t irq_id; /**< IRQ ID of this UART device */
} UartHwConfig;

/* UART transfer config */
typedef struct UartXferConfig {
    uint32_t baud_rate; /**< Baud rate */
    uint8_t xfer_mode; /**< Transfer mode, definition @see UartXferMode */
    uint8_t word_size; /**< Word size config */
    uint8_t parity; /**< Parity config */
    uint8_t stop_bit; /**< Stop bit */
    uint8_t fc_enable; /**< flow control enable */
    uint8_t fifo_enable; /**< Enable FIFO support */
    uint8_t loop_back; /**< loop back enable */
} UartXferConfig;

struct UartDriverApi;
/**
 * @brief Runtime UART device structure (in memory)
 */
typedef struct UartDevice {
    uint8_t dev_id; /**< Device ID of UART controller */
    const UartHwConfig *hw_cfg; /**< UART hardware configuration */
    const UartXferConfig *xfer_cfg; /**< Transfer configuration */
    void *ops; /**< the Pointer for UART operations */
    void *ctx; /**< Context of UART device */
} UartDevice;

/** @brief UART Driver API structure. */
typedef struct UartOperations {
    /** Update uart config */
    int (*uart_config)(UartDevice *dev, const UartXferConfig *cfg);
    /** Flush RX FIFO by read RX FIFO */
    int (*fifo_flush)(const struct UartDevice *dev);
    /** Console data I/O function */
    uint32_t (*data_gets)(const UartDevice *dev, const uint32_t size,
                          char *data);
    uint32_t (*data_puts)(const UartDevice *dev, const uint32_t size,
                          const char *data);
    /** UART operation for irq handler */
    void (*irq_handler)(const UartDevice *dev);
} UartOperations;

/**
 * @brief Add the UART controller device
 * @param[in]  dev  the UART device
 * @return  VSD_SUCCESS on success, others on error
 */
int hal_uart_add_dev(UartDevice *dev);

/**
 * @brief Remove the UART device
 * @param[in]  dev  the UART device
 * @return  VSD_SUCCESS on success, others on error
 */
int hal_uart_remove_dev(UartDevice *dev);

/**
 * @brief Get the UART device by the id
 * @param[in] dev_id the UART device id
 * @return  UartDevice pointer: on success,  NULL : error
 */
UartDevice *hal_uart_get_device(uint8_t dev_id);

/**
 * @brief flush RX FIFO
 * @param[in]   dev   UART device structure
 * @param[in]   enable   true : enable, false : disable
 * @return  VSD_SUCCESS on success, others on error
 */
int hal_uart_fifo_flush(const UartDevice *dev);

/**
 * @brief Receive a char from UART
 * @param[in]   dev   UART device structure
 * @param[in]   c   the pointer to char
 *
 * @return  VSD_SUCCESS on success, others on error
 */
int hal_uart_get_char(const UartDevice *dev, char *c);

/**
 * @brief Transmit a char to output
 * @param[in]   dev   UART device structure
 * @param[in]   c   the char needs to be transmitted
 *
 * @return  VSD_SUCCESS on success, others on error
 */
int hal_uart_put_char(const UartDevice *dev, const char c);

/**
 * @brief UART receive string
 * @param[in]   dev   UART device structure
 * @param[in]   s   data to send
 * @param[in]   len   length of data to send
 * @param[out]  act_len pointer to actual length
 */
int hal_uart_get_string(const UartDevice *dev, char *s, uint32_t len,
                        uint32_t *act_len);

/**
 * @brief UART transmit string
 * @param[in]   dev   UART device structure
 * @param[in]   s   string to send
 */
int hal_uart_put_string(const UartDevice *dev, const char *s);

/**
 * @brief update uart config
 * @param[in]   dev   UART device structure
 * @param[in]   cfg   uart config, include baudrate, parity
 *                    stop_bits, data_bits, flow_ctrl
 * @return  VSD_SUCCESS on success, others on error
 */
int hal_uart_config(UartDevice *dev, const UartXferConfig *cfg);

/**
 * @brief UART irq handle function
 * @param dev UART device instance
 */
void hal_uart_irq_handler(const UartDevice *dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_UART_H_ */
