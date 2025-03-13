#include "stm32f4xx_hal.h"
#include "lcd.h"  // Include LCD driver header

// Define Custom Characters (5x8 pixel patterns)
uint8_t alpha[8] = { 0x04, 0x0A, 0x0A, 0x0E, 0x0A, 0x1F, 0x11, 0x00 };  // α (Alpha)
uint8_t beta[8]  = { 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x1F, 0x0E, 0x00 };  // β (Beta)
uint8_t pi[8]    = { 0x1F, 0x15, 0x15, 0x15, 0x15, 0x11, 0x11, 0x00 };  // π (Pi)
uint8_t ohm[8]   = { 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x0A, 0x1F, 0x00 };  // Ω (Ohm)

// Function to Create Custom Characters
void LCD_CreateCustomCharacters() {
    LCD_SendCommand(0x40);  // Set CGRAM address to 0x40

    for (uint8_t i = 0; i < 8; i++) LCD_SendData(alpha[i]);  // Store α in CGRAM
    for (uint8_t i = 0; i < 8; i++) LCD_SendData(beta[i]);   // Store β in CGRAM
    for (uint8_t i = 0; i < 8; i++) LCD_SendData(pi[i]);     // Store π in CGRAM
    for (uint8_t i = 0; i < 8; i++) LCD_SendData(ohm[i]);    // Store Ω in CGRAM
}

// Main Function
int main(void) {
    HAL_Init();   // Initialize HAL
    LCD_Init();   // Initialize LCD

    LCD_CreateCustomCharacters(); // Load custom characters into CGRAM

    LCD_SetCursor(0, 0);  // Set cursor to first line
    LCD_SendString("Special Chars:");

    // Display Custom Characters
    LCD_SetCursor(1, 0);  // Move to second line
    LCD_SendData(0);  // Display α (Alpha)
    LCD_SendString(" ");
    LCD_SendData(1);  // Display β (Beta)
    LCD_SendString(" ");
    LCD_SendData(2);  // Display π (Pi)
    LCD_SendString(" ");
    LCD_SendData(3);  // Display Ω (Ohm)

    while (1);
}
