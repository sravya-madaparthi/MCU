#include "stm32f4xx.h"

// Define LED Pins
#define LED1_PIN    GPIO_PIN_7
#define LED2_PIN    GPIO_PIN_8
#define LED3_PIN    GPIO_PIN_9
#define LED_PORT    GPIOA

// Define Button Pins
#define BUTTON1_PIN GPIO_PIN_8  // PB8
#define BUTTON2_PIN GPIO_PIN_9  // PB9
#define BUTTON3_PIN GPIO_PIN_9  // PC9
#define BUTTON_PORT_B GPIOB
#define BUTTON_PORT_C GPIOC

// Function Prototypes
void SystemClock_Config(void);
void GPIO_Config(void);
void EXTI9_5_IRQHandler(void);
void EXTI9_IRQHandler(void);
void delay(uint32_t time);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Config();

    while (1) {
        // Main loop does nothing, waiting for interrupts
    }
}

// Configure System Clock (Modify based on board settings)
void SystemClock_Config(void) {
    // Default system clock configuration (depends on board)
}

// Configure GPIO for LEDs and Buttons with Interrupts
void GPIO_Config(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable clock for GPIOA (LEDs)
    __HAL_RCC_GPIOB_CLK_ENABLE();  // Enable clock for GPIOB (Buttons)
    __HAL_RCC_GPIOC_CLK_ENABLE();  // Enable clock for GPIOC (Buttons)
    __HAL_RCC_SYSCFG_CLK_ENABLE(); // Enable clock for SYSCFG (Interrupts)

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure LED pins (PA7, PA8, PA9) as output
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure Button1 and Button2 (PB8, PB9) as input with interrupt
    GPIO_InitStruct.Pin = BUTTON1_PIN | BUTTON2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Interrupt on button press
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // Pull-up resistor
    HAL_GPIO_Init(BUTTON_PORT_B, &GPIO_InitStruct);

    // Configure Button3 (PC9) as input with interrupt
    GPIO_InitStruct.Pin = BUTTON3_PIN;
    HAL_GPIO_Init(BUTTON_PORT_C, &GPIO_InitStruct);

    // Enable EXTI Line 9_5 Interrupt for PB8, PB9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // Enable EXTI Line 9 Interrupt for PC9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

// External Interrupt Handler for PB8, PB9
void EXTI9_5_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(BUTTON1_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON1_PIN); // Clear interrupt flag
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET); // Turn on LED1
    }

    if (__HAL_GPIO_EXTI_GET_IT(BUTTON2_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON2_PIN); // Clear interrupt flag
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET); // Turn on LED1
        HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET); // Turn on LED2
    }

    if (__HAL_GPIO_EXTI_GET_IT(BUTTON3_PIN) != RESET) {  // PC9 uses EXTI9_5
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON3_PIN); // Clear interrupt flag
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET); // Turn on LED1
        HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET); // Turn on LED2
        HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_SET); // Turn on LED3
    }
}



// Simple delay function
void delay(uint32_t time) {
    HAL_Delay(time); // HAL built-in millisecond delay
}
