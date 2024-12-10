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

#include "24cxx.h"
#include "lcd.h"
#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"

/* USER CODE BEGIN 0 */
USARTRecvBuffer USART1Buffer = {{0}, {0}, 0, USART_RECV_MAXLEN};
uint8_t data_buf[BUF_SIZE] = {0};
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void) {
  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle) {
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

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle) {
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

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  if (huart->Instance == USART1) {
    if (!(USART_OK_LOCAL(USART1Buffer))) {
      if (USART_CRLF_LOCAL(USART1Buffer)) {
        if (USART1Buffer.curBuffer[0] != 0x0A)
          USART1Buffer.rxlen = 0;
        else {
          USART1Buffer.uBuffer[USART_RECVLENGTH_LOCAL(USART1Buffer)] = '\n';
          USART1Buffer.rxlen |= USART_RECV_OK;
        }
      } else {
        if (USART1Buffer.curBuffer[0] == 0x0D) {
          USART1Buffer.rxlen |= USART_RECV_CRLF;
        } else {
          USART1Buffer.uBuffer[USART_RECVLENGTH_LOCAL(USART1Buffer)] = USART1Buffer.curBuffer[0];
          USART1Buffer.rxlen++;
          if (USART1Buffer.rxlen >= USART1Buffer.maxlen) {
            USART1Buffer.rxlen = 0;
          }
        }
      }
    } else {
      AT24CXX_Read(0, data_buf, BUF_SIZE);
      // HAL_UART_Transmit(&huart1, data_buf, BUF_SIZE, 0xFF);
      AT24CXX_Write(0, USART1Buffer.uBuffer, USART1Buffer.rxlen & 0xFF);
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      USART1Buffer.rxlen = 0;
    }
  }
}
/* USER CODE END 1 */
