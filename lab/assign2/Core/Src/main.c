/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"

#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stm32f103xe.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void uintToBinString(uint8_t num, uint8_t len, uint8_t *str);
inline void modify_flag(uint8_t *flag_ptr, uint8_t flag_value);
void toggleLed(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin, uint8_t cycles, uint8_t delayMs);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t flag_display = 0;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  LCD_Init();
  uint8_t str_keywkup[22] = "KEY_WAKEUP is pressed";
  uint8_t str_key0[16] = "KEY0 is pressed";
  uint8_t str_key1[16] = "KEY1 is pressed";
  uint8_t str_standby[24] = "Listening KEY interrupt";
  uint8_t *msg_array[4] = {str_standby, str_keywkup, str_key0, str_key1};
  uint8_t cached_flag_display = flag_display;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LCD_Clear(WHITE);
  BACK_COLOR = WHITE;
  POINT_COLOR = BLACK;
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (cached_flag_display != flag_display) {
      LCD_Clear(WHITE);
      BACK_COLOR = WHITE;
      POINT_COLOR = BLACK;
    }
    LCD_ShowString(30, 40, 200, 12, 12, msg_array[flag_display]);
    cached_flag_display = flag_display;
    /* USER CODE END 3 */
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void custom_HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin) {
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u) {
    HAL_Delay(75);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
    LCD_Fill(30, 40, 320, 52, WHITE);
    HAL_Delay(25);
    switch (GPIO_Pin) {
      case KEY_WAKEUP_Pin:
        modify_flag(&flag_display, 1);
        break;
      case KEY0_Pin:
        modify_flag(&flag_display, 2);
        break;
      case KEY1_Pin:
        modify_flag(&flag_display, 3);
        break;
      default:
        break;
    }
  }
}

inline void modify_flag(uint8_t *flag_ptr, uint8_t flag_value) {
  if (*flag_ptr != flag_value) *flag_ptr = flag_value;
}

/**
 * @brief Converts a uint8_t number to a binary string with a specified length. This function works by looping through the bits of the number from
most significant to least significant bit.
 *
 * For each bit, the function sets the corresponding character in the string to '0' or '1', and then shifts the number right to move to the next bit.
If the length of the string is less than 8 or greater than 1, the function will not modify the string.
 *
 * @param[in] num: The number to convert.
 * @param[in] len: The length of the binary string. Must be between 1 and 8.
 * @param[out] str: The binary string.
 */
void uintToBinString(uint8_t num, uint8_t len, uint8_t *str) {
  if (len < 8 && len > 0) {
    for (uint8_t i = len - 1; i >= 0; i--) {
      str[i] = (num & 1) + '0';
      num >>= 1;
    }
  } else {
    return;
  }
}

/**
 * @brief Toggle a GPIO pin a specified number of times at a specified delay. Extracted all necessary functions from HAL library to minimize the
expense for function invocations.
 *
 * @param[in] GPIOx: GPIO port to use (e.g. GPIOA, GPIOB, ...)
 * @param[in] GPIO_Pin: Pin number to toggle (0-15)
 * @param[in] cycles: Number of times to toggle the pin
 * @param[in] delayMs: Delay in milliseconds between toggles
 */
void toggleLed(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin, uint8_t cycles, uint8_t delayMs) {
  uint32_t odr;
  if (IS_GPIO_PIN(GPIO_Pin)) {
    uint32_t tickstart = uwTick;
    uint32_t wait = delayMs;
    for (uint8_t i = 0; i < cycles; i++) {
      odr = GPIOx->ODR;
      GPIOx->BSRR = ((odr & GPIO_Pin) << 0x10) | (~odr & GPIO_Pin);
      if (wait < HAL_MAX_DELAY) {
        wait += (uint32_t)(uwTickFreq);
      }
      while ((uwTick - tickstart) < wait) {
      }
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
