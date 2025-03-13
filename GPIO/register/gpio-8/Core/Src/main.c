#include "stm32f4xx.h"  // Change according to your STM32 series

void GPIO_Init(void);
void delay_ms(uint32_t ms);
void toggle_led(uint8_t times);

volatile uint8_t press_count = 0;  // To count switch presses

int main(void) {
    GPIO_Init();  // Initialize GPIO

    while (1) {
        if (!(GPIOC->IDR & (1 << 13))) {  // Check if Switch (PC13 - onboard button) is pressed
            press_count++;  // Increment switch press count
            while (!(GPIOC->IDR & (1 << 13))); // Wait for switch release (debounce)

            if (press_count > 3) {
                press_count = 1;  // Reset after 3 presses
            }

            toggle_led(press_count); // Toggle LED based on press count

            delay_ms(200);  // Debounce delay
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA and GPIOC (Onboard LED & Button)
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock (Onboard LED - PA5)
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock (Onboard Button - PC13)

    // Configure PA5 as Output (Onboard LED)
    GPIOA->MODER &= ~(3 << (5 * 2)); // Clear MODER bits for PA5
    GPIOA->MODER |= (1 << (5 * 2));  // Set PA5 as Output
    GPIOA->OTYPER &= ~(1 << 5);      // Set PA5 as Push-Pull
    GPIOA->OSPEEDR &= ~(3 << (5 * 2)); // Low speed
    GPIOA->PUPDR &= ~(3 << (5 * 2)); // No pull-up, pull-down

    // Configure PC13 as Input with Pull-up (Onboard Button)
    GPIOC->MODER &= ~(3 << (13 * 2)); // Set PC13 as Input
    GPIOC->PUPDR &= ~(3 << (13 * 2)); // Clear bits
    GPIOC->PUPDR |= (1 << (13 * 2));  // Enable pull-up for PC13
}

void toggle_led(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        GPIOA->ODR ^= (1 << 5);  // Toggle LED (PA5)
        delay_ms(300);            // Small delay for visibility
        GPIOA->ODR ^= (1 << 5);  // Toggle LED (PA5)
        delay_ms(300);
    }
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) { // Approximate delay for STM32F4 @16MHz
        __NOP();
    }
}
