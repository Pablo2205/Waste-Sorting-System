/**
 * @file display.h
 * @brief Módulo de visualización (LCD y LEDs)
 * @author Smart Waste Manager
 * @date 2025
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include "classifier.h"
#include "statistics.h"
#include <stdbool.h>
#include <stdint.h>

// ============================================================================
// FUNCIONES PÚBLICAS
// ============================================================================

/**
 * @brief Inicializa el módulo de visualización
 */
void display_init(void);

/**
 * @brief Muestra mensaje de bienvenida
 */
void display_show_welcome(void);

/**
 * @brief Muestra mensaje de detección
 */
void display_show_detecting(void);

/**
 * @brief Muestra resultado de clasificación
 * @param result Resultado de la clasificación
 */
void display_show_result(ClassificationResult result);

/**
 * @brief Muestra error
 * @param message Mensaje de error
 */
void display_show_error(const char* message);

/**
 * @brief Muestra estadísticas del sistema
 * @param stats Puntero a estadísticas
 */
void display_show_statistics(Statistics *stats);

/**
 * @brief Actualiza LEDs según material
 * @param material Tipo de material
 */
void display_update_leds(MaterialType material);

/**
 * @brief Apaga todos los LEDs
 */
void display_clear_leds(void);

/**
 * @brief Muestra mensaje en LCD
 * @param line1 Primera línea
 * @param line2 Segunda línea
 */
void display_lcd_message(const char* line1, const char* line2);

/**
 * @brief Limpia pantalla LCD
 */
void display_lcd_clear(void);

/**
 * @brief Muestra nivel de contenedores
 * @param levels Niveles de los contenedores
 */
void display_show_container_levels(ContainerLevels levels);

/**
 * @brief Prueba todos los LEDs
 */
void display_test_leds(void);

/**
 * @brief Prueba LCD
 */
void display_test_lcd(void);

/**
 * @brief Muestra diagnóstico completo
 */
void display_diagnostic(void);

#endif // DISPLAY_H
