/**
 * @file sensors.h
 * @brief Módulo de sensores del sistema
 * @author Smart Waste Manager
 * @date 2025
 */

#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include <stdbool.h>
#include <stdint.h>

// ============================================================================
// TIPOS DE DATOS
// ============================================================================

// Los tipos TranslucencyLevel y SoundLevel están definidos en config.h

// ============================================================================
// FUNCIONES PÚBLICAS
// ============================================================================

/**
 * @brief Inicializa el módulo de sensores
 */
void sensors_init(void);

/**
 * @brief Lee sensores digitales (inductivo, capacitivo, PIR)
 * @return Estructura con valores de sensores digitales
 */
SensorDigitalData sensors_read_digital(void);

/**
 * @brief Lee sensores analógicos (LDR, micrófono, extras)
 * @param adc_buffer Buffer con valores ADC del DMA
 * @return Estructura con valores de sensores analógicos
 */
SensorAnalogData sensors_read_analog(uint16_t *adc_buffer);

/**
 * @brief Lee un sensor ultrasónico específico
 * @param trig_port Puerto del pin TRIG
 * @param trig_pin Pin TRIG
 * @param echo_port Puerto del pin ECHO
 * @param echo_pin Pin ECHO
 * @return Distancia en cm (-1 si error)
 */
float sensors_read_ultrasonic(GPIO_TypeDef* trig_port, uint16_t trig_pin, 
                              GPIO_TypeDef* echo_port, uint16_t echo_pin);

/**
 * @brief Lee niveles de todos los contenedores
 * @return Estructura con niveles de los 4 contenedores
 */
ContainerLevels sensors_read_container_levels(void);

/**
 * @brief Detecta si hay material presente
 * @return true si detecta presencia
 */
bool sensors_detect_presence(void);

/**
 * @brief Clasifica nivel de translucidez basado en LDR
 * @param ldr_value Valor del LDR (0-4095)
 * @return Nivel de translucidez
 */
TranslucencyLevel sensors_classify_translucency(uint16_t ldr_value);

/**
 * @brief Clasifica nivel de sonido basado en micrófono
 * @param mic_value Valor del micrófono (0-4095)
 * @return Nivel de sonido
 */
SoundLevel sensors_classify_sound(uint16_t mic_value);

/**
 * @brief Genera sonido de prueba para calibración
 */
void sensors_generate_sound(void);

/**
 * @brief Calibra sensores (muestra valores de referencia)
 */
void sensors_calibrate(void);

/**
 * @brief Muestra diagnóstico completo de sensores
 */
void sensors_diagnostic(void);

#endif // SENSORS_H
