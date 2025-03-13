#include "stm32f4xx_hal.h"
#include <stdio.h>

#define LED1_PIN  GPIO_PIN_7  // PA7
#define LED2_PIN  GPIO_PIN_8  // PA8
#define LED3_PIN  GPIO_PIN_9  // PA9
#define LED_PORT  GPIOA

#define SWITCH_PIN  GPIO_PIN_8  // PB8
#define SWITCH_PORT GPIOB

volatile uint8_t switch_press_count = 0;  // Store press count

void SystemClock_Config(void);
void GPIO_Init(void);
void reset_leds(void);

int main(void) {
    HAL_Init();

    GPIO_Init();

    while (1) {
        // Main loop does nothing, everything is handled in the interrupt
    }
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable GPIOB clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PA7, PA8, PA9 as Output (LEDs)
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure PB8 as Input (Switch) with Interrupt
    GPIO_InitStruct.Pin = SWITCH_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Falling edge trigger
    GPIO_InitStruct.Pull = GPIO_PULLUP; // Enable pull-up resistor
    HAL_GPIO_Init(SWITCH_PORT, &GPIO_InitStruct);

    // Enable EXTI Interrupt for PB8
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

// Interrupt Handler for PB8
void EXTI9_5_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(SWITCH_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(SWITCH_PIN); // Clear interrupt flag

        switch_press_count++; // Increase press count

        // Turn on LEDs based on press count
        if (switch_press_count == 1) {
            HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET); // LED1 ON
        } else if (switch_press_count == 2) {
            HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET); // LED2 ON
        } else if (switch_press_count == 3) {
            HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_SET); // LED3 ON
        }

        printf("Switch Pressed %d times\n", switch_press_count);
    }
}

// Interrupt for Switch Release (All LEDs OFF)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == SWITCH_PIN) {
        HAL_Delay(100);  // Simple debounce delay
        if (HAL_GPIO_ReadPin(SWITCH_PORT, SWITCH_PIN) == GPIO_PIN_SET) {  // If switch is released
            reset_leds();  // Turn OFF LEDs
            switch_press_count = 0;  // Reset count
            printf("Switch Released, LEDs OFF\n");
        }
    }
}

// Function to Reset All LEDs
void reset_leds(void) {
    HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_RESET);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}
