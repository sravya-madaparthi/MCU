#include "stm32f4xx.h"

volatile uint8_t press_count = 0;  // To count switch presses
volatile uint8_t led1_state = 0;   // LED1 state (ON/OFF)
volatile uint8_t led2_state = 0;   // LED2 state (ON/OFF)

void GPIO_Init(void);
void delay_ms(uint32_t ms);

int main(void) {
    GPIO_Init();  // Initialize GPIO for LED1, LED2, and Switch

    while (1) {
        // Check if switch (PC13) is pressed (active low)
        if ((GPIOC->IDR & GPIO_IDR_IDR_13) == 0) {  // Switch pressed
            press_count++;  // Increment press count
            while ((GPIOC->IDR & GPIO_IDR_IDR_13) == 0); // Wait for switch release (debounce)

            if (press_count % 2 == 1) {  // 1st press: LED1 ON, LED2 OFF
                GPIOA->ODR |= GPIO_ODR_OD5;  // LED1 ON (PA5)
                GPIOB->ODR &= ~GPIO_ODR_OD7; // LED2 OFF (PB7)
                led1_state = 1;
                led2_state = 0;
            } else {  // 2nd press: LED1 ON, LED2 OFF (state remains the same)
                GPIOA->ODR |= GPIO_ODR_OD5;  // LED1 ON (PA5)
                GPIOB->ODR &= ~GPIO_ODR_OD7; // LED2 OFF (PB7)
                led1_state = 1;
                led2_state = 0;
            }

            delay_ms(200);  // Debounce delay
        }
    }
}

void GPIO_Init(void) {
    // Enable clock for GPIOA, GPIOB, and GPIOC
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

    // Configure PA5 as output (onboard LED)
    GPIOA->MODER &= ~GPIO_MODER_MODE5;  // Clear mode for PA5
    GPIOA->MODER |= GPIO_MODER_MODE5_0; // Set PA5 as output (general purpose)
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;  // Push-pull output
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5; // Low speed
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5; // No pull-up, pull-down

    // Configure PB7 as output (external LED)
    GPIOB->MODER &= ~GPIO_MODER_MODE7;  // Clear mode for PB7
    GPIOB->MODER |= GPIO_MODER_MODE7_0; // Set PB7 as output (general purpose)
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;  // Push-pull output
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7; // Low speed
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7; // No pull-up, pull-down

    // Configure PC13 as input with pull-up (Switch)
    GPIOC->MODER &= ~GPIO_MODER_MODE13;  // Set PC13 as input
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13; // Clear bits
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0; // Enable pull-up for PC13
}

// Simple delay function
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) { // Approximate delay for STM32F4 @16MHz
        __NOP();
    }
}
