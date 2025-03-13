#include "stm32f4xx.h"

#define RS_PIN  GPIO_PIN_0
#define E_PIN   GPIO_PIN_1
#define D4_PIN  GPIO_PIN_4
#define D5_PIN  GPIO_PIN_5
#define D6_PIN  GPIO_PIN_6
#define D7_PIN  GPIO_PIN_7
#define LCD_PORT GPIOA

void LCD_Command(uint8_t cmd);
void LCD_Init(void);
void LCD_Write_Char(char data);
void LCD_Write_String(char *str);
void LCD_Clear(void);
void LCD_Scroll_Text(char *text);

void delay_ms(uint16_t ms);

int main(void) {
    HAL_Init();

    // Enable GPIOA Clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure GPIOs for LCD
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = RS_PIN | E_PIN | D4_PIN | D5_PIN | D6_PIN | D7_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);

    LCD_Init();  // Initialize LCD
    LCD_Clear();

    char message[] = "HELLO, STM32 SCROLLING!";
    while (1) {
        LCD_Scroll_Text(message);
    }
}

// Function to send a command to LCD
void LCD_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET);  // RS = 0 (Command mode)

    // Send higher nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (cmd >> 4) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (cmd >> 5) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (cmd >> 6) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (cmd >> 7) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    delay_ms(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);
    delay_ms(1);

    // Send lower nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (cmd >> 0) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (cmd >> 1) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (cmd >> 2) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (cmd >> 3) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    delay_ms(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);
    delay_ms(1);
}

// Function to write a character
void LCD_Write_Char(char data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET);  // RS = 1 (Data mode)

    // Send higher nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 4) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 5) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 6) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 7) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    delay_ms(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);
    delay_ms(1);

    // Send lower nibble
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 0) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 1) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 2) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 3) & 1);

    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    delay_ms(1);
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);
    delay_ms(1);
}

// Function to initialize LCD
void LCD_Init(void) {
    delay_ms(50);
    LCD_Command(0x02); // 4-bit mode
    LCD_Command(0x28); // 2 lines, 5x8 font
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x06); // Entry mode
    LCD_Command(0x01); // Clear display
    delay_ms(2);
}

// Function to clear LCD
void LCD_Clear(void) {
    LCD_Command(0x01);
    delay_ms(2);
}

// Function to print a string
void LCD_Write_String(char *str) {
    while (*str) {
        LCD_Write_Char(*str++);
    }
}

// Function to scroll text
void LCD_Scroll_Text(char *text) {
    char buffer[17] = {0};
    uint8_t len = strlen(text);

    for (uint8_t i = 0; i < len + 16; i++) {
        memset(buffer, ' ', 16);
        for (uint8_t j = 0; j < 16; j++) {
            if (i + j < len) buffer[j] = text[i + j];
        }

        LCD_Clear();
        LCD_Write_String(buffer);
        delay_ms(500);
    }
}

// Function to create delay
void delay_ms(uint16_t ms) {
    HAL_Delay(ms);
}
