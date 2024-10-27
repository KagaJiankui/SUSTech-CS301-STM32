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

#include <sys/types.h>
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
#define USART_RECV_LEN 0x3fff
#define USART_RECV_MAXLEN 1024
#define USART_STOREBUFFER_LEN 20
#define USART_RXBUFFER_LEN 1

// 定义状态机的状态
typedef enum { STATE_IDLE, STATE_AT, STATE_PLUS, STATE_CMD, STATE_ECHO, STATE_DONE } FSMState;

typedef union {
  uint8_t Buffer[4];
  uint32_t Word;
} ATCommand;

// 定义状态机结构体
typedef struct {
  FSMState state;
  ATCommand cmdBuffer;  // 用于存储AT命令
  int cmdIndex;
} FSM;

/**
 * @brief 定义USART接收缓冲区
 * @memberof uBuffer[USART_RECV_MAXLEN] 存储所有接收到字节的接收缓冲区
 * @memberof curBuffer[USART_RXBUFFER_LEN] 提供给HAL函数的接收缓冲区
 * @memberof rxlen[15:0] 当前接收到的字节数和接收状态
 *
 *    | STATUS |  CRLF  | LENGTH[13:0] |
 *    | 0x8000 | 0x4000 |    0x3fff    |
 *
 * @memberof maxlen 接收缓冲区的最大长度
 */
typedef struct {
  uint8_t uBuffer[USART_RECV_MAXLEN];
  volatile uint8_t curBuffer[USART_RXBUFFER_LEN];
  uint16_t rxlen;
  uint16_t maxlen;
} USARTRecvBuffer;

#define USART_OK_LOCAL(usartbuf) (usartbuf.rxlen & USART_RECV_OK) == USART_RECV_OK
#define USART_OK(usartbuf) (((USARTRecvBuffer *)usartbuf)->rxlen & USART_RECV_OK) == USART_RECV_OK
#define USART_CRLF_LOCAL(usartbuf) (usartbuf.rxlen & USART_RECV_CRLF) == USART_RECV_CRLF
#define USART_CRLF(usartbuf) (((USARTRecvBuffer *)usartbuf)->rxlen & USART_RECV_CRLF) == USART_RECV_CRLF

#define USART_RECVLENGTH_LOCAL(usartbuf) (usartbuf.rxlen & USART_RECV_LEN)
#define USART_RECVLENGTH(usartbuf) (((USARTRecvBuffer *)usartbuf)->rxlen & USART_RECV_LEN)

/**
 * @brief 将十六进制字符转换为对应的数值
 *
 * @param c 十六进制字符
 * @return 转换后的数值
 */
#define HEX(c) (c) - 'A' + 0xA

/**
 * @brief 将ASCII字符转换为对应的十六进制数值
 *
 * @param dest 存储转换结果的变量
 * @param src 待转换的ASCII字符
 */
#define bHEX(dest, src)                                                                                               \
  do {                                                                                                                \
    dest = ((src) >= '0' && (src) <= '9') ? ((src) - '0') : ((src) >= 'A' && (src) <= 'Z') ? ((src) - 'A' + 0xA) : 0; \
  } while (0)

/**
 * @brief 将数值转换为对应的ASCII十六进制字符
 *
 * @param n 数值
 * @return 对应的ASCII十六进制字符
 */
#define ASCIIHEX(n) ((n) < 10 ? ('0' + (n)) : ('A' + (n) - 10))

#define AT_CLR 0x0c151b00
#define AT_PRNT 0x191b171d
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void FSM_Init(FSM *fsm);
void FSM_Process(FSM *fsm, USARTRecvBuffer *rxBuffer);
void ATCommandProc(ATCommand *uATCommand, USARTRecvBuffer *uRxBuffer);
void ATWordToString(ATCommand *uATCommand, USARTRecvBuffer *uRxBuffer);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
