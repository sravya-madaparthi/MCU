#include "stm32f4xx_hal.h"

// Function Prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void) {
    // HAL Initialization
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Initialize GPIO for LED and Switch
    MX_GPIO_Init();

    while (1) {
        // Check if the switch (PC9) is pressed (active low)
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET) {  // Switch pressed
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  // Turn ON LED (PA8)
        } else {  // Switch released
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  // Turn OFF LED (PA8)
        }
    }
}

void SystemClock_Config(void) {
    // The STM32F4 series typically has an external clock setup, configure the system clock here
    // This function can be left as default or configured based on your specific board.
    // HAL_RCC_OscConfig(), HAL_RCC_ClockConfig(), etc., might be used in more complex setups.
}

static void MX_GPIO_Init(void) {
    // GPIO Initialization Structure
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA and GPIOC clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PA8 as output (LED)
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PC9 as input (Switch) with pull-up resistor
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
