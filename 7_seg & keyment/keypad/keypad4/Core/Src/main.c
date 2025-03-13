#include "stm32f4xx_hal.h"
#include <stdio.h>

UART_HandleTypeDef huart2;  // UART handle

// Define rows as output pins
#define ROW1 GPIO_PIN_0
#define ROW2 GPIO_PIN_1
#define ROW3 GPIO_PIN_2
#define ROW4 GPIO_PIN_3

// Define columns as input pins
#define COL1 GPIO_PIN_4
#define COL2 GPIO_PIN_5
#define COL3 GPIO_PIN_6
#define COL4 GPIO_PIN_7

#define KEYPAD_PORT GPIOC  // Change from GPIOA to GPIOC

// Keypad mapping
char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void UART2_Init(void);
char Read_Keypad(void);
void send_uart(char *message);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    UART2_Init();

    char key;
    char msg[20];

    while (1) {
        key = Read_Keypad();
        if (key != 0) {  // If a key is pressed
            sprintf(msg, "Key: %c\n", key);
            send_uart(msg);
            HAL_Delay(300); // Debounce delay
        }
    }
}

// Function to read the pressed key
char Read_Keypad(void) {
    for (int row = 0; row < 4; row++) {
        // Set all rows LOW first
        HAL_GPIO_WritePin(KEYPAD_PORT, ROW1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(KEYPAD_PORT, ROW2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(KEYPAD_PORT, ROW3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(KEYPAD_PORT, ROW4, GPIO_PIN_RESET);

        // Set one row HIGH at a time
        HAL_GPIO_WritePin(KEYPAD_PORT, (ROW1 << row), GPIO_PIN_SET);

        for (int col = 0; col < 4; col++) {
            if (HAL_GPIO_ReadPin(KEYPAD_PORT, (COL1 << col)) == GPIO_PIN_SET) {
                return keypad[row][col];  // Return key value
            }
        }
    }
    return 0;  // No key pressed
}

// Function to initialize GPIO
void GPIO_Init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();  // Enable clock for GPIOC

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure rows as output
    GPIO_InitStruct.Pin = ROW1 | ROW2 | ROW3 | ROW4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEYPAD_PORT, &GPIO_InitStruct);

    // Configure columns as input with pull-down
    GPIO_InitStruct.Pin = COL1 | COL2 | COL3 | COL4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEYPAD_PORT, &GPIO_InitStruct);
}

// UART initialization function
void UART2_Init(void) {
    __HAL_RCC_USART2_CLK_ENABLE();

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart2);
}

// Function to send a message over UART
void send_uart(char *message) {
    HAL_UART_Transmit(&huart2, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
}

// System Clock Configuration
void SystemClock_Config(void) {
    // Default clock configuration for STM32F103
}
