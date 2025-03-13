#include "stm32f4xx_hal.h"
#include <stdio.h>

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void toggle_led(uint8_t times);
void delay_ms(uint32_t ms);

// Global variable to count switch presses
uint8_t press_count = 0;

int main(void) {
    HAL_Init();               // Initialize HAL Library
    SystemClock_Config();      // Configure System Clock
    MX_GPIO_Init();           // Initialize GPIO

    while (1) {
        // Check if switch (PC13) is pressed
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
            HAL_Delay(200); // Debounce delay

            // Wait for switch release
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);

            press_count++;  // Increase press count

            // Reset press count after 3 presses
            if (press_count > 3) {
                press_count = 1;
            }

            // Toggle LED based on press count
            toggle_led(press_count);

            // Print press count to SWV ITM Data Console
            printf("Switch pressed: %d times\r\n", press_count);
        }
    }
}

// Function to toggle LED multiple times
void toggle_led(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(300);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(300);
    }
}

// System Clock Configuration
void SystemClock_Config(void) {
    // System clock configuration can be generated using STM32CubeMX
}

// GPIO Initialization
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA and GPIOC clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PA5 as Output (Onboard LED)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PC13 as Input (Onboard Button) with Pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// Delay function
void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

// Redirect printf to ITM (SWV Data Console)
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}
