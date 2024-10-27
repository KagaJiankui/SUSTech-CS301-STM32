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

#include <string.h>

#include "stm32f1xx_hal_uart.h"

/* USER CODE BEGIN 0 */
USARTRecvBuffer USART1Buffer = {{0}, {0}, 0, USART_RECV_MAXLEN};
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

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief Callback function executed on UART receive complete interrupt.
 *
 * This function handles the reception of data from USART1. It checks the
 * current state of the USART reception buffer and updates the buffer status
 * and content accordingly.
 *
 * It processes CRLF sequences and stores received
 * characters in a circular buffer fashion, the receive buffer will be
 * overwritten from the head if the buffer overflows.
 *
 * @param huart Pointer to the UART handle.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
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
          if (USART1Buffer.rxlen >= USART1Buffer.maxlen) USART1Buffer.rxlen = 0;
        }
      }
    }
  }
}

void FSM_Init(FSM *fsm) {
  fsm->state = STATE_IDLE;
  fsm->cmdIndex = 0;
  fsm->cmdBuffer.Buffer[0] = '\0';
}

void FSM_Process(FSM *fsm, USARTRecvBuffer *rxBuffer) {
  uint16_t pBuffer = 0;
  while (pBuffer <= USART_RECVLENGTH(rxBuffer) && rxBuffer->uBuffer[pBuffer]) {
    if (rxBuffer->uBuffer[pBuffer] == '\0') {
      fsm->state = STATE_DONE;
    }
    switch (fsm->state) {
      case STATE_IDLE:
        if (rxBuffer->uBuffer[pBuffer] == 'A') {
          fsm->state = STATE_AT;
        }
        break;
      case STATE_AT:
        if (rxBuffer->uBuffer[pBuffer] == 'T') {
          fsm->state = STATE_PLUS;
        } else {
          fsm->state = STATE_IDLE;
        }
        break;
      case STATE_PLUS:
        if (rxBuffer->uBuffer[pBuffer] == '+') {
          fsm->state = STATE_CMD;
        } else {
          fsm->state = STATE_IDLE;
        }
        break;
      case STATE_CMD:
        if (fsm->cmdIndex < 4) {
          bHEX(fsm->cmdBuffer.Buffer[fsm->cmdIndex++], rxBuffer->uBuffer[pBuffer]);
        } else {
          fsm->cmdIndex = 0;
          fsm->state=STATE_IDLE;
        }
        if(rxBuffer->uBuffer[pBuffer] == '\n') {
          fsm->state=STATE_ECHO;
        }
        break;
      case STATE_ECHO:
        // 回显
        HAL_UART_Transmit(&huart1, rxBuffer->uBuffer, USART_RECVLENGTH(rxBuffer), 1000);
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET);
        fsm->state = STATE_DONE;
        break;
      case STATE_DONE:
        fsm->cmdBuffer.Buffer[fsm->cmdIndex] = '\0';
        return;
      default:
        fsm->state = STATE_IDLE;
        break;
    }
    pBuffer++;
  }
}

void ATCommandProc(ATCommand *uATCommand, USARTRecvBuffer *uRxBuffer) {
  switch (uATCommand->Word) {
    case AT_CLR:
      memset(uRxBuffer->uBuffer, 0, sizeof(uint8_t) * 1024);
      uRxBuffer->rxlen = 0;
      break;
    case AT_PRNT:
      ATWordToString(uATCommand, uRxBuffer);
      break;

    default:
      break;
  }
}

void ATWordToString(ATCommand *uATCommand, USARTRecvBuffer *uRxBuffer) {
  if (uATCommand->Word != 0xFFFFFFFF) {
    uint32_t mask = 0xF0000000;
    for (uint8_t i = 0; i < 8; i++) {
      uRxBuffer->uBuffer[i] = (uATCommand->Word & (mask >> (4 * i))) + '0';
    }
  }
}
/* USER CODE END 1 */
