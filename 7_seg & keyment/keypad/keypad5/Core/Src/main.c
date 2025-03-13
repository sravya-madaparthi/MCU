#include "stm32f4xx_hal.h"
#include <string.h>

UART_HandleTypeDef huart1;

// 7-Segment Display Patterns for 0-9 (Common Cathode)
const uint8_t segment_map[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Keypad Mapping (4x3)
char keypad_map[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Function to Initialize USART1
void USART1_Init(void) {
    __HAL_RCC_USART1_CLK_ENABLE();
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
}

// Function to Send Data Over USART1
void USART1_Send(char *str) {
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

// Function to Initialize GPIO
void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Keypad ROWS (PA0 - PA3) as OUTPUT
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Keypad COLUMNS (PB0 - PB2) as INPUT with PULL-UP
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 7-Segment Display Pins (PC0 - PC6) as OUTPUT
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
                          GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// Function to Read Keypad Input
char Read_Keypad(void) {
    for (int row = 0; row < 4; row++) {
        // Set all rows HIGH, then set one row LOW
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, (GPIO_PIN_0 << row), GPIO_PIN_RESET);

        for (int col = 0; col < 3; col++) {
            if (HAL_GPIO_ReadPin(GPIOB, (GPIO_PIN_0 << col)) == GPIO_PIN_RESET) {
                while (HAL_GPIO_ReadPin(GPIOB, (GPIO_PIN_0 << col)) == GPIO_PIN_RESET); // Wait for key release
                return keypad_map[row][col];
            }
        }
    }
    return 0; // No key pressed
}

// Function to Display Number on 7-Segment
void Display_7Segment(char key) {
    if (key >= '0' && key <= '9') {
        uint8_t segments = segment_map[key - '0'];
        for (int i = 0; i < 7; i++) {
            HAL_GPIO_WritePin(GPIOC, (GPIO_PIN_0 << i), (segments >> i) & 1);
        }
    }
}

// Main Function
int main(void) {
    HAL_Init();

    GPIO_Init();
    USART1_Init();

    USART1_Send("STM32 Keypad & 7-Segment Display Initialized!\r\n");

    while (1) {
        char key = Read_Keypad();
        if (key) {
            char msg[30];
            sprintf(msg, "Key Pressed: %c\r\n", key);
            USART1_Send(msg);
            Display_7Segment(key);
        }
        HAL_Delay(200); // Small delay for debounce
    }
}
