#include "lcd.h"

// Define LCD GPIO pins (adjust based on your board)
#define RS_PIN GPIO_PIN_0
#define RW_PIN GPIO_PIN_1
#define E_PIN  GPIO_PIN_2
#define D4_PIN GPIO_PIN_3
#define D5_PIN GPIO_PIN_4
#define D6_PIN GPIO_PIN_5
#define D7_PIN GPIO_PIN_6

#define LCD_PORT GPIOB  // Change based on your connection

void LCD_SendCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET); // RS = 0 (Command mode)
    HAL_GPIO_WritePin(LCD_PORT, RW_PIN, GPIO_PIN_RESET); // RW = 0 (Write mode)

    // Send upper nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (cmd >> 4) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (cmd >> 5) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (cmd >> 6) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (cmd >> 7) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);

    // Send lower nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (cmd >> 0) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (cmd >> 1) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (cmd >> 2) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (cmd >> 3) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);

    HAL_Delay(2);
}

void LCD_SendData(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET); // RS = 1 (Data mode)
    HAL_GPIO_WritePin(LCD_PORT, RW_PIN, GPIO_PIN_RESET); // RW = 0 (Write mode)

    // Send upper nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 4) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 5) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 6) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 7) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);

    // Send lower nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 0) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 1) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 2) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 3) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);

    HAL_Delay(2);
}

void LCD_SendString(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 + col : 0xC0 + col;
    LCD_SendCommand(address);
}

void LCD_Clear(void) {
    LCD_SendCommand(0x01);  // Clear display command
    HAL_Delay(2);
}

void LCD_Init(void) {
    // GPIO Initialization
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = RS_PIN | RW_PIN | E_PIN | D4_PIN | D5_PIN | D6_PIN | D7_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);

    // LCD Initialization Sequence
    HAL_Delay(50);
    LCD_SendCommand(0x33);
    LCD_SendCommand(0x32);
    LCD_SendCommand(0x28); // 4-bit mode, 2 lines, 5x8 font
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    LCD_SendCommand(0x06); // Entry mode
    LCD_SendCommand(0x01); // Clear display
    HAL_Delay(2);
}
