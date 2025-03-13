#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_hal.h"  // Change this based on your STM32 series

// Function prototypes
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Clear(void);
void LCD_CreateCustomCharacters(void);

#endif  // LCD_H
