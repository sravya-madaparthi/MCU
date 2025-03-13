/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body with optimized UART
  **************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
uint8_t rxData[10]; // Buffer for received data

/* Function Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Initialize the HAL Library */
  HAL_Init();
  /* Configure the system clock */
  /* Initialize all configured peripherals */
  MX_USART2_UART_Init();

  // Start non-blocking UART reception
  HAL_UART_Receive_IT(&huart2, rxData, sizeof(rxData));

  while (1)
  {
    char *msg = "Hello, UART\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
    HAL_Delay(500); // Short delay to observe output frequency
  }
}

/**
  * @brief USART2 Initialization Function
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Callback function for UART reception complete interrupt
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    HAL_UART_Transmit(&huart2, rxData, sizeof(rxData), 100); // Echo received data
    HAL_UART_Receive_IT(&huart2, rxData, sizeof(rxData)); // Restart reception
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
