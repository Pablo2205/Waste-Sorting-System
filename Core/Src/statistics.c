/**
 * @file statistics.c
 * @brief Implementación del módulo de estadísticas
 * @author Smart Waste Manager
 * @date 2025
 */

#include "statistics.h"
#include <stdio.h>
#include <string.h>

// Dirección en Flash para guardar estadísticas (ajustar según tu mapa de memoria)
#define STATS_FLASH_ADDR    0x08010000  // Página de Flash para datos

// ============================================================================
// INICIALIZACIÓN
// ============================================================================

void statistics_init(Statistics *stats) {
  // Intentar cargar desde Flash
  if (!statistics_load_from_flash(stats)) {
    // Si no hay datos guardados, inicializar en cero
    statistics_reset(stats);
    printf("Estadísticas inicializadas desde cero\r\n");
  } else {
    printf("Estadísticas cargadas desde Flash\r\n");
    statistics_print(stats);
  }
}

// ============================================================================
// ACTUALIZACIÓN
// ============================================================================

void statistics_update(Statistics *stats, ClassificationResult result) {
  // Incrementar total
  stats->total_clasificados++;
  
  // Incrementar contador específico
  switch(result.material) {
    case MATERIAL_METAL:
      stats->contador_metal++;
      break;
    case MATERIAL_PAPEL:
      stats->contador_papel++;
      break;
    case MATERIAL_PLASTICO:
      stats->contador_plastico++;
      break;
    case MATERIAL_VIDRIO:
      stats->contador_vidrio++;
      break;
    default:
      stats->clasificaciones_erroneas++;
      break;
  }
  
  // Actualizar promedio de confianza (promedio incremental)
  if (result.isValid) {
    float n = (float)(stats->total_clasificados - stats->clasificaciones_erroneas);
    stats->promedio_confianza = ((stats->promedio_confianza * (n - 1.0f)) + result.confidence) / n;
  }
  
  // Guardar en Flash cada 10 clasificaciones
  if (stats->total_clasificados % 10 == 0) {
    statistics_save_to_flash(stats);
  }
  
  // Log
  printf("Stats actualizado: Total=%lu, Metal=%lu, Papel=%lu, Plast=%lu, Vidrio=%lu, Avg=%.1f%%\r\n",
         stats->total_clasificados,
         stats->contador_metal,
         stats->contador_papel,
         stats->contador_plastico,
         stats->contador_vidrio,
         stats->promedio_confianza);
}

// ============================================================================
// GETTERS
// ============================================================================

float statistics_get_average_confidence(Statistics *stats) {
  return stats->promedio_confianza;
}

uint32_t statistics_get_total(Statistics *stats) {
  return stats->total_clasificados;
}

uint32_t statistics_get_material_count(Statistics *stats, MaterialType material) {
  switch(material) {
    case MATERIAL_METAL:    return stats->contador_metal;
    case MATERIAL_PAPEL:    return stats->contador_papel;
    case MATERIAL_PLASTICO: return stats->contador_plastico;
    case MATERIAL_VIDRIO:   return stats->contador_vidrio;
    default:                return 0;
  }
}

// ============================================================================
// PERSISTENCIA EN FLASH
// ============================================================================

bool statistics_save_to_flash(Statistics *stats) {
  HAL_StatusTypeDef status;
  
  // Desbloquear Flash
  HAL_FLASH_Unlock();
  
  // Borrar sector (ajustar según STM32F410RB)
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PageError;
  
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = STATS_FLASH_ADDR;
  EraseInitStruct.NbPages = 1;
  
  status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
  if (status != HAL_OK) {
    HAL_FLASH_Lock();
    printf("Error borrando Flash: %d\r\n", status);
    return false;
  }
  
  // Escribir datos (word por word)
  uint32_t *data = (uint32_t *)stats;
  uint32_t address = STATS_FLASH_ADDR;
  size_t words = sizeof(Statistics) / 4;
  
  for (size_t i = 0; i < words; i++) {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data[i]);
    if (status != HAL_OK) {
      HAL_FLASH_Lock();
      printf("Error escribiendo Flash: %d\r\n", status);
      return false;
    }
    address += 4;
  }
  
  // Bloquear Flash
  HAL_FLASH_Lock();
  
  printf("Estadísticas guardadas en Flash\r\n");
  return true;
}

bool statistics_load_from_flash(Statistics *stats) {
  // Leer desde Flash
  uint32_t *flash_addr = (uint32_t *)STATS_FLASH_ADDR;
  
  // Verificar si hay datos válidos (simple check: total > 0 y < 1 millón)
  uint32_t total = flash_addr[0];
  if (total == 0xFFFFFFFF || total == 0 || total > 1000000) {
    return false;  // No hay datos válidos
  }
  
  // Copiar datos
  memcpy(stats, (void *)STATS_FLASH_ADDR, sizeof(Statistics));
  
  return true;
}

// ============================================================================
// RESET
// ============================================================================

void statistics_reset(Statistics *stats) {
  stats->total_clasificados = 0;
  stats->contador_metal = 0;
  stats->contador_papel = 0;
  stats->contador_plastico = 0;
  stats->contador_vidrio = 0;
  stats->clasificaciones_erroneas = 0;
  stats->promedio_confianza = 0.0f;
  stats->tiempo_operacion_horas = 0;
  
  printf("Estadísticas reseteadas\r\n");
}

// ============================================================================
// DEBUG
// ============================================================================

void statistics_print(Statistics *stats) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║              ESTADÍSTICAS DEL SISTEMA                    ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Total clasificados:    %6lu                            ║\r\n", stats->total_clasificados);
  printf("║ Errores:               %6lu                            ║\r\n", stats->clasificaciones_erroneas);
  printf("║ Confianza promedio:    %6.1f%%                          ║\r\n", stats->promedio_confianza);
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Metal:                 %6lu  ", stats->contador_metal);
  if (stats->total_clasificados > 0) {
    printf("(%5.1f%%)          ║\r\n", (float)stats->contador_metal * 100.0f / stats->total_clasificados);
  } else {
    printf("(  0.0%%)          ║\r\n");
  }
  printf("║ Papel:                 %6lu  ", stats->contador_papel);
  if (stats->total_clasificados > 0) {
    printf("(%5.1f%%)          ║\r\n", (float)stats->contador_papel * 100.0f / stats->total_clasificados);
  } else {
    printf("(  0.0%%)          ║\r\n");
  }
  printf("║ Plástico:              %6lu  ", stats->contador_plastico);
  if (stats->total_clasificados > 0) {
    printf("(%5.1f%%)          ║\r\n", (float)stats->contador_plastico * 100.0f / stats->total_clasificados);
  } else {
    printf("(  0.0%%)          ║\r\n");
  }
  printf("║ Vidrio:                %6lu  ", stats->contador_vidrio);
  if (stats->total_clasificados > 0) {
    printf("(%5.1f%%)          ║\r\n", (float)stats->contador_vidrio * 100.0f / stats->total_clasificados);
  } else {
    printf("(  0.0%%)          ║\r\n");
  }
  printf("╚══════════════════════════════════════════════════════════╝\r\n\n");
}

// ============================================================================
// FIN DEL ARCHIVO
// ============================================================================

