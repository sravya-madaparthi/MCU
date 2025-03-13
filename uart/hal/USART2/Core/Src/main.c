
/* p4_2.c UART on USART2 Receive at 115200 Baud
*
* Receive key strokes from terminal emulator (TeraTerm) of the *
host PC to the USART2 of the Nucleo-F446RE board.
* USART2 is connected to the ST-Link virtual COM port.
* Launch Tera Term on a PC and hit any key.
* The LED program from P3_5 of Chapter 3 is used to blink the green
LED * according to the key received.
* You need to wait till the blinking stops before hitting another
key. * Received key is not echoed back to the terminal, so you will
not see * the character displayed.
*
* By default, the clock is running at 16 MHz.
* The UART2 is configured for 115200 Baud.
* PA3 - USART2 RX (AF7)
*
* This program was tested with Keil uVision v5.24a with DFP v2.11.0
*/
#include "stm32F4xx.h"
void USART2_init(void); char USART2_read(void); void LED_blink(int
value); void delayMs(int);
/*------------------------------------------------------------------
---------MAIN function
*-------------------------------------------------------------------
---------*/ int main (void) {
char c;
RCC->AHB1ENR |= 1; /* enable GPIOA clock */ GPIOA->MODER &=
~0x00000C00; /* clear pin mode */ GPIOA->MODER |= 0x00000400; /* set
pin to output mode */
USART2_init(); /* initialize USART2 */
while(1) { /* Loop forever */
c = USART2_read(); /* wait for a character received */ LED_blink(c);
/* blink the LED */
}
}
/*------------------------------------------------------------------
---------Initialize UART pins, Baudrate
*-------------------------------------------------------------------
---------*/ void USART2_init (void) {
RCC->AHB1ENR |= 1; /* Enable GPIOA clock */
RCC->APB1ENR |= 0x20000; /* Enable USART2 clock */
USART2->BRR = 0x008B; /* 115200 baud @ 16 MHz */ USART2->CR1 =
0x0004; /* enable Rx, 8-bit data */ USART2->CR2 = 0x0000; /* 1 stop
bit */ USART2->CR3 = 0x0000; /* no flow control */ USART2->CR1 |=
0x2000; /* enable USART2 */
/* Configure PA3 for USART2 RX */
GPIOA->AFR[0] &= ~0xF000;
GPIOA->AFR[0] |= 0x7000; /* alt7 for USART2 */
GPIOA->MODER &= ~0x00C0;
GPIOA->MODER |= 0x0080; /* enable alternate function for PA3 */
}
/* Read a character from USART2 */
char USART2_read(void) {
while (!(USART2->SR & 0x0020)) {} // wait until char arrives
return USART2->DR;
}
/* turn on or off the LEDs according to the value */
void LED_blink(int value) {
value %= 16; /* cap the max count at 15 */
for (; value > 0; value--) {
GPIOA->BSRR = 0x00000020; /* turn on LED */ delayMs(200);
GPIOA->BSRR = 0x00200000; /* turn off LED */ delayMs(200);
}
delayMs(800);
}
void delayMs(int n) {
int i;
for (; n > 0; n--)
for (i = 0; i < 2000; i++) ; }
