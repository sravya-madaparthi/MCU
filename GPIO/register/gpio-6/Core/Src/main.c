#include "stm32f4xx.h"  // Change according to your STM32 series

void GPIO_Init(void);
void delay_ms(uint32_t ms);

volatile uint8_t press_count = 0;  // To count switch presses

int main(void) {
    GPIO_Init();  // Initialize GPIO

    while (1) {
        if (!(GPIOB->IDR & (1 << 8))) {  // Check if Switch (PB8) is pressed
            press_count++;  // Increment switch press count
            while (!(GPIOB->IDR & (1 << 8))); // Wait for switch release (debounce)

            if (press_count == 1) {
                GPIOA->BSRR = (1 << 7);  // Turn ON LED1 (PA7)
            } else if (press_count == 2) {
                GPIOA->BSRR = (1 << 8);  // Turn ON LED2 (PA8)
            } else if (press_count == 3) {
                GPIOA->BSRR = (1 << 9);  // Turn ON LED3 (PA9)
            }

            delay_ms(200);  // Debounce delay

            // Turn OFF all LEDs when switch is released
            GPIOA->BSRR = (1 << (7 + 16)) | (1 << (8 + 16)) | (1 << (9 + 16));

            if (press_count >= 3) {
                press_count = 0;  // Reset count after 3 presses
            }
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA and GPIOB
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock

    // Configure PA7, PA8, PA9 as Output (LEDs)
    GPIOA->MODER &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // Clear MODER bits
    GPIOA->MODER |= ((1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));  // Set as Output
    GPIOA->OTYPER &= ~((1 << 7) | (1 << 8) | (1 << 9)); // Set as Push-Pull
    GPIOA->OSPEEDR &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // Low speed
    GPIOA->PUPDR &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // No pull-up/down

    // Configure PB8 as Input with Pull-up (Switch)
    GPIOB->MODER &= ~(3 << (8 * 2)); // Set PB8 as Input
    GPIOB->PUPDR &= ~(3 << (8 * 2)); // Clear bits
    GPIOB->PUPDR |= (1 << (8 * 2));  // Enable pull-up for PB8
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) { // Approximate delay for STM32F4 @16MHz
        __NOP();
    }
}
