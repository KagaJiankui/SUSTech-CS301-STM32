/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"

#include <stdio.h>

#include "main.h"
#include "usart.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern USARTRecvBuffer USART1Buffer;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1) {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
#if defined(__CC_ARM)
__asm void HardFault_Handler(void) { TST lr, #4 ITE EQ MRSEQ r0, MSP MRSNE r0, PSP B __cpp(Hard_Fault_Handler) }
#elif defined(__ICCARM__) || defined(__GNUC__)
void HardFault_Handler(void) {
  __asm volatile("TST lr, #4");
  __asm volatile("ITE EQ");
  __asm volatile("MRSEQ r0,MSP");
  __asm volatile("MRSNE r0,PSP");
  __asm volatile("B Hard_Fault_Handler");
}
#else
#warning "Unknown toolchain"
#endif
void printUsageErrorMsg(uint32_t CFSRValue) {
  printf("Usage fault: ");
  CFSRValue >>= 16;  // right shift to lsb
  if ((CFSRValue & (1 << 9)) != 0) {
    printf("Divide by zero\n");
  }
  if ((CFSRValue & (1 << 8)) != 0) {
    printf("Unaligned access \\n");
  }
}
void printBusFaultErrorMsg(uint32_t CFSRValue) {
  printf("Bus fault: ");
  CFSRValue >>= 16;  // right shift to lsb
}
void printMemManageErrorMsg(uint32_t CFSRValue) {
  printf("Memory Management fault: ");
  CFSRValue >>= 16;  // right shift to lsb
}

enum { r0, r1, r2, r3, r12, lr, pc, psr };

void stackDump(uint32_t stack[]) {
  static char msg[80];
  sprintf(msg, "r0  = 0x%08x\n", stack[r0]);
  printf(msg);
  sprintf(msg, "r1  = 0x%08x\n", stack[r1]);
  printf(msg);
  sprintf(msg, "r2  = 0x%08x\n", stack[r2]);
  printf(msg);
  sprintf(msg, "r3  = 0x%08x\n", stack[r3]);
  printf(msg);
  sprintf(msg, "r12 = 0x%08x\n", stack[r12]);
  printf(msg);
  sprintf(msg, "lr  = 0x%08x\n", stack[lr]);
  printf(msg);
  sprintf(msg, "pc  = 0x%08x\n", stack[pc]);
  printf(msg);
  sprintf(msg, "psr = 0x%08x\n", stack[psr]);
  printf(msg);
}
void Hard_Fault_Handler(uint32_t stack[]) {
  static char msg[80];
  printf("In Hard Fault Handler\\n");
  sprintf(msg, "SCB->HFSR = 0x%08x\\n", SCB->HFSR);
  printf(msg);
  if ((SCB->HFSR & (1 << 30)) != 0) {
    printf("Forced Hard Fault\\n");
    sprintf(msg, "SCB->CFSR = 0x%08x\\n", SCB->CFSR);
    printf(msg);
    if ((SCB->CFSR & 0xFFFF0000) != 0) {
      printUsageErrorMsg(SCB->CFSR);
    }
    if ((SCB->CFSR & 0xFF00) != 0) {
      printBusFaultErrorMsg(SCB->CFSR);
    }
    if ((SCB->CFSR & 0xFF) != 0) {
      printMemManageErrorMsg(SCB->CFSR);
    }
  }
  stackDump(stack);
  __ASM volatile("BKPT #01");
  while (1);
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1) {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void) {
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1) {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1) {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void) {
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void) {
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void) {
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) {
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)USART1Buffer.curBuffer, USART_RXBUFFER_LEN);
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
