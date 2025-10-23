/**
 * @file actuators.h
 * @brief Módulo de actuadores (servomotores)
 * @author Smart Waste Manager
 * @date 2025
 */

#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "config.h"
#include <stdbool.h>
#include <stdint.h>

// ============================================================================
// FUNCIONES PÚBLICAS
// ============================================================================

/**
 * @brief Inicializa el módulo de actuadores
 */
void actuators_init(void);

/**
 * @brief Deposita material en el contenedor correspondiente
 * @param material Tipo de material a depositar
 * @return true si se ejecutó correctamente
 */
bool actuators_deposit_material(MaterialType material);

/**
 * @brief Mueve la plataforma a una posición específica
 * @param angle Ángulo de inclinación (0-180°)
 * @return true si se movió correctamente
 */
bool actuators_move_platform(uint8_t angle);

/**
 * @brief Abre la tapa de un contenedor
 * @param material Tipo de material (contenedor)
 * @return true si se abrió correctamente
 */
bool actuators_open_container(MaterialType material);

/**
 * @brief Cierra la tapa de un contenedor
 * @param material Tipo de material (contenedor)
 * @return true si se cerró correctamente
 */
bool actuators_close_container(MaterialType material);

/**
 * @brief Ejecuta secuencia completa de depósito
 * @param material Tipo de material
 * @return true si se completó correctamente
 */
bool actuators_execute_deposit_sequence(MaterialType material);

/**
 * @brief Mueve un servo específico
 * @param servo Servo a mover (1-5)
 * @param angle Ángulo objetivo (0-180°)
 * @return true si se movió correctamente
 */
bool actuators_move_servo(uint8_t servo, uint8_t angle);

/**
 * @brief Convierte ángulo a valor PWM
 * @param angle Ángulo (0-180°)
 * @return Valor PWM (1000-2000)
 */
uint16_t actuators_angle_to_pwm(uint8_t angle);

/**
 * @brief Establece posición de reposo de todos los servos
 */
void actuators_set_rest_position(void);

/**
 * @brief Prueba todos los servos
 */
void actuators_test_all_servos(void);

/**
 * @brief Muestra estado actual de los servos
 */
void actuators_show_status(void);

#endif // ACTUATORS_H
