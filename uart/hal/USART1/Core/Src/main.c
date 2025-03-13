#include "stm32f4xx.h"
#include <stdint.h>

void USART2_init(void);
void USART2_write(int c);
void delayMs(int n);

/*------------------------------------------------------------------
--------- MAIN function
*-------------------------------------------------------------------
---------*/
int main(void) {
    USART2_init(); /* Initialize USART2 */

    while (1) { /* Loop forever */
        USART2_write('Y');
        USART2_write('e');
        USART2_write('s');
        delayMs(1000); /* Leave a gap between messages */
    }
}

/*------------------------------------------------------------------
--------- Initialize UART pins, Baudrate
*-------------------------------------------------------------------
---------*/
void USART2_init(void) {
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= (1 << 0);

    /* Enable USART2 clock */
    RCC->APB1ENR |= (1 << 17);

    /* Configure PA2 for USART2 TX (AF7) */
    GPIOA->MODER &= ~(3 << 4);
    GPIOA->MODER |= (2 << 4);  /* Alternate function mode */

    GPIOA->AFR[0] &= ~(0xF << 8);
    GPIOA->AFR[0] |= (7 << 8); /* Set alternate function 7 for PA2 */

    /* Configure USART2 */
    USART2->BRR = 0x0683;  /* 9600 baud @ 16 MHz */
    USART2->CR1 = (1 << 3); /* Enable Tx, 8-bit data */
    USART2->CR2 = 0x0000;   /* 1 stop bit */
    USART2->CR3 = 0x0000;   /* No flow control */
    USART2->CR1 |= (1 << 13); /* Enable USART2 */
}

/*------------------------------------------------------------------
--------- USART2 Write Character
*-------------------------------------------------------------------
---------*/
void USART2_write(int ch) {
    while (!(USART2->SR & 0x0080)); /* Wait until Tx buffer empty */
    USART2->DR = (ch & 0xFF);
}

/*------------------------------------------------------------------
--------- Simple Delay Function
*-------------------------------------------------------------------
---------*/
void delayMs(int n) {
    int i;
    for (; n > 0; n--) {
        for (i = 0; i < 2000; i++);
    }
}
