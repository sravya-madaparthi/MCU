#include "stm32f4xx.h"  // Change according to your STM32 series

void GPIO_Init(void);

int main(void) {
    GPIO_Init();  // Initialize GPIO

    while (1) {
        if (!(GPIOB->IDR & (1 << 8))) {  // Check if Switch1 (PB8) is pressed
            GPIOA->BSRR = (1 << 7);  // Turn ON LED (PA7)
        }

        if (!(GPIOB->IDR & (1 << 9))) {  // Check if Switch2 (PB9) is pressed
            GPIOA->BSRR = (1 << (7 + 16));  // Turn OFF LED (PA7)
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA and GPIOB
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock

    // Configure PA7 as Output (LED)
    GPIOA->MODER &= ~(3 << (7 * 2)); // Clear MODER bits for PA7
    GPIOA->MODER |= (1 << (7 * 2));  // Set PA7 as General Purpose Output Mode
    GPIOA->OTYPER &= ~(1 << 7);      // Set PA7 as Push-Pull
    GPIOA->OSPEEDR &= ~(3 << (7 * 2)); // Low speed
    GPIOA->PUPDR &= ~(3 << (7 * 2)); // No pull-up, pull-down

    // Configure PB8, PB9 as Input with Pull-up (Switch1 and Switch2)
    GPIOB->MODER &= ~((3 << (8 * 2)) | (3 << (9 * 2))); // Set PB8, PB9 as Input
    GPIOB->PUPDR &= ~((3 << (8 * 2)) | (3 << (9 * 2))); // Clear bits
    GPIOB->PUPDR |= ((1 << (8 * 2)) | (1 << (9 * 2)));  // Enable pull-up for PB8 and PB9
}
