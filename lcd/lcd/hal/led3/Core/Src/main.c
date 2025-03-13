#include "stm32f4xx_hal.h"

// Pin definitions for RS and EN
#define RS_PIN GPIO_PIN_0
#define EN_PIN GPIO_PIN_1
#define RS_EN_PORT GPIOC

// Data pins (D0-D7 connected to PC2-PC9)
#define DATA_PORT GPIOC

void SystemClock_Config(void);
void GPIO_Init(void);
void LCD_init(void);
void LCD_command(uint8_t cmd);
void LCD_data(uint8_t data);
void LCD_send(uint8_t value, uint8_t mode);
void LCD_string(char *str);
void LCD_setCursor(uint8_t row, uint8_t col);
void delayMs(uint32_t ms);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    LCD_init();

    LCD_setCursor(0, 0); // First row, first column
    LCD_string("Hello STM32");

    LCD_setCursor(1, 0); // Second row, first column
    LCD_string("LCD Interface");

    while (1) {}
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Initialize RS and EN as output
    GPIO_InitStruct.Pin = RS_PIN | EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RS_EN_PORT, &GPIO_InitStruct);

    // Initialize data pins (PC2-PC9)
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
                          GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(DATA_PORT, &GPIO_InitStruct);
}

void LCD_init(void) {
    delayMs(20); // Wait for LCD to power up
    LCD_command(0x38); // 8-bit, 2-line, 5x8 font
    delayMs(5);
    LCD_command(0x0C); // Display ON, cursor OFF
    delayMs(5);
    LCD_command(0x06); // Entry mode: cursor right
    delayMs(5);
    LCD_command(0x01); // Clear display
    delayMs(5);
}

void LCD_command(uint8_t cmd) {
    HAL_GPIO_WritePin(RS_EN_PORT, RS_PIN, GPIO_PIN_RESET); // RS = 0 (command)
    LCD_send(cmd, 0);
}

void LCD_data(uint8_t data) {
    HAL_GPIO_WritePin(RS_EN_PORT, RS_PIN, GPIO_PIN_SET); // RS = 1 (data)
    LCD_send(data, 1);
}

void LCD_send(uint8_t value, uint8_t mode) {
    // Send data to data pins (PC2-PC9)
    uint16_t data_pins = (value << 2); // Align with PC2-PC9
    DATA_PORT->ODR = (DATA_PORT->ODR & 0xFC03) | data_pins; // Mask other bits

    // EN pulse
    HAL_GPIO_WritePin(RS_EN_PORT, EN_PIN, GPIO_PIN_SET);
    delayMs(1);
    HAL_GPIO_WritePin(RS_EN_PORT, EN_PIN, GPIO_PIN_RESET);
    delayMs(2);
}

void LCD_string(char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

void LCD_setCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_command(address);
}

void delayMs(uint32_t ms) {
    HAL_Delay(ms); // Using HAL’s built-in delay
}

// Dummy system clock config (modify as per your STM32CubeMX settings)
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
