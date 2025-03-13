#include "lcd.h"
#include "gpio.h"

/* LCD Initialization Function */
void LCD_Init(void)
{
    HAL_Delay(100);  // Power-on delay for LCD

    LCD_SendCommand(0x33);  // Initialize to 8-bit mode
    HAL_Delay(5);
    LCD_SendCommand(0x32);  // Switch to 4-bit mode
    HAL_Delay(5);

    LCD_SendCommand(0x28);  // 4-bit mode, 2 lines, 5x8 dots
    HAL_Delay(5);

    LCD_SendCommand(0x0C);  // Display ON, Cursor OFF
    HAL_Delay(5);

    LCD_SendCommand(0x06);  // Auto-increment cursor
    HAL_Delay(5);

    LCD_Clear();            // Clear display
    HAL_Delay(10);          // Important delay after clear display

    LCD_SetCursor(0, 0);    // Set cursor to start position
}

/* Send Command to LCD */
void LCD_SendCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // RS = 0 (Command mode)

    // Upper nibble
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (cmd >> 4) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (cmd >> 5) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (cmd >> 6) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (cmd >> 7) & 0x01);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Enable pulse
    HAL_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

    // Lower nibble
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, cmd & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (cmd >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (cmd >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (cmd >> 3) & 0x01);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Enable pulse
    HAL_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

/* Send Data to LCD */
void LCD_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // RS = 1 (Data mode)

    // Upper nibble
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (data >> 4) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (data >> 5) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (data >> 6) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (data >> 7) & 0x01);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Enable pulse
    HAL_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

    // Lower nibble
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, data & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (data >> 3) & 0x01);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Enable pulse
    HAL_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

/* Send String to LCD */
void LCD_SendString(char *str)
{
    while (*str)
    {
        LCD_SendData(*str++);
    }
}

/* Clear Display */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);  // Clear display
    HAL_Delay(10);          // Important delay for clear command
}

/* Set Cursor Position */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_SendCommand(address);
    HAL_Delay(2);
}
