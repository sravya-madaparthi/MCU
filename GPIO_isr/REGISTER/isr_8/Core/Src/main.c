#include "stm32f4xx.h"  // Change this based on your STM32 series

void GPIO_Config(void);
void EXTI_Config(void);
void LED_Toggle(uint8_t count);
void delay_ms(uint32_t ms);

volatile uint8_t press_count = 0;  // Tracks button presses

int main(void) {
    GPIO_Config();
    EXTI_Config();

    while (1) {
        // Main loop does nothing, work is done in ISR
    }
}

void GPIO_Config(void) {
    // Enable clock for GPIOA (LED) and GPIOC (Button)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    // Configure PA5 as output (Onboard LED)
    GPIOA->MODER |= (1 << (5 * 2));  // Output mode
    GPIOA->OTYPER &= ~(1 << 5);  // Push-pull
    GPIOA->OSPEEDR |= (3 << (5 * 2));  // High speed
    GPIOA->PUPDR &= ~(3 << (5 * 2));  // No pull-up/pull-down

    // Configure PC13 as input (Onboard Button)
    GPIOC->MODER &= ~(3 << (13 * 2));  // Input mode
    GPIOC->PUPDR |= (1 << (13 * 2));   // Enable pull-up resistor
}

void EXTI_Config(void) {
    // Enable SYSCFG Clock (Required for External Interrupts)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure EXTI13 (PC13) for falling edge trigger
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;  // Select PC13 for EXTI13
    EXTI->IMR |= EXTI_IMR_IM13;  // Enable interrupt for line 13
    EXTI->FTSR |= EXTI_FTSR_TR13;  // Trigger on falling edge

    // Enable EXTI13 interrupt in NVIC
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 2);  // Set priority
}

// Interrupt Service Routine for EXTI Line 13
void EXTI15_10_IRQn_Handler(void) {
    if (EXTI->PR & EXTI_PR_PR13) {  // Check if EXTI13 is triggered
        delay_ms(50);  // Debounce delay

        if ((GPIOC->IDR & GPIO_IDR_ID13) == 0) {  // Button still pressed
            press_count++;
            if (press_count > 3) press_count = 1;  // Reset after 3 presses

            LED_Toggle(press_count);  // Toggle LED based on press count
        }

        EXTI->PR |= EXTI_PR_PR13;  // Clear the interrupt flag
    }
}

void LED_Toggle(uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        GPIOA->ODR ^= (1 << 5);  // Toggle LED
        delay_ms(500);  // Delay for visibility
        GPIOA->ODR ^= (1 << 5);  // Toggle back
        delay_ms(500);
    }
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (ms * 4000); i++); // Approximate delay
}
