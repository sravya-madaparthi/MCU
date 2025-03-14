#include "stm32f4xx_hal.h"
#include <stdio.h>

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void delay_ms(uint32_t ms);

// Global variables
uint8_t press_count = 0;

int main(void) {
    HAL_Init();               // Initialize the HAL Library
    SystemClock_Config();      // Configure the system clock
    MX_GPIO_Init();           // Initialize GPIO

    while (1) {
        // Check if switch (PB8) is pressed
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) {
            HAL_Delay(200); // Debounce delay

            // Wait for switch release
            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET);

            press_count++;  // Increase press count

            // Control LEDs based on press count
            if (press_count == 1) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // LED1 ON
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
            } else if (press_count == 2) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // LED1 ON
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // LED2 ON
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
            } else if (press_count == 3) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // LED1 ON
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // LED2 ON
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // LED3 ON
            }

            // Turn off all LEDs after releasing the switch
            HAL_Delay(500);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

            // Reset press count if greater than 3
            if (press_count >= 3) {
                press_count = 0;
            }
        }
    }
}

// System Clock Configuration (Generated from STM32CubeMX)
void SystemClock_Config(void) {
    // Configure system clock (This can be generated from STM32CubeMX)
}

// GPIO Initialization
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA and GPIOB clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure PA7, PA8, PA9 as Output (LEDs)
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PB8 as Input (Switch) with Pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// Delay function
void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}
