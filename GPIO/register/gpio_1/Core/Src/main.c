#include "stm32f4xx.h"  // Change according to your STM32 series

void GPIO_Init(void);

int main(void) {
    GPIO_Init();  // Initialize GPIO

    while (1) {
        if (!(GPIOC->IDR & (1 << 9))) { // Read PC9, check if switch is pressed (active LOW)
            GPIOA->BSRR = (1 << 8);     // Turn ON LED (Set PA8)
        } else {
            GPIOA->BSRR = (1 << (8 + 16)); // Turn OFF LED (Reset PA8)
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA and GPIOC
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock

    // Configure PA8 as Output
    GPIOA->MODER &= ~(3 << (8 * 2)); // Clear MODER bits for PA8
    GPIOA->MODER |= (1 << (8 * 2));  // Set PA8 as General Purpose Output Mode
    GPIOA->OTYPER &= ~(1 << 8);      // Set PA8 as Push-Pull
    GPIOA->OSPEEDR &= ~(3 << (8 * 2)); // Low speed
    GPIOA->PUPDR &= ~(3 << (8 * 2)); // No pull-up, pull-down

    // Configure PC9 as Input with Pull-up
    GPIOC->MODER &= ~(3 << (9 * 2)); // Set PC9 as Input mode
    GPIOC->PUPDR &= ~(3 << (9 * 2)); // Clear bits
    GPIOC->PUPDR |= (1 << (9 * 2));  // Enable pull-up for PC9
}
