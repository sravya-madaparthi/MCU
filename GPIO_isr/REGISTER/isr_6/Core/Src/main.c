#include "stm32f4xx.h"  // Adjust based on your STM32 series

volatile uint8_t press_count = 0; // To track number of presses

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

    // Configure PA7, PA8, PA9 as output (LEDs)
    GPIOA->MODER |= (1 << 14) | (1 << 16) | (1 << 18);  // Set PA7, PA8, PA9 as output
    GPIOA->MODER &= ~((1 << 15) | (1 << 17) | (1 << 19)); // Keep second bit 0

    // Configure PB8 as input (Switch)
    GPIOB->MODER &= ~(3 << 16); // PB8 as input

    // Enable Pull-up for PB8 (Since button connects to GND when pressed)
    GPIOB->PUPDR |= (1 << 16);
    GPIOB->PUPDR &= ~(1 << 17);

    // Configure EXTI for PB8
    SYSCFG->EXTICR[2] |= (1 << 0); // EXTI8 mapped to PB

    EXTI->IMR |= (1 << 8);  // Enable EXTI8
    EXTI->RTSR |= (1 << 8); // Rising edge trigger (Button Released)
    EXTI->FTSR |= (1 << 8); // Falling edge trigger (Button Pressed)

    // Enable EXTI9_5 interrupt in NVIC
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
}

// Interrupt handler for EXTI Line 9-5
void EXTI9_5_IRQHandler(void) {
    for (volatile int i = 0; i < 50000; i++); // Simple debounce delay

    if (EXTI->PR & (1 << 8)) { // Check if interrupt was caused by PB8
        if (!(GPIOB->IDR & (1 << 8))) { // Button Press Detected (Falling Edge)
            press_count = (press_count % 3) + 1; // Cycle through 1, 2, 3

            // Turn off all LEDs first
            GPIOA->BSRR |= (1 << (7 + 16)) | (1 << (8 + 16)) | (1 << (9 + 16));

            // Turn on LED based on press count
            if (press_count == 1) {
                GPIOA->BSRR |= (1 << 7); // Turn on LED1 (PA7)
            } else if (press_count == 2) {
                GPIOA->BSRR |= (1 << 8); // Turn on LED2 (PA8)
            } else if (press_count == 3) {
                GPIOA->BSRR |= (1 << 9); // Turn on LED3 (PA9)
            }
        } else { // Button Released (Rising Edge)
            GPIOA->BSRR |= (1 << (7 + 16)) | (1 << (8 + 16)) | (1 << (9 + 16)); // Turn OFF all LEDs
        }

        EXTI->PR |= (1 << 8); // Clear EXTI8 flag
    }
}
