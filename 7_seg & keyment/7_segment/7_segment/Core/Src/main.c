/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body for 7-segment display counter on STM32
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private variables ---------------------------------------------------------*/
volatile uint8_t current_number = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void display_number(uint8_t number);

int main(void) {
  /* Initialize HAL Library */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* Infinite loop */
  while (1) {
    display_number(current_number);
    HAL_Delay(1000); // 1-second delay
    current_number = (current_number + 1) % 10; // Increment and wrap from 9 to 0
  }
}

/**
  * @brief Display the given number on the 7-segment display.
  * @param number: Number between 0 and 9
  */
void display_number(uint8_t number) {
  const GPIO_PinState segment_states[10][7] = {
    {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET},  // 0
    {GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_SET,   GPIO_PIN_SET,   GPIO_PIN_SET},  // 1
    {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_RESET},// 2
    {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_SET,   GPIO_PIN_RESET},// 3
    {GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET},// 4
    {GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET},// 5
    {GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},// 6
    {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_SET,   GPIO_PIN_SET,   GPIO_PIN_SET},  // 7
    {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},// 8
    {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_PIN_RESET} // 9
  };

  GPIO_TypeDef* ports[7] = {GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC};
  uint16_t pins[7] = {A_Pin, B_Pin, C_Pin, D_Pin, E_Pin, F_Pin, G_Pin};

  for (int i = 0; i < 7; i++) {
    HAL_GPIO_WritePin(ports[i], pins[i], segment_states[number][i]);
  }
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, A_Pin | B_Pin | C_Pin | D_Pin | E_Pin | F_Pin | G_Pin, GPIO_PIN_RESET);

  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = A_Pin | B_Pin | C_Pin | D_Pin | E_Pin | F_Pin | G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief Error Handler
  */
void Error_Handler(void) {
  __disable_irq();
  while (1) {
    // Stay here to handle error
  }
}
