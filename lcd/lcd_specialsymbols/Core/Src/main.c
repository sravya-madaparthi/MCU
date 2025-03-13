#include "stm32f4xx_hal.h"

/* --- LCD Pin Definitions (using GPIOB) --- */
#define LCD_RS_Pin    GPIO_PIN_0   // Register Select
#define LCD_RS_Port   GPIOB

#define LCD_EN_Pin    GPIO_PIN_1   // Enable
#define LCD_EN_Port   GPIOB

#define LCD_D4_Pin    GPIO_PIN_2   // Data bit 4
#define LCD_D4_Port   GPIOB

#define LCD_D5_Pin    GPIO_PIN_3   // Data bit 5
#define LCD_D5_Port   GPIOB

#define LCD_D6_Pin    GPIO_PIN_4   // Data bit 6
#define LCD_D6_Port   GPIOB

#define LCD_D7_Pin    GPIO_PIN_5   // Data bit 7
#define LCD_D7_Port   GPIOB

/* Function Prototypes for LCD Operations */
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char ch);
void LCD_WriteString(char *str);
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
static void LCD_SendNibble(uint8_t nibble);
static void LCD_ToggleEnable(void);

/* Function Prototypes for System and GPIO Initialization */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    /* Initialize HAL, system clock, and GPIO */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    /* Initialize and clear the LCD */
    LCD_Init();
    LCD_Clear();

    /* Define custom characters (5x8 dot matrix):
       Only the lower 5 bits of each byte are used to form the pattern.
    */

    // Custom character for alpha (α)
    uint8_t customAlpha[8] = {
        0x04,  // 00100
        0x0A,  // 01010
        0x0A,  // 01010
        0x0E,  // 01110
        0x0A,  // 01010
        0x0A,  // 01010
        0x00,  // 00000
        0x00   // 00000
    };

    // Custom character for beta (β)
    uint8_t customBeta[8] = {
        0x0E,  // 01110
        0x09,  // 01001
        0x09,  // 01001
        0x0E,  // 01110
        0x09,  // 01001
        0x09,  // 01001
        0x0E,  // 01110
        0x00   // 00000
    };

    // Custom character for pi (π)
    uint8_t customPi[8] = {
        0x1F,  // 11111  (Top horizontal bar)
        0x04,  // 00100
        0x04,  // 00100
        0x04,  // 00100
        0x04,  // 00100
        0x04,  // 00100
        0x04,  // 00100
        0x00   // 00000
    };

    // Custom character for ohm (Ω)
    uint8_t customOhm[8] = {
        0x0E,  // 01110
        0x11,  // 10001
        0x15,  // 10101
        0x15,  // 10101
        0x15,  // 10101
        0x11,  // 10001
        0x0E,  // 01110
        0x00   // 00000
    };

    /* Load custom characters into CGRAM at locations 0, 1, 2, and 3 */
    LCD_CreateCustomChar(0, customAlpha);
    LCD_CreateCustomChar(1, customBeta);
    LCD_CreateCustomChar(2, customPi);
    LCD_CreateCustomChar(3, customOhm);

    /* Display the custom characters along with labels on the LCD */
    LCD_SetCursor(0, 0);
    LCD_WriteString("Alpha: ");
    LCD_WriteChar(0);  // Displays custom alpha (α)
    LCD_WriteString(" Beta: ");
    LCD_WriteChar(1);  // Displays custom beta (β)

    LCD_SetCursor(1, 0);
    LCD_WriteString("Pi: ");
    LCD_WriteChar(2);  // Displays custom pi (π)
    LCD_WriteString(" Ohm: ");
    LCD_WriteChar(3);  // Displays custom ohm (Ω)

    while (1)
    {
        // Main loop (idle)
    }
}

/* System Clock Configuration (customize as needed) */
void SystemClock_Config(void)
{
    // This is a stub. In practice, configure your clocks using STM32CubeMX or manually.
}

/* Initialize GPIOB pins for LCD control and data */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable clock for GPIOB */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure PB0-PB5 as outputs */
    GPIO_InitStruct.Pin = LCD_RS_Pin | LCD_EN_Pin | LCD_D4_Pin | LCD_D5_Pin | LCD_D6_Pin | LCD_D7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* LCD Initialization: sets 4-bit mode and basic display settings */
void LCD_Init(void)
{
    HAL_Delay(40);  // Wait for LCD power-up

    /* Initialization sequence for many HD44780 LCDs */
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    LCD_SendNibble(0x30);  // Function set (8-bit mode command, sent as upper nibble)
    HAL_Delay(5);
    LCD_SendNibble(0x30);
    HAL_Delay(5);
    LCD_SendNibble(0x30);
    HAL_Delay(1);
    LCD_SendNibble(0x20);  // Set to 4-bit mode
    HAL_Delay(1);

    LCD_SendCommand(0x28);  // Function set: 4-bit, 2-line, 5x8 font
    LCD_SendCommand(0x0C);  // Display on, cursor off, blink off
    LCD_SendCommand(0x06);  // Entry mode: auto-increment cursor
}

/* Clear LCD display */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);  // Clear display command
    HAL_Delay(2);
}

/* Set the cursor position (row: 0 or 1, col: 0 to 15) */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? 0x00 : 0x40;
    address += col;
    LCD_SendCommand(0x80 | address);
}

/* Write a single character to the LCD */
void LCD_WriteChar(char ch)
{
    LCD_SendData((uint8_t)ch);
}

/* Write a null-terminated string to the LCD */
void LCD_WriteString(char *str)
{
    while (*str)
    {
        LCD_WriteChar(*str++);
    }
}

/* Create a custom character in CGRAM at a specified location (0-7) */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x07;  // Only 8 locations available (0 to 7)
    LCD_SendCommand(0x40 | (location << 3));  // Set CGRAM address for custom character
    for (int i = 0; i < 8; i++)
    {
        LCD_SendData(charmap[i]);
    }
}

/* Send a command byte to the LCD */
void LCD_SendCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);  // RS = 0 for command
    LCD_SendNibble(cmd & 0xF0);        // Send upper nibble
    LCD_SendNibble((cmd << 4) & 0xF0); // Send lower nibble
    HAL_Delay(2);
}

/* Send a data byte (character) to the LCD */
void LCD_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_SET);  // RS = 1 for data
    LCD_SendNibble(data & 0xF0);         // Send upper nibble
    LCD_SendNibble((data << 4) & 0xF0);  // Send lower nibble
    HAL_Delay(2);
}

/* Send a 4-bit nibble to the LCD data lines (D4-D7 on PB2-PB5)
   The nibble should be in the upper four bits of the input.
*/
static void LCD_SendNibble(uint8_t nibble)
{
    /* Clear PB2-PB5 */
    HAL_GPIO_WritePin(LCD_D4_Port, LCD_D4_Pin, (nibble & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_Port, LCD_D5_Pin, (nibble & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_Port, LCD_D6_Pin, (nibble & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_Port, LCD_D7_Pin, (nibble & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_ToggleEnable();
}

/* Pulse the Enable pin to latch the data into the LCD */
static void LCD_ToggleEnable(void)
{
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}
