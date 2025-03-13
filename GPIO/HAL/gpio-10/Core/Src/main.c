#include "stm32f4xx_hal.h"
#include <stdio.h>

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void toggle_led(void);
int _write(int file, char *ptr, int len);

// Global variables
uint8_t press_count = 0;
uint8_t led_state = 0;  // 0 = OFF, 1 = ON

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

            // Toggle LED and print status
            led_state = !led_state;
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            printf("LED %s, Switch pressed: %d times\r\n", led_state ? "ON" : "OFF", press_count);
        }
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

    // Configure PA5 as Output (LED)
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

// Redirect printf to ITM (SWV Data Console)
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}
