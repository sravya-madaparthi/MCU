#include "stm32f4xx_hal.h"
#include "stdio.h"

// Define LED and Button Pins
#define LED_PIN        GPIO_PIN_5  // Onboard LED (adjust per board)
#define LED_GPIO_PORT  GPIOA       // Adjust as per your board
#define BUTTON_PIN     GPIO_PIN_13 // User Button (adjust per board)
#define BUTTON_GPIO_PORT GPIOC

// Variables
volatile uint8_t led_state = 0;
volatile uint32_t button_press_count = 0;

// Function to Print to SWV ITM Data Console
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}

// Interrupt Service Routine for Button Press
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
        button_press_count++;
        led_state = !led_state;
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, (led_state) ? GPIO_PIN_SET : GPIO_PIN_RESET);

        if (led_state) {
            printf("LED ON\n");
        } else {
            printf("LED OFF\n");
        }
        printf("Button Pressed: %lu times\n", button_press_count);
    }
}

// GPIO Initialization Function
void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // LED Configuration
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    // Button Configuration
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);

    // Enable Interrupt for Button
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int main(void) {
    HAL_Init();
    GPIO_Init();

    // Print initial message
    printf("System Initialized\n");

    while (1) {
        // Loop doing nothing, waiting for interrupt
    }
}

// External Interrupt Handler
void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}
