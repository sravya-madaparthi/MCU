#include "stm32f4xx.h"
#include <stdio.h>

void GPIO_Init(void);
void delay_ms(uint32_t ms);
void SWV_Print(const char *str);
void ITM_SendChar(uint8_t ch);

volatile uint8_t press_count = 0;  // Stores switch press count
volatile uint8_t led_state = 0;    // 0 = OFF, 1 = ON

int main(void) {
    GPIO_Init();  // Initialize GPIO

    while (1) {
        if (!(GPIOC->IDR & (1 << 13))) {  // Check if switch (PC13) is pressed
            press_count++;
            while (!(GPIOC->IDR & (1 << 13)));  // Wait for switch release

            if (press_count % 2 == 1) {  // Odd press → LED ON
                GPIOA->ODR |= (1 << 5);
                led_state = 1;
                SWV_Print("LED ON\n");
            } else {  // Even press → LED OFF
                GPIOA->ODR &= ~(1 << 5);
                led_state = 0;
                SWV_Print("LED OFF\n");
            }

            // Print press count
            char msg[30];
            sprintf(msg, "Switch pressed: %d times\n", press_count);
            SWV_Print(msg);

            delay_ms(200);  // Debounce delay
        }
    }
}

void GPIO_Init(void) {
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA (LED)
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC (Button)

    // Configure PA5 as Output (LED)
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |= (1 << (5 * 2));
    GPIOA->OTYPER &= ~(1 << 5);
    GPIOA->OSPEEDR &= ~(3 << (5 * 2));
    GPIOA->PUPDR &= ~(3 << (5 * 2));

    // Configure PC13 as Input with Pull-up
    GPIOC->MODER &= ~(3 << (13 * 2));
    GPIOC->PUPDR &= ~(3 << (13 * 2));
    GPIOC->PUPDR |= (1 << (13 * 2));
}

void SWV_Print(const char *str) {
    while (*str) {
        ITM_SendChar(*str++);
    }
}

void ITM_SendChar(uint8_t ch) {
    if ((ITM->TCR & ITM_TCR_ITMENA_Msk) && (ITM->TER & (1UL << 0))) {
        while (ITM->PORT[0].u32 == 0);
        ITM->PORT[0].u8 = ch;
    }
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) {
        __NOP();
    }
}
