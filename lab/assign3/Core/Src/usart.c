/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include <string.h>

#include "lcd.h"
#include "memcpy.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
extern USARTRecvBuffer USART1Buffer;
enum {
  INIT,
  fStart_0x12,
  fStart_0x34,
  _CR,
  _LF,
  LenCount,
  _doRecv,
  _doMemcpy
} USART1_State;

/* USART1 init function */

void MX_USART1_UART_Init(void) {
  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 256000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (uartHandle->Instance == USART1) {
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK) {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {
  if (uartHandle->Instance == USART1) {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief Callback function for arbitrary length frame reception with UART Rx Event.
 *
 * This function is triggered when a data reception event occurs on USART1.
 * It utilizes `\r\n` sequence as a frame terminator, and toggles corresponding flag in `USARTRecvBuffer` struct accordingly. The buffer cycles back
 * to the beginning if any overflow occurs.
 *
 * Frame Structure:
 *
 * ```
 * | Header | Data_Legnth |               Data               |  CRLF  |
 * | 0x1234 |   <=0x1518  |sizeof(Data)<=5400*sizeof(uint8_t)| 0x0D0A |
 * ```
 *
 * @param huart Pointer to the UART handle for the receiving interface.
 * @param Size Number of bytes received during the event.
 * @note paremeter `uint16_t Size` pointers to `huart->RxXferSize` rather than `CNDTR`.
 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, volatile uint16_t Size) {
  uint8_t d_len[8] = {0};
  if (huart->Instance == USART1) {
    if (USART1Buffer.rxlen >= USART1Buffer.maxlen) {
      USART1Buffer.rxlen = 0;
    }
    uint8_t *dest = &USART1Buffer.uBuffer[(uint8_t)USART_RECVLENGTH_LOCAL(USART1Buffer)];
    sprintf(d_len, "%x:%s", (uint8_t)huart1.hdmarx->Instance->CNDTR, USART1Buffer.curBuffer);
    HAL_UART_Transmit(huart, d_len, sizeof(d_len), 1000);
    if (Size > 2) {
      memcpy(dest, (uint8_t *)(&USART1Buffer.curBuffer[4]), (Size - 2) * sizeof(uint8_t));
    }  // this thing copies u32?
    USART1Buffer.rxlen += Size;  // 2: header, 2: length, 2: CRLF
    memset((uint8_t *)USART1Buffer.curBuffer, 0, USART_RECV_BUFLEN * sizeof(uint8_t));
  }
}

/* USER CODE END 1 */
