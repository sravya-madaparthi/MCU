#include "stm32f4xx_hal.h"
#include <stdio.h>

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
int _write(int file, char *ptr, int len);

// Global variables
uint8_t press_count = 0;
uint8_t led1_state = 0;
uint8_t led2_state = 0;

int main(void) {
    HAL_Init();               // Initialize HAL Library
    SystemClock_Config();      // Configure System Clock
    MX_GPIO_Init();            // Initialize GPIO

    while (1) {
        // Check if switch (PC13) is pressed
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
            HAL_Delay(200); // Debounce delay

            // Wait for switch release
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);

            press_count++;  // Increase press count

            // Toggle LED states
            if (press_count % 2 == 1) {
                led1_state = 1;
                led2_state = 0;
            } else {
                led1_state = 0;
                led2_state = 1;
            }

            // Update LEDs
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, led1_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, led2_state ? GPIO_PIN_SET : GPIO_PIN_RESET);

            // Print status to SWV ITM Console
            printf("Switch pressed: %d times | LED1: %s | LED2: %s\r\n",
                   press_count, led1_state ? "ON" : "OFF", led2_state ? "ON" : "OFF");
        }
    }
}

// System Clock Configuration (Use STM32CubeMX to generate)
void SystemClock_Config(void) {}

// GPIO Initialization
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA, GPIOB, and GPIOC clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PA5 as Output (Onboard LED)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PB7 as Output (External LED)
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure PC13 as Input (Onboard Button) with Pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// Redirect printf to ITM (SWV Data Console)
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}
