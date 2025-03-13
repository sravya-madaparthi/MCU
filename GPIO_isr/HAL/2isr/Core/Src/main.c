#include "stm32f4xx_hal.h"

// Define the LED pin and button pin
#define LED_PIN    GPIO_PIN_8
#define LED_PORT   GPIOA
#define BUTTON_PIN GPIO_PIN_9
#define BUTTON_PORT GPIOC

volatile uint8_t ledState = 0;  // Variable to track LED state

// Function prototypes
void SystemClock_Config(void);
void GPIO_Config(void);
void EXTI9_5_IRQHandler(void);

int main(void) {
    // Initialize system clock and peripherals
    HAL_Init();
    SystemClock_Config();
    GPIO_Config();

    while (1) {
        // Main loop does nothing, waiting for interrupt
    }
}

// Configure System Clock (Assumes default HSE/HCLK settings)
void SystemClock_Config(void) {
    // Configure system clock if necessary (depends on your board)
}

// Configure GPIO for LED and Button with Interrupt
void GPIO_Config(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable clock for GPIOA (LED)
    __HAL_RCC_GPIOC_CLK_ENABLE();  // Enable clock for GPIOC (Button)
    __HAL_RCC_SYSCFG_CLK_ENABLE(); // Enable clock for SYSCFG (Interrupts)

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure LED pin (PA8) as output
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure Button pin (PC9) as input with interrupt
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // Pull-up resistor to prevent floating input
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

    // Enable EXTI Line 9 interrupt
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

// External Interrupt Handler for Button Press (PC9)
void EXTI9_5_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(BUTTON_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_PIN); // Clear interrupt flag

        // Toggle LED state
        ledState =! ledState;
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, (ledState) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}
