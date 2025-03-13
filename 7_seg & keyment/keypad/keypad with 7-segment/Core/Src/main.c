#include "main.h"

// Define Keypad GPIO Pins
#define ROW1_PIN GPIO_PIN_0
#define ROW2_PIN GPIO_PIN_1
#define ROW3_PIN GPIO_PIN_2
#define ROW4_PIN GPIO_PIN_3
#define COL1_PIN GPIO_PIN_4
#define COL2_PIN GPIO_PIN_5
#define COL3_PIN GPIO_PIN_6
#define ROW_PORT GPIOA
#define COL_PORT GPIOA

// Define 7-Segment GPIO Pins
#define SEG_PORT GPIOB
#define SEG_A GPIO_PIN_0
#define SEG_B GPIO_PIN_1
#define SEG_C GPIO_PIN_2
#define SEG_D GPIO_PIN_3
#define SEG_E GPIO_PIN_4
#define SEG_F GPIO_PIN_5
#define SEG_G GPIO_PIN_6

// 7-Segment Display Mapping for Numbers 0-9 (Common Cathode)
uint8_t segmentMap[10] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111   // 9
};

// Keypad Layout (4x3)
char keyMap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Function Prototypes
void MX_GPIO_Init(void);
char scanKeypad(void);
void displayNumber(char key);

int main(void) {
    HAL_Init();

    MX_GPIO_Init();

    while (1) {
        char key = scanKeypad();  // Scan keypad for input
        if (key != 0) {
            displayNumber(key);    // Display the pressed key on 7-segment
            HAL_Delay(300);        // Debounce delay
        }
    }
}

// Function to scan the keypad
char scanKeypad(void) {
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_SET);

    for (int row = 0; row < 4; row++) {
        HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_SET);

        if (row == 0) HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_RESET);
        else if (row == 1) HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_RESET);
        else if (row == 2) HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_RESET);
        else HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_RESET);

        for (int col = 0; col < 3; col++) {
            uint16_t colPin = (col == 0) ? COL1_PIN : (col == 1) ? COL2_PIN : COL3_PIN;
            if (HAL_GPIO_ReadPin(COL_PORT, colPin) == GPIO_PIN_RESET) {
                return keyMap[row][col];  // Return the key pressed
            }
        }
    }
    return 0; // No key pressed
}

// Function to display a number on the 7-segment display
void displayNumber(char key) {
    uint8_t segments = 0;

    if (key >= '0' && key <= '9') {
        segments = segmentMap[key - '0'];  // Convert char to corresponding number
    } else {
        return;  // Ignore non-numeric keys
    }

    HAL_GPIO_WritePin(SEG_PORT, SEG_A, (segments & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_PORT, SEG_B, (segments & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_PORT, SEG_C, (segments & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_PORT, SEG_D, (segments & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_PORT, SEG_E, (segments & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_PORT, SEG_F, (segments & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_PORT, SEG_G, (segments & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// Function to initialize GPIO for Keypad and 7-Segment
void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Initialize Rows (PA0 to PA3) as Output
    GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);

    // Initialize Columns (PA4 to PA6) as Input with Pull-up Resistor
    GPIO_InitStruct.Pin = COL1_PIN | COL2_PIN | COL3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);

    // Initialize 7-Segment Pins (PB0 to PB6) as Output
    GPIO_InitStruct.Pin = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SEG_PORT, &GPIO_InitStruct);
}
