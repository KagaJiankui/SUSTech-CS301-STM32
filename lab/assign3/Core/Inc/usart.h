/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define USART_RECV_OK 0x8000
#define USART_RECV_CRLF 0x4000
#define USART_RECV_MASKLEN 0x3fff
#define USART_RECV_LEN 0x708
#define USART_RECV_BUFLEN 0x180
#define FRAME_LEN 0x1518
#define USART_FRAME_NUM (FRAME_LEN/USART_RECV_BUFLEN) // 缓冲数据帧数 = FRAME_LEN / USART_RECV_BUFLEN

#define USART_OK_LOCAL(usartbuf) (usartbuf.rxlen & USART_RECV_OK) == USART_RECV_OK
#define USART_OK(usartbuf) (((USARTRecvBuffer *)usartbuf)->rxlen & USART_RECV_OK) == USART_RECV_OK
#define USART_CRLF_LOCAL(usartbuf) (usartbuf.rxlen & USART_RECV_CRLF) == USART_RECV_CRLF
#define USART_CRLF(usartbuf) (((USARTRecvBuffer *)usartbuf)->rxlen & USART_RECV_CRLF) == USART_RECV_CRLF

#define USART_RECVLENGTH_LOCAL(usartbuf) (usartbuf.rxlen & USART_RECV_MASKLEN)
#define USART_RECVLENGTH(usartbuf) (((USARTRecvBuffer *)usartbuf)->rxlen & USART_RECV_MASKLEN)

/**
 * @brief 定义USART接收缓冲区. rxlen[15:0]定义如下:
 * ```
 *    | STATUS |  CRLF  | LENGTH[13:0] |
 *    |  :--:  |  :--:  |    :--:      |
 *    | 0x8000 | 0x4000 |    0x3fff    |
 * ```
 * @memberof uBuffer[USART_RECV_LEN] 存储所有接收到字节的接收缓冲区
 * @memberof curBuffer[USART_RECV_BUFLEN] 提供给HAL函数的接收缓冲区
 * @memberof rxlen[15:0] 当前接收到的字节数和接收状态
 *
 *
 * @memberof maxlen 接收缓冲区的最大长度
 */
typedef struct {
  uint8_t uBuffer[USART_RECV_LEN];
  volatile uint8_t curBuffer[USART_RECV_BUFLEN];
  uint16_t rxlen;
  uint16_t maxlen;
} USARTRecvBuffer;

#define curBufSize(buf) (sizeof(buf.curBuffer) / sizeof(buf.curBuffer[0]))
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
