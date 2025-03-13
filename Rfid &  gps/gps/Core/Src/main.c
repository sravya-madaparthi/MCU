#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#define GPS_BUFFER_SIZE 128

uint8_t gps_buffer[GPS_BUFFER_SIZE];
char gps_data[GPS_BUFFER_SIZE];  // Buffer for storing received data

// Function to parse GPS data
void parseGPSData(char* data)
{
    char *token;
    token = strtok(data, ",");

    if (strcmp(token, "$GPGGA") == 0)  // Check for GGA sentence
    {
        strtok(NULL, ","); // Skip time
        char *latitude = strtok(NULL, ",");
        char *lat_dir = strtok(NULL, ",");
        char *longitude = strtok(NULL, ",");
        char *long_dir = strtok(NULL, ",");

        printf("Latitude: %s %s\n", latitude, lat_dir);
        printf("Longitude: %s %s\n", longitude, long_dir);
    }
}

// UART Callback for received data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        memcpy(gps_data, gps_buffer, GPS_BUFFER_SIZE);  // Copy data for parsing
        parseGPSData((char*)gps_data);  // Parse GPS data
        HAL_UART_Receive_IT(&huart1, gps_buffer, GPS_BUFFER_SIZE);  // Continue receiving
    }
}

// Main Function
int main(void)
{
    HAL_Init();

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    HAL_UART_Receive_IT(&huart1, gps_buffer, GPS_BUFFER_SIZE);  // Start UART Reception

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);  // Blink LED for status
        HAL_Delay(1000);
    }
}

// Error Handler
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
