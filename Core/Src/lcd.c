/**
 * @file lcd.c
 * @brief Driver mínimo de LCD. Si no tenés LCD implementado, lo
 *        emulamos por UART (debug). Ajusta para tu controlador (HD44780, I2C, SPI...).
 */

#include "lcd.h"
#include "usart.h"
#include <stdio.h>

// Usamos UART1 para debug si no hay pantalla física
extern UART_HandleTypeDef huart1;

void lcd_init(void)
{
    // Si tu pantalla usa I2C/SPI/HAL, inicializar aquí.
    // Por ahora mostramos mensaje por UART:
    char *msg = "[LCD] init";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void lcd_clear(void)
{
    char *msg = "[LCD] clear";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    // Implementar según driver físico si es necesario.
    (void)row;
    (void)col;
}

void lcd_print(const char *text)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "[LCD] %s", text);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
}
