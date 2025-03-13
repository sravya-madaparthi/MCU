#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#define GPS_BUFFER_SIZE 128  // Buffer size for GPS data

uint8_t gps_buffer[GPS_BUFFER_SIZE];   // UART receive buffer
char gps_data[GPS_BUFFER_SIZE];        // Parsed GPS data storage

// Function to parse and display GPS data
void parseGPSData(char* data)
{
    char *token;
    token = strtok(data, ",");

    if (strcmp(token, "$GPGGA") == 0)  // Check for GGA sentence
    {
        strtok(NULL, ",");  // Skip time
        char *latitude = strtok(NULL, ",");
        char *lat_dir = strtok(NULL, ",");
        char *longitude = strtok(NULL, ",");
        char *long_dir = strtok(NULL, ",");

        if (latitude && longitude)
        {
            char parsed_data[100];
            snprintf(parsed_data, sizeof(parsed_data),
                     "Latitude: %s %s\nLongitude: %s %s\r\n",
                     latitude, lat_dir, longitude, long_dir);

            HAL_UART_Transmit(&huart1, (uint8_t *)parsed_data, strlen(parsed_data), HAL_MAX_DELAY);
        }
        else
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"GPS Data Incomplete\r\n", 22, HAL_MAX_DELAY);
        }
    }
    else
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"Invalid GPS Data\r\n", 19, HAL_MAX_DELAY);
    }
}

// UART Callback for received data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        memcpy(gps_data, gps_buffer, GPS_BUFFER_SIZE);  // Copy data for parsing
        HAL_UART_Transmit(&huart1, gps_buffer, strlen((char*)gps_buffer), HAL_MAX_DELAY);  // Show raw data
        parseGPSData((char*)gps_data);                   // Parse GPS data
        HAL_UART_Receive_IT(&huart1, gps_buffer, GPS_BUFFER_SIZE);  // Continue receiving
    }
}

// Main Function
int main(void)
{
    HAL_Init();

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    HAL_UART_Transmit(&huart1, (uint8_t *)"Starting GPS Receiver...\r\n", 27, HAL_MAX_DELAY);
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
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);  // Blink rapidly for error indication
        HAL_Delay(200);
    }
}
