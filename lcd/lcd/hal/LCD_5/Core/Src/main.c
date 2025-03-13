#include "stm32f4xx_hal.h"

/* --- LCD Pin Definitions (using GPIOB) --- */
#define LCD_RS_Pin    GPIO_PIN_0
#define LCD_RS_Port   GPIOB

#define LCD_EN_Pin    GPIO_PIN_1
#define LCD_EN_Port   GPIOB

#define LCD_D4_Pin    GPIO_PIN_2
#define LCD_D4_Port   GPIOB

#define LCD_D5_Pin    GPIO_PIN_3
#define LCD_D5_Port   GPIOB

#define LCD_D6_Pin    GPIO_PIN_4
#define LCD_D6_Port   GPIOB

#define LCD_D7_Pin    GPIO_PIN_5
#define LCD_D7_Port   GPIOB

/* Function Prototypes for LCD Operations */
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char ch);
void LCD_WriteString(char *str);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
static void LCD_SendNibble(uint8_t nibble);
static void LCD_ToggleEnable(void);

/* Function Prototypes for System and GPIO Initialization */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    /* Initialize HAL and system clock */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    /* Initialize LCD in 4-bit mode and clear screen */
    LCD_Init();
    LCD_Clear();

    /* Set cursor position and display string */
    LCD_SetCursor(0, 0);
    LCD_WriteString("Hello, World!");

    while (1)
    {
        // Main loop - additional application code can be added here
    }
}

/* System Clock Configuration
   (Customize this function or generate it with STM32CubeMX)
*/
void SystemClock_Config(void)
{
    // Clock configuration code goes here.
    // This is usually auto-generated or customized for your board.
}

/* Initialize GPIO pins for LCD (using GPIOB) */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable clock for GPIOB */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure LCD pins as output */
    GPIO_InitStruct.Pin = LCD_RS_Pin | LCD_EN_Pin | LCD_D4_Pin | LCD_D5_Pin | LCD_D6_Pin | LCD_D7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* --- LCD Driver Functions --- */

/* Initialize LCD in 4-bit mode */
void LCD_Init(void)
{
    HAL_Delay(40);  // Wait for LCD to power up

    /* Initialization sequence for 4-bit mode (per HD44780 datasheet) */
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    LCD_SendNibble(0x03);
    HAL_Delay(5);
    LCD_SendNibble(0x03);
    HAL_Delay(5);
    LCD_SendNibble(0x03);
    HAL_Delay(1);
    LCD_SendNibble(0x02);  // Set to 4-bit mode

    /* Function set: 4-bit, 2-line, 5x8 font */
    LCD_SendCommand(0x28);
    /* Display on, cursor off, blink off */
    LCD_SendCommand(0x0C);
    /* Entry mode: Increment cursor */
    LCD_SendCommand(0x06);
}

/* Clear LCD display */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);  // Clear display command
    HAL_Delay(2);
}

/* Set cursor position: row (0 or 1), col (0 to 15) */
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

/* Send a command byte to the LCD */
void LCD_SendCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);  // RS low for command
    LCD_SendNibble(cmd >> 4);      // Send high nibble
    LCD_SendNibble(cmd & 0x0F);      // Send low nibble
    HAL_Delay(2);
}

/* Send a data byte (character) to the LCD */
void LCD_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_SET);    // RS high for data
    LCD_SendNibble(data >> 4);       // Send high nibble
    LCD_SendNibble(data & 0x0F);       // Send low nibble
    HAL_Delay(2);
}

/* Send a 4-bit nibble to the LCD */
static void LCD_SendNibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(LCD_D4_Port, LCD_D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_Port, LCD_D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_Port, LCD_D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_Port, LCD_D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_ToggleEnable();
}

/* Toggle the Enable pin to latch data into the LCD */
static void LCD_ToggleEnable(void)
{
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}
