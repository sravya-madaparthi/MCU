#include "stm32f4xx.h"  // Change according to your STM32 series

void GPIO_Init(void);
void delay_ms(uint32_t ms);

int main(void) {
    GPIO_Init();  // Initialize GPIO

    while (1) {
        if (!(GPIOB->IDR & (1 << 8))) {  // Check if Switch1 (PB8) is pressed
            GPIOA->BSRR = (1 << 7);       // Turn ON LED1 (PA7)
            delay_ms(200);                // Debounce delay
            GPIOA->BSRR = (1 << (7 + 16)); // Turn OFF LED1 (PA7)
        }

        if (!(GPIOB->IDR & (1 << 9))) {  // Check if Switch2 (PB9) is pressed
            GPIOA->BSRR = (1 << 8);       // Turn ON LED2 (PA8)
            delay_ms(200);                // Debounce delay
            GPIOA->BSRR = (1 << (8 + 16)); // Turn OFF LED2 (PA8)
        }

        if (!(GPIOC->IDR & (1 << 9))) {  // Check if Switch3 (PC9) is pressed
            GPIOA->BSRR = (1 << 9);       // Turn ON LED3 (PA9)
            delay_ms(200);                // Debounce delay
            GPIOA->BSRR = (1 << (9 + 16)); // Turn OFF LED3 (PA9)
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA, GPIOB, and GPIOC
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock

    // Configure PA7, PA8, PA9 as Output (LEDs)
    GPIOA->MODER &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // Clear MODER bits
    GPIOA->MODER |= ((1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));  // Set as Output
    GPIOA->OTYPER &= ~((1 << 7) | (1 << 8) | (1 << 9)); // Set as Push-Pull
    GPIOA->OSPEEDR &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // Low speed
    GPIOA->PUPDR &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // No pull-up/down

    // Configure PB8, PB9 as Input with Pull-up (Switch1 and Switch2)
    GPIOB->MODER &= ~((3 << (8 * 2)) | (3 << (9 * 2))); // Set PB8, PB9 as Input
    GPIOB->PUPDR &= ~((3 << (8 * 2)) | (3 << (9 * 2))); // Clear bits
    GPIOB->PUPDR |= ((1 << (8 * 2)) | (1 << (9 * 2)));  // Enable pull-up

    // Configure PC9 as Input with Pull-up (Switch3)
    GPIOC->MODER &= ~(3 << (9 * 2)); // Set PC9 as Input
    GPIOC->PUPDR &= ~(3 << (9 * 2)); // Clear bits
    GPIOC->PUPDR |= (1 << (9 * 2));  // Enable pull-up
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) { // Approximate delay for STM32F4 @16MHz
        __NOP();
    }
}
