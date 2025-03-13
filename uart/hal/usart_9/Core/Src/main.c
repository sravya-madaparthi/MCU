#include "main.h"
#include "gpio.h"
#include "lcd.h"

int main(void)
{
    HAL_Init();

    MX_GPIO_Init();

    LCD_Init();  // Initialize LCD

    LCD_SetCursor(0, 0);
    LCD_SendString("Hello World!");

    while (1)
    {
        LCD_SetCursor(1, 0);
        LCD_SendString("STM32F411RE");
        HAL_Delay(1000);

        LCD_SetCursor(1, 0);
        LCD_SendString("                "); // Clear line 2
        HAL_Delay(1000);
    }
}
