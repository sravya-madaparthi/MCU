#include "stm32f4xx_hal.h"
#include <stdio.h>  // For printing, if semihosting is enabled

#define LED1_PIN     GPIO_PIN_5   // Onboard LED1 at PA5
#define LED2_PIN     GPIO_PIN_7   // Onboard LED2 at PA7
#define LED_PORT     GPIOA

#define SWITCH_PIN   GPIO_PIN_13  // Onboard switch at PC13
#define SWITCH_PORT  GPIOC

volatile uint8_t press_count = 0; // Global counter for switch presses

// Function to toggle LED1 and LED2 based on the current press count
void Toggle_LEDs(uint8_t count) {
    // Toggle LED1 'count' times
    for (uint8_t i = 0; i < count; i++) {
        HAL_GPIO_TogglePin(LED_PORT, LED1_PIN);
        HAL_Delay(300); // Delay for visibility
        HAL_GPIO_TogglePin(LED_PORT, LED1_PIN);
        HAL_Delay(300);
    }

    // Toggle LED2 '3 * count' times
    for (uint8_t i = 0; i < (3 * count); i++) {
        HAL_GPIO_TogglePin(LED_PORT, LED2_PIN);
        HAL_Delay(300);
        HAL_GPIO_TogglePin(LED_PORT, LED2_PIN);
        HAL_Delay(300);
    }
}

// External Interrupt Handler for PC13 (the onboard switch)
void EXTI15_10_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(SWITCH_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(SWITCH_PIN);  // Clear the interrupt flag

        press_count++;  // Increase the press count
        if (press_count > 3) {
            press_count = 1;  // Reset count after the 3rd press
        }

        // (Optional) Print the number of times the switch has been pressed.
        // Remove or comment out if semihosting/printf is not used.
        printf("Switch pressed %d times\n", press_count);

        // Toggle the LEDs based on the current press count.
        Toggle_LEDs(press_count);
    }
}

// GPIO Initialization: Configures LED pins and the switch pin
void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable clock for GPIOA (LEDs)
    __HAL_RCC_GPIOC_CLK_ENABLE();  // Enable clock for GPIOC (Switch)

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure LED pins (PA5 and PA7) as Output
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure the onboard switch (PC13) as Input with External Interrupt
    GPIO_InitStruct.Pin = SWITCH_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Falling edge trigger (press)
    GPIO_InitStruct.Pull = GPIO_PULLUP;          // Enable pull-up resistor
    HAL_GPIO_Init(SWITCH_PORT, &GPIO_InitStruct);

    // Enable and set priority for the EXTI line [15:10] interrupt (handles PC13)
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int main(void) {
    HAL_Init();    // Initialize the HAL library
    // SystemClock_Config();  // Configure the system clock if needed
    GPIO_Init();   // Initialize GPIO pins

    while (1) {
        // Main loop is empty; all action is interrupt-driven.
    }
}

// SysTick Handler required for HAL_Delay()
void SysTick_Handler(void) {
    HAL_IncTick();
}
