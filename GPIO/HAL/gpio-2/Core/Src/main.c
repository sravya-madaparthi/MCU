#include "stm32f4xx_hal.h"

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void delay_ms(uint32_t ms);

int main(void) {
    HAL_Init();               // Initialize the HAL Library
    SystemClock_Config();      // Configure the system clock
    MX_GPIO_Init();           // Initialize GPIO pins for LED and switch

    uint8_t led_state = 0;    // Track the LED state (0 = OFF, 1 = ON)
    uint8_t last_state = 1;   // Last state of the switch (high when released)
    uint8_t current_state = 1; // Current state of the switch

    while (1) {
        current_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9); // Read switch state (PC9)

        // Check for falling edge (switch pressed)
        if (last_state == 1 && current_state == 0) {
            // Toggle the LED state
            led_state = !led_state;
            if (led_state) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // Turn on LED (PA8)
            } else {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Turn off LED (PA8)
            }
            delay_ms(200); // Simple debounce delay
        }

        // Update last_state for next loop
        last_state = current_state;
    }
}

void SystemClock_Config(void) {
    // This is a placeholder function for configuring the system clock
    // This is usually done by STM32CubeMX, and you can leave this function
    // or modify it based on your clock settings.
}

static void MX_GPIO_Init(void) {
    // GPIO Initialization Structure
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA and GPIOC clock (for LED and switch)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PA8 as output (LED)
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PC9 as input (Switch)
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // Enable internal pull-up resistor
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void delay_ms(uint32_t ms) {
    // Simple delay function to debounce the switch
    HAL_Delay(ms);
}
