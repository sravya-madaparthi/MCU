#include "stm32f4xx.h"  // Use the correct header file for your STM32 series

void GPIO_Config(void);
void EXTI9_5_IRQHandler(void);

int main(void) {
    GPIO_Config();  // Initialize GPIO and Interrupts

    while (1) {
        // Main loop does nothing; LED control happens in interrupt
    }
}

void GPIO_Config(void) {
    // Enable Clocks for GPIOA and GPIOB
    RCC->AHB1ENR |= (1 << 0);  // Enable clock for GPIOA
    RCC->AHB1ENR |= (1 << 1);  // Enable clock for GPIOB
    RCC->APB2ENR |= (1 << 14); // Enable clock for SYSCFG (for EXTI)

    // Configure PA7 as output (LED)
    GPIOA->MODER |= (1 << 14);  // Set PA7 as output
    GPIOA->MODER &= ~(1 << 15); // Keep second bit 0
    GPIOA->OTYPER &= ~(1 << 7); // Push-pull output
    GPIOA->OSPEEDR |= (1 << 14); // High speed
    GPIOA->PUPDR &= ~((1 << 14) | (1 << 15)); // No pull-up/pull-down

    // Configure PB8 and PB9 as input (Switches)
    GPIOB->MODER &= ~(3 << 16); // PB8 as input
    GPIOB->MODER &= ~(3 << 18); // PB9 as input

    // Enable Pull-up for PB8 and PB9 (since buttons connect to GND)
    GPIOB->PUPDR |= (1 << 16) | (1 << 18);
    GPIOB->PUPDR &= ~((1 << 17) | (1 << 19));

    // Configure EXTI for PB8 and PB9
    SYSCFG->EXTICR[2] |= (1 << 0) | (1 << 4); // EXTI8 and EXTI9 mapped to PB

    EXTI->IMR |= (1 << 8) | (1 << 9);  // Enable EXTI8 and EXTI9
    EXTI->FTSR |= (1 << 8) | (1 << 9); // Falling edge trigger for PB8 and PB9

    // Enable EXTI9_5 interrupt in NVIC
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
}

// Interrupt handler for EXTI Line 9-5
void EXTI9_5_IRQHandler(void) {
    for (volatile int i = 0; i < 50000; i++); // Simple debounce delay

    if (EXTI->PR & (1 << 8)) { // PB8 (Switch 1) Pressed
        GPIOA->BSRR |= (1 << 7);  // Turn ON LED (PA7)
        EXTI->PR |= (1 << 8); // Clear EXTI flag for PB8
    }

    if (EXTI->PR & (1 << 9)) { // PB9 (Switch 2) Pressed
        GPIOA->BSRR |= (1 << (7 + 16));  // Turn OFF LED (PA7)
        EXTI->PR |= (1 << 9); // Clear EXTI flag for PB9
    }
}
