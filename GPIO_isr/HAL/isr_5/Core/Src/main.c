#include "stm32f4xx_hal.h"

#define LED_PIN   GPIO_PIN_7
#define LED_PORT  GPIOA

#define SWITCH1_PIN  GPIO_PIN_8  // PB8
#define SWITCH2_PIN  GPIO_PIN_9  // PB9
#define SWITCH_PORT  GPIOB

void SystemClock_Config(void);
void GPIO_Init(void);

int main(void) {
    HAL_Init();

    GPIO_Init();

    while (1) {
        // Main loop does nothing, everything is handled in interrupt
    }
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable GPIOB clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PA7 as Output (LED)
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure PB8 and PB9 as Input (Switches) with Interrupt
    GPIO_InitStruct.Pin = SWITCH1_PIN | SWITCH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Falling edge trigger
    GPIO_InitStruct.Pull = GPIO_PULLUP; // Enable pull-up resistor
    HAL_GPIO_Init(SWITCH_PORT, &GPIO_InitStruct);

    // Enable EXTI Interrupts for PB8 and PB9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

// Interrupt Handler for PB8 (Switch1) and PB9 (Switch2)
void EXTI9_5_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(SWITCH1_PIN) != RESET) {  // PB8 Pressed
        __HAL_GPIO_EXTI_CLEAR_IT(SWITCH1_PIN);
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // LED ON
    }

    if (__HAL_GPIO_EXTI_GET_IT(SWITCH2_PIN) != RESET) {  // PB9 Pressed
        __HAL_GPIO_EXTI_CLEAR_IT(SWITCH2_PIN);
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // LED OFF
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}
