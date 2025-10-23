/**
 * @file leds.c
 * @brief Control básico de LEDs conectado a GPIO
 *
 * Requiere que en config.h estén definidos:
 *   LED_METAL_GPIO_Port, LED_METAL_Pin, etc.
 */

#include "leds.h"
#include "main.h"
#include "gpio.h"

// Define identificadores (si no están en config.h)
#ifndef LED_METAL
#define LED_METAL    0
#define LED_PLASTIC  1
#define LED_GLASS    2
#define LED_PAPER    3
#define LED_CARDBOARD 4
#endif

void leds_init(void)
{
    // Los pines deberían estar configurados por MX_GPIO_Init() en main.c
    // acá no hay que hacer nada adicional salvo apagar todo.
    leds_off_all();
}

void leds_on(uint8_t led)
{
    switch (led) {
#ifdef LED_METAL_GPIO_Port
        case LED_METAL:
            HAL_GPIO_WritePin(LED_METAL_GPIO_Port, LED_METAL_Pin, GPIO_PIN_SET);
            break;
#endif
#ifdef LED_PLASTIC_GPIO_Port
        case LED_PLASTIC:
            HAL_GPIO_WritePin(LED_PLASTIC_GPIO_Port, LED_PLASTIC_Pin, GPIO_PIN_SET);
            break;
#endif
#ifdef LED_GLASS_GPIO_Port
        case LED_GLASS:
            HAL_GPIO_WritePin(LED_GLASS_GPIO_Port, LED_GLASS_Pin, GPIO_PIN_SET);
            break;
#endif
#ifdef LED_PAPER_GPIO_Port
        case LED_PAPER:
            HAL_GPIO_WritePin(LED_PAPER_GPIO_Port, LED_PAPER_Pin, GPIO_PIN_SET);
            break;
#endif
        default:
            break;
    }
}

void leds_off(uint8_t led)
{
    switch (led) {
#ifdef LED_METAL_GPIO_Port
        case LED_METAL:
            HAL_GPIO_WritePin(LED_METAL_GPIO_Port, LED_METAL_Pin, GPIO_PIN_RESET);
            break;
#endif
#ifdef LED_PLASTIC_GPIO_Port
        case LED_PLASTIC:
            HAL_GPIO_WritePin(LED_PLASTIC_GPIO_Port, LED_PLASTIC_Pin, GPIO_PIN_RESET);
            break;
#endif
#ifdef LED_GLASS_GPIO_Port
        case LED_GLASS:
            HAL_GPIO_WritePin(LED_GLASS_GPIO_Port, LED_GLASS_Pin, GPIO_PIN_RESET);
            break;
#endif
#ifdef LED_PAPER_GPIO_Port
        case LED_PAPER:
            HAL_GPIO_WritePin(LED_PAPER_GPIO_Port, LED_PAPER_Pin, GPIO_PIN_RESET);
            break;
#endif
        default:
            break;
    }
}

void leds_off_all(void)
{
#ifdef LED_METAL_GPIO_Port
    HAL_GPIO_WritePin(LED_METAL_GPIO_Port, LED_METAL_Pin, GPIO_PIN_RESET);
#endif
#ifdef LED_PLASTIC_GPIO_Port
    HAL_GPIO_WritePin(LED_PLASTIC_GPIO_Port, LED_PLASTIC_Pin, GPIO_PIN_RESET);
#endif
#ifdef LED_GLASS_GPIO_Port
    HAL_GPIO_WritePin(LED_GLASS_GPIO_Port, LED_GLASS_Pin, GPIO_PIN_RESET);
#endif
#ifdef LED_PAPER_GPIO_Port
    HAL_GPIO_WritePin(LED_PAPER_GPIO_Port, LED_PAPER_Pin, GPIO_PIN_RESET);
#endif
}
