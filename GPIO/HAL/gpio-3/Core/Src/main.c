#include "stm32f4xx_hal.h"

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void delay_ms(uint32_t ms);

int main(void) {
    HAL_Init();               // Initialize the HAL Library
    SystemClock_Config();      // Configure the system clock
    MX_GPIO_Init();           // Initialize GPIO pins for LEDs and switches

    while (1) {
        // Check if switch 1 (PB8) is pressed
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // Turn on LED 1 (PA7)
            delay_ms(500);  // Delay to keep the LED on
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Turn off LED 1 (PA7)
            delay_ms(200);  // Simple debounce delay
        }

        // Check if switch 2 (PB9) is pressed
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // Turn on LED 2 (PA8)
            delay_ms(500);  // Delay to keep the LED on
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Turn off LED 2 (PA8)
            delay_ms(200);  // Simple debounce delay
        }

        // Check if switch 3 (PC9) is pressed
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Turn on LED 3 (PA9)
            delay_ms(500);  // Delay to keep the LED on
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Turn off LED 3 (PA9)
            delay_ms(200);  // Simple debounce delay
        }
    }
}

void SystemClock_Config(void) {
    // This function can be configured based on your clock settings,
    // usually configured via STM32CubeMX.
}

static void MX_GPIO_Init(void) {
    // GPIO Initialization Structure
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA, GPIOB, GPIOC clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PA7, PA8, PA9 as output (LEDs)
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PB8, PB9, PC9 as input (Switches)
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // Enable pull-up resistors
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void delay_ms(uint32_t ms) {
    // Simple delay function using HAL_Delay
    HAL_Delay(ms);  // This function is non-blocking and uses the system tick timer
}
