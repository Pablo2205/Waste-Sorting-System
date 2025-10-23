/**
 * @file statistics.h
 * @brief Módulo de estadísticas del sistema
 * @author Smart Waste Manager
 * @date 2025
 */

#ifndef STATISTICS_H
#define STATISTICS_H

#include "config.h"
#include "classifier.h"

// ============================================================================
// FUNCIONES PÚBLICAS
// ============================================================================

/**
 * @brief Inicializa el módulo de estadísticas
 * @param stats Puntero a estructura de estadísticas
 */
void statistics_init(Statistics *stats);

/**
 * @brief Actualiza estadísticas con nueva clasificación
 * @param stats Puntero a estructura de estadísticas
 * @param result Resultado de clasificación
 */
void statistics_update(Statistics *stats, ClassificationResult result);

/**
 * @brief Obtiene el promedio de confianza
 * @param stats Puntero a estructura de estadísticas
 * @return Promedio de confianza (0-100%)
 */
float statistics_get_average_confidence(Statistics *stats);

/**
 * @brief Obtiene el total de materiales clasificados
 * @param stats Puntero a estructura de estadísticas
 * @return Total de clasificaciones
 */
uint32_t statistics_get_total(Statistics *stats);

/**
 * @brief Obtiene contador de un material específico
 * @param stats Puntero a estructura de estadísticas
 * @param material Tipo de material
 * @return Cantidad de ese material
 */
uint32_t statistics_get_material_count(Statistics *stats, MaterialType material);

/**
 * @brief Guarda estadísticas en Flash
 * @param stats Puntero a estructura de estadísticas
 * @return true si se guardó correctamente
 */
bool statistics_save_to_flash(Statistics *stats);

/**
 * @brief Carga estadísticas desde Flash
 * @param stats Puntero a estructura de estadísticas
 * @return true si se cargó correctamente
 */
bool statistics_load_from_flash(Statistics *stats);

/**
 * @brief Resetea todas las estadísticas
 * @param stats Puntero a estructura de estadísticas
 */
void statistics_reset(Statistics *stats);

/**
 * @brief Imprime estadísticas por UART
 * @param stats Puntero a estructura de estadísticas
 */
void statistics_print(Statistics *stats);

#endif // STATISTICS_H

