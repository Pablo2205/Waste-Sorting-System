/**
 * @file classifier.h
 * @brief Módulo clasificador de materiales
 * @author Smart Waste Manager
 * @date 2025
 */

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "config.h"
#include "sensors.h"
#include <stdbool.h>
#include <stdint.h>

// ============================================================================
// TIPOS DE DATOS
// ============================================================================

/**
 * @brief Resultado de la clasificación
 */
typedef struct {
  MaterialType material;        // Tipo de material identificado
  bool isValid;                // true si la clasificación es válida
  float confidence;            // Confianza (0-100%)
  char description[20];        // Descripción del material
} ClassificationResult;

// ============================================================================
// CONSTANTES DE CLASIFICACIÓN
// ============================================================================

#define MIN_CONFIDENCE_THRESHOLD    60.0f  // Mínimo 60% de confianza
#define HIGH_CONFIDENCE_THRESHOLD   80.0f  // Alta confianza 80%+

// ============================================================================
// FUNCIONES PÚBLICAS
// ============================================================================

/**
 * @brief Inicializa el módulo clasificador
 */
void classifier_init(void);

/**
 * @brief Clasifica material basado en sensores
 * @param digital Datos de sensores digitales
 * @param analog Datos de sensores analógicos
 * @return Resultado de la clasificación
 */
ClassificationResult classifier_classify(SensorDigitalData digital, SensorAnalogData analog);

/**
 * @brief Calcula confianza de clasificación
 * @param digital Datos de sensores digitales
 * @param analog Datos de sensores analógicos
 * @param material Material a evaluar
 * @return Confianza (0-100%)
 */
float classifier_calculate_confidence(SensorDigitalData digital, SensorAnalogData analog, MaterialType material);

/**
 * @brief Valida resultado de clasificación
 * @param result Resultado a validar
 * @return true si es válido
 */
bool classifier_validate_result(ClassificationResult result);

/**
 * @brief Obtiene descripción del material
 * @param material Tipo de material
 * @return Descripción en string
 */
const char* classifier_get_material_description(MaterialType material);

/**
 * @brief Muestra tabla de verdad para debug
 */
void classifier_show_truth_table(void);

/**
 * @brief Calibra umbrales de clasificación
 */
void classifier_calibrate(void);

#endif // CLASSIFIER_H
