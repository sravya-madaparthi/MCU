#include "stm32f4xx.h"  // Change this header based on your STM32 series

void delay_ms(uint32_t ms);
void GPIO_Config(void);

volatile uint8_t press_count = 0;

int main(void) {
    GPIO_Config();

    while (1) {
        if ((GPIOB->IDR & GPIO_IDR_ID8) == 0) {  // Check if PB8 is pressed (Active LOW)
            delay_ms(50);  // Debounce delay

            while ((GPIOB->IDR & GPIO_IDR_ID8) == 0);  // Wait until button is released
            delay_ms(50);  // Additional debounce delay

            press_count++;
            if (press_count > 3) press_count = 1;  // Reset to first LED after 3 presses

            // Control LEDs based on button press count
            if (press_count == 1) {
                GPIOA->ODR = (1 << 7);  // Turn on LED1 (PA7), turn off others
            } else if (press_count == 2) {
                GPIOA->ODR = (1 << 8);  // Turn on LED2 (PA8)
            } else if (press_count == 3) {
                GPIOA->ODR = (1 << 9);  // Turn on LED3 (PA9)
            }
        }
    }
}

void GPIO_Config(void) {
    // Enable clock for GPIOA and GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    // Configure PA7, PA8, PA9 as output (LEDs)
    GPIOA->MODER |= (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2));  // Output mode
    GPIOA->OTYPER &= ~((1 << 7) | (1 << 8) | (1 << 9)); // Push-pull
    GPIOA->OSPEEDR |= (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)); // High speed
    GPIOA->PUPDR &= ~((3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // No pull-up/pull-down

    // Configure PB8 as input (Switch)
    GPIOB->MODER &= ~(3 << (8 * 2));  // Input mode
    GPIOB->PUPDR |= (1 << (8 * 2));   // Enable pull-up resistor (for stable input)
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (ms * 4000); i++); // Approximate delay for STM32 (depends on clock speed)
}
