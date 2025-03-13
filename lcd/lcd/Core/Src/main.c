#include "stm32f4xx.h"

#define RS 0x20  // PB5 mask for RS
#define RW 0x40  // PB6 mask for RW
#define EN 0x80  // PB7 mask for EN

void delayMs(int n);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_string(char str[]);
void LCD_init(void);
void PORTS_init(void);
void LCD_enablePulse(void);

int main(void) {
    LCD_init();  // Initialize LCD
    while (1) {
        LCD_command(0x01);  // Clear display
        delayMs(2);         // Wait after clear
        LCD_string("WELCOME ALL");  // Display string
        delayMs(1000);      // Wait before repeating
    }
}

void LCD_string(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        LCD_data(str[i]);
    }
}

void LCD_init(void) {
    PORTS_init();      // Initialize GPIO ports
    delayMs(50);       // Wait after power on

    // Initialization sequence
    LCD_command(0x30); delayMs(5);
    LCD_command(0x30); delayMs(5);
    LCD_command(0x30); delayMs(5);

    LCD_command(0x38);  // Function set: 8-bit, 2-line, 5x7 font
    delayMs(5);
    LCD_command(0x06);  // Entry mode: cursor moves right
    delayMs(5);
    LCD_command(0x01);  // Clear display
    delayMs(2);
    LCD_command(0x0C);  // Display ON, cursor OFF, blink OFF
    delayMs(5);
}

void PORTS_init(void) {
    RCC->AHB1ENR |= 0x06;         // Enable GPIOB and GPIOC clocks

    // Configure GPIOB (PB5, PB6, PB7) as output for RS, RW, EN
    GPIOB->MODER &= ~0x0000FC00;  // Clear mode for PB5-PB7
    GPIOB->MODER |= 0x00005400;   // Set PB5, PB6, PB7 as output
    GPIOB->BSRR = (RW | EN) << 16; // RW=0, EN=0

    // Configure GPIOC (PC0-PC7) as output for D0-D7
    GPIOC->MODER &= ~0x0000FFFF;  // Clear PC0-PC7 modes
    GPIOC->MODER |= 0x00005555;   // Set PC0-PC7 as output
}

void LCD_command(unsigned char command) {
    GPIOB->BSRR = (RS | RW) << 16; // RS=0, RW=0 for command
    GPIOC->ODR = command;          // Put command on data bus
    LCD_enablePulse();             // Pulse EN

    if (command == 0x01 || command == 0x02)
        delayMs(2);                // Commands requiring longer delay
    else
        delayMs(1);                // Short delay for other commands
}

void LCD_data(char data) {
    GPIOB->BSRR = RS;             // RS=1 for data
    GPIOB->BSRR = RW << 16;       // RW=0 for write
    GPIOC->ODR = data;            // Put data on data bus
    LCD_enablePulse();            // Pulse EN
    delayMs(1);                   // Short delay after data write
}

void LCD_enablePulse(void) {
    GPIOB->BSRR = EN;             // EN=1
    delayMs(1);                   // Short delay for latch
    GPIOB->BSRR = EN << 16;       // EN=0
}

void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++);  // Approximate 1ms delay
}
