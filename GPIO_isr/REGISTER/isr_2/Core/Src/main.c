#include "stm32f4xx.h"  // Change based on your STM32 series

void GPIO_Config(void);
void EXTI9_5_IRQHandler(void);

volatile uint8_t led_state = 0; // Variable to track LED state

int main(void) {
    GPIO_Config();  // Configure GPIO and Interrupt

    while (1) {
        // Main loop does nothing; LED control happens in interrupt
    }
}

void GPIO_Config(void) {
    // Enable Clocks for GPIOC and GPIOA
    RCC->AHB1ENR |= (1 << 2);  // Enable clock for GPIOC
    RCC->AHB1ENR |= (1 << 0);  // Enable clock for GPIOA
    RCC->APB2ENR |= (1 << 14); // Enable clock for SYSCFG (for EXTI)

    // Configure PA8 as output (LED)
    GPIOA->MODER |= (1 << 16);  // Set PA8 as output (MODER8[1:0] = 01)
    GPIOA->MODER &= ~(1 << 17); // Keep the second bit 0

    // Configure PC9 as input (Button)
    GPIOC->MODER &= ~(3 << 18); // Clear bits (MODER9[1:0] = 00 for input)

    // Enable Pull-up for PC9
    GPIOC->PUPDR |= (1 << 18);  // Set PC9 as PULL-UP (PUPDR9[1:0] = 01)
    GPIOC->PUPDR &= ~(1 << 19); // Keep the second bit 0

    // Configure EXTI for PC9
    SYSCFG->EXTICR[2] |= (2 << 4); // Map EXTI9 to GPIOC
    EXTI->IMR |= (1 << 9);         // Enable EXTI9 interrupt
    EXTI->FTSR |= (1 << 9);        // Trigger on Falling Edge (Button Press)

    // Enable EXTI9_5 interrupt in NVIC
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
}

// Interrupt handler for EXTI Line 9 (PC9)
void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & (1 << 9)) {  // Check if EXTI9 is triggered
        for (volatile int i = 0; i < 50000; i++); // Simple debounce delay

        if (!(GPIOC->IDR & (1 << 9))) { // Confirm button is still pressed
            led_state = !led_state; // Toggle LED state
            if (led_state) {
                GPIOA->BSRR |= (1 << 8);  // Turn ON LED
            } else {
                GPIOA->BSRR |= (1 << 24); // Turn OFF LED
            }
        }

        EXTI->PR |= (1 << 9);  // Clear the interrupt flag
    }
}
