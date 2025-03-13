#include "stm32f4xx_hal.h"

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void) {
    HAL_Init();               // Initialize the HAL Library
    SystemClock_Config();      // Configure the system clock
    MX_GPIO_Init();           // Initialize GPIO

    while (1) {
        // Read switch states
        uint8_t switch1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
        uint8_t switch2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
        uint8_t switch3 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);

        // If Switch 1 is pressed (active-low)
        if (switch1 == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // LED 1 ON
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // LED 1 OFF
        }

        // If Switch 2 is pressed (active-low)
        if (switch2 == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // LED 1 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // LED 2 ON
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // LED 2 OFF
        }

        // If Switch 3 is pressed (active-low)
        if (switch3 == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // LED 1 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // LED 2 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // LED 3 ON
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // LED 3 OFF
        }
    }
}

void SystemClock_Config(void) {
    // Configure system clock (This can be generated from STM32CubeMX)
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

    // Configure PB8, PB9 as input (Switches with Pull-up)
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // Enable pull-up resistors
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure PC9 as input (Switch with Pull-up)
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
