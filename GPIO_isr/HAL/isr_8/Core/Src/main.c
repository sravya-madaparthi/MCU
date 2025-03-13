#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define LED_PIN     GPIO_PIN_5
#define LED_PORT    GPIOA

#define SWITCH_PIN  GPIO_PIN_13
#define SWITCH_PORT GPIOC

volatile uint8_t press_count = 0; // Stores number of times switch is pressed

// Function to toggle LED N times
void Toggle_LED(uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(300); // 300ms delay for visible effect
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(300);
    }
}

// EXTI IRQ Handler
void EXTI15_10_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(SWITCH_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(SWITCH_PIN); // Clear interrupt flag

        press_count++; // Increase press count

        if (press_count > 3) {
            press_count = 1; // Reset to loop behavior
        }

        Toggle_LED(press_count); // Toggle LED according to count
    }
}

// GPIO Initialization
void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock

    // Configure LED (PA5) as OUTPUT
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure Switch (PC13) as INPUT with External Interrupt
    GPIO_InitStruct.Pin = SWITCH_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Detect falling edge (button press)
    GPIO_InitStruct.Pull = GPIO_PULLUP; // Use pull-up resistor
    HAL_GPIO_Init(SWITCH_PORT, &GPIO_InitStruct);

    // Enable Interrupt
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int main(void) {
    HAL_Init();  // Initialize HAL library
    GPIO_Init(); // Initialize GPIO

    while (1) {
        // Main loop does nothing, all logic is handled in interrupt
    }
}
