/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    dma.c
 * @brief   This file provides code for the configuration
 *          of all the requested memory to memory DMA transfers.
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
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
DMA_HandleTypeDef hdma_memtomem_dma1_channel6;

/**
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma1_channel6
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_channel6 on DMA1_Channel6 */
  hdma_memtomem_dma1_channel6.Instance = DMA1_Channel6;
  hdma_memtomem_dma1_channel6.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_channel6.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_channel6.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma1_channel6.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_memtomem_dma1_channel6.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_memtomem_dma1_channel6.Init.Mode = DMA_CIRCULAR;
  hdma_memtomem_dma1_channel6.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_channel6) != HAL_OK)
  {
    Error_Handler();
  }

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/* USER CODE BEGIN 2 */
/**
 * @brief Control Word for LCD 8080 interface
 * CS/RS/WR/RD is on GPIOC PC9/PC8/PC7/PC6
 * Map the corresponding control word from program memory to mmap'ed GPIOC->BSRR with DMA1_Channel6
 *
 * ```
 * WIRE SET   RESET
 * CS   0x200 0x2000000
 * RS   0x100 0x1000000
 * WR   0x080 0x0800000
 * RD   0x040 0x0400000
 * ```
 *
 * Data Frame Structure:
 *
 * ```
 * |RS|CS_CLR|HOLD|WR_CLR|WR_SET|CS_SET|
 * |
 * ```
 *
 */


/* USER CODE END 2 */
