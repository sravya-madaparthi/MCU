#include "stm32f4xx.h"  // Modify as per your STM32 series

void GPIO_Config(void);
void EXTI9_5_IRQHandler(void);

int main(void) {
    GPIO_Config();  // Configure GPIO and Interrupts

    while (1) {
        // Main loop does nothing; LED control happens in interrupt
    }
}

void GPIO_Config(void) {
    // Enable Clocks for GPIOA, GPIOB, and GPIOC
    RCC->AHB1ENR |= (1 << 0);  // Enable clock for GPIOA
    RCC->AHB1ENR |= (1 << 1);  // Enable clock for GPIOB
    RCC->AHB1ENR |= (1 << 2);  // Enable clock for GPIOC
    RCC->APB2ENR |= (1 << 14); // Enable clock for SYSCFG (for EXTI)

    // Configure PA7, PA8, PA9 as output (LEDs)
    GPIOA->MODER |= (1 << 14) | (1 << 16) | (1 << 18);  // Set PA7, PA8, PA9 as output
    GPIOA->MODER &= ~((1 << 15) | (1 << 17) | (1 << 19)); // Keep second bit of each pair 0

    // Configure PB8, PB9, PC9 as input (Switches)
    GPIOB->MODER &= ~(3 << 16); // PB8 as input
    GPIOB->MODER &= ~(3 << 18); // PB9 as input
    GPIOC->MODER &= ~(3 << 18); // PC9 as input

    // Enable Pull-up for PB8, PB9, PC9
    GPIOB->PUPDR |= (1 << 16) | (1 << 18); // PB8, PB9 pull-up
    GPIOB->PUPDR &= ~((1 << 17) | (1 << 19)); // Keep second bit 0
    GPIOC->PUPDR |= (1 << 18); // PC9 pull-up
    GPIOC->PUPDR &= ~(1 << 19);

    // Configure EXTI for PB8, PB9, PC9
    SYSCFG->EXTICR[2] |= (1 << 0) | (1 << 4); // EXTI8, EXTI9 mapped to PB
    SYSCFG->EXTICR[2] |= (2 << 4); // EXTI9 also mapped to PC for PC9

    EXTI->IMR |= (1 << 8) | (1 << 9); // Enable EXTI8, EXTI9
    EXTI->FTSR |= (1 << 8) | (1 << 9); // Falling edge trigger for PB8, PB9

    // Enable EXTI9_5 interrupt in NVIC
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
}

// Interrupt handler for EXTI Line 9-5
void EXTI9_5_IRQHandler(void) {
    for (volatile int i = 0; i < 50000; i++); // Simple debounce delay

    if (!(GPIOB->IDR & (1 << 8))) { // Check PB8 (Switch 1)
        GPIOA->BSRR |= (1 << 7); // Turn on LED1 (PA7)
        EXTI->PR |= (1 << 8); // Clear EXTI8 flag
    }

    if (!(GPIOB->IDR & (1 << 9))) { // Check PB9 (Switch 2)
        GPIOA->BSRR |= (1 << 7) | (1 << 8); // Turn on LED1 (PA7) and LED2 (PA8)
        EXTI->PR |= (1 << 9); // Clear EXTI9 flag
    }

    if (!(GPIOC->IDR & (1 << 9))) { // Check PC9 (Switch 3)
        GPIOA->BSRR |= (1 << 7) | (1 << 8) | (1 << 9); // Turn on LED1, LED2, and LED3
        EXTI->PR |= (1 << 9); // Clear EXTI9 flag
    }
}

