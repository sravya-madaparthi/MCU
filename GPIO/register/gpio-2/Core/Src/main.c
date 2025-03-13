#include "stm32f4xx.h"  // Change according to your STM32 series

void GPIO_Init(void);
void delay_ms(uint32_t ms);

int main(void) {
    GPIO_Init();  // Initialize GPIO
    uint8_t ledState = 0;  // LED state (0 = OFF, 1 = ON)

    while (1) {
        if (!(GPIOC->IDR & (1 << 9))) { // Check if button is pressed (PC9 is LOW)
            delay_ms(200);  // Simple debounce delay
            while (!(GPIOC->IDR & (1 << 9))); // Wait for button release

            ledState = !ledState; // Toggle LED state

            if (ledState) {
                GPIOA->BSRR = (1 << 8);  // Turn ON LED (Set PA8)
            } else {
                GPIOA->BSRR = (1 << (8 + 16)); // Turn OFF LED (Reset PA8)
            }
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA and GPIOC
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock

    // Configure PA8 as Output (LED)
    GPIOA->MODER &= ~(3 << (8 * 2)); // Clear MODER bits for PA8
    GPIOA->MODER |= (1 << (8 * 2));  // Set PA8 as General Purpose Output Mode
    GPIOA->OTYPER &= ~(1 << 8);      // Set PA8 as Push-Pull
    GPIOA->OSPEEDR &= ~(3 << (8 * 2)); // Low speed
    GPIOA->PUPDR &= ~(3 << (8 * 2)); // No pull-up, pull-down

    // Configure PC9 as Input with Pull-up (Button)
    GPIOC->MODER &= ~(3 << (9 * 2)); // Set PC9 as Input mode
    GPIOC->PUPDR &= ~(3 << (9 * 2)); // Clear bits
    GPIOC->PUPDR |= (1 << (9 * 2));  // Enable pull-up for PC9
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) { // Approximate delay for STM32F4 @16MHz
        __NOP();
    }
}
