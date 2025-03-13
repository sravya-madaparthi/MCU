#include "stm32f4xx_hal.h"

/* UART handle declaration */
UART_HandleTypeDef huart2;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void Error_Handler(void);

int main(void)
{
    char rxData;  // Variable to store received character

    /* Initialize the HAL Library */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize GPIO (for LED) and USART2 (for UART communication) */
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    /* Main loop: wait for a character and control the LED accordingly */
    while (1)
    {
        /* Receive one character in blocking mode */
        HAL_UART_Receive(&huart2, (uint8_t *)&rxData, 1, HAL_MAX_DELAY);

        if (rxData == 'N' || rxData == 'n')
        {
            /* Turn on the LED connected to PA5 */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        }
        else if (rxData == 'F' || rxData == 'f')
        {
            /* Turn off the LED connected to PA5 */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        }
    }
}

/* USART2 initialization function */
static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;               // Set baud rate (adjust if needed)
    huart2.Init.WordLength = UART_WORDLENGTH_8B;   // 8-bit word length
    huart2.Init.StopBits = UART_STOPBITS_1;        // 1 stop bit
    huart2.Init.Parity = UART_PARITY_NONE;         // No parity
    huart2.Init.Mode = UART_MODE_TX_RX;            // Enable both transmit and receive
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;   // No hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* GPIO initialization function */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable the clock for GPIOA (PA5 is on GPIOA) */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure PA5 as output for the LED */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up or pull-down resistors
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Low frequency is sufficient for LED control
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Note: USART2 typically uses PA2 (TX) and PA3 (RX). These pins are configured
       automatically by HAL_UART_Init() using their alternate function mode. */
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable power control clock and configure voltage scaling */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Configure the main internal oscillator (HSI) and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure bus clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Error Handler: In case of an error, the program stays in an infinite loop */
void Error_Handler(void)
{
    while (1)
    {
    }
}
