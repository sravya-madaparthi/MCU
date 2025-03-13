#include "stm32f4xx_hal.h"

// Define GPIO Pins for Rows and Columns
#define ROW1_PIN   GPIO_PIN_0
#define ROW2_PIN   GPIO_PIN_1
#define ROW3_PIN   GPIO_PIN_2
#define ROW4_PIN   GPIO_PIN_3
#define COL1_PIN   GPIO_PIN_4
#define COL2_PIN   GPIO_PIN_5
#define COL3_PIN   GPIO_PIN_6
#define COL4_PIN   GPIO_PIN_7

#define ROW_PORT   GPIOA
#define COL_PORT   GPIOA

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void Scan_Keypad(void);

int main(void) {
    // Initialize HAL Library
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    while (1) {
        Scan_Keypad();
        HAL_Delay(100); // Small delay for debouncing
    }
}

// Function to initialize GPIO
void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure ROWS as INPUT with PULL-UP
    GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);

    // Configure COLUMNS as OUTPUT
    GPIO_InitStruct.Pin = COL1_PIN | COL2_PIN | COL3_PIN | COL4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);
}

// Function to scan keypad and detect any key press
void Scan_Keypad(void) {
    for (uint8_t col = 0; col < 4; col++) {
        // Set one column LOW at a time
        HAL_GPIO_WritePin(COL_PORT, (COL1_PIN << col), GPIO_PIN_RESET);

        // Check if any row is pulled LOW
        if (HAL_GPIO_ReadPin(ROW_PORT, ROW1_PIN) == GPIO_PIN_RESET ||
            HAL_GPIO_ReadPin(ROW_PORT, ROW2_PIN) == GPIO_PIN_RESET ||
            HAL_GPIO_ReadPin(ROW_PORT, ROW3_PIN) == GPIO_PIN_RESET ||
            HAL_GPIO_ReadPin(ROW_PORT, ROW4_PIN) == GPIO_PIN_RESET) {

            // Key is pressed, print message
            printf("Key Pressed\n");

            // Wait until key is released (debouncing)
            while (HAL_GPIO_ReadPin(ROW_PORT, ROW1_PIN) == GPIO_PIN_RESET ||
                   HAL_GPIO_ReadPin(ROW_PORT, ROW2_PIN) == GPIO_PIN_RESET ||
                   HAL_GPIO_ReadPin(ROW_PORT, ROW3_PIN) == GPIO_PIN_RESET ||
                   HAL_GPIO_ReadPin(ROW_PORT, ROW4_PIN) == GPIO_PIN_RESET);

            HAL_Delay(50); // Debounce delay
        }

        // Set column back to HIGH
        HAL_GPIO_WritePin(COL_PORT, (COL1_PIN << col), GPIO_PIN_SET);
    }
}

// Dummy System Clock Configuration function
void SystemClock_Config(void) {
    // System Clock Configuration - Modify according to your board settings
}
