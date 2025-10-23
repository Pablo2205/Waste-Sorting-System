/**
 * @file display.c
 * @brief Implementación del módulo de visualización
 * @author Smart Waste Manager
 * @date 2025
 */

#include "display.h"
#include <stdio.h>
#include <string.h>

// ============================================================================
// VARIABLES PRIVADAS
// ============================================================================

static bool display_initialized = false;

// ============================================================================
// INICIALIZACIÓN
// ============================================================================

void display_init(void) {
  if (display_initialized) return;
  
  // Configurar pines de LEDs como salida
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  
  // Configurar todos los LEDs
  GPIO_InitStruct.Pin = LED_METAL_PIN;
  HAL_GPIO_Init(LED_METAL_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LED_PAPEL_PIN;
  HAL_GPIO_Init(LED_PAPEL_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LED_PLASTICO_PIN;
  HAL_GPIO_Init(LED_PLASTICO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LED_VIDRIO_PIN;
  HAL_GPIO_Init(LED_VIDRIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LED_ERROR_PIN;
  HAL_GPIO_Init(LED_ERROR_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LED_SISTEMA_PORT;
  HAL_GPIO_Init(LED_SISTEMA_PORT, &GPIO_InitStruct);
  
  // Apagar todos los LEDs inicialmente
  display_clear_leds();
  
  // Encender LED del sistema
  HAL_GPIO_WritePin(LED_SISTEMA_PORT, LED_SISTEMA_PIN, GPIO_PIN_SET);
  
  display_initialized = true;
  printf("Display inicializado\r\n");
}

// ============================================================================
// CONTROL DE LEDs
// ============================================================================

void display_update_leds(MaterialType material) {
  if (!display_initialized) return;
  
  // Apagar todos los LEDs primero
  display_clear_leds();
  
  // Encender LED correspondiente al material
  switch (material) {
    case MATERIAL_METAL:
      HAL_GPIO_WritePin(LED_METAL_PORT, LED_METAL_PIN, GPIO_PIN_SET);
      break;
      
    case MATERIAL_PAPEL:
      HAL_GPIO_WritePin(LED_PAPEL_PORT, LED_PAPEL_PIN, GPIO_PIN_SET);
      break;
      
    case MATERIAL_PLASTICO:
      HAL_GPIO_WritePin(LED_PLASTICO_PORT, LED_PLASTICO_PIN, GPIO_PIN_SET);
      break;
      
    case MATERIAL_VIDRIO:
      HAL_GPIO_WritePin(LED_VIDRIO_PORT, LED_VIDRIO_PIN, GPIO_PIN_SET);
      break;
      
    case MATERIAL_DESCONOCIDO:
      HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_SET);
      break;
      
    default:
      break;
  }
}

void display_clear_leds(void) {
  if (!display_initialized) return;
  
  // Apagar todos los LEDs excepto el del sistema
  HAL_GPIO_WritePin(LED_METAL_PORT, LED_METAL_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_PAPEL_PORT, LED_PAPEL_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_PLASTICO_PORT, LED_PLASTICO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_VIDRIO_PORT, LED_VIDRIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_RESET);
}

// ============================================================================
// MENSAJES LCD
// ============================================================================

void display_lcd_message(const char* line1, const char* line2) {
  // Por ahora solo imprimir por UART
  // En una implementación completa, aquí se enviaría al LCD I2C
  printf("LCD: %s | %s\r\n", line1, line2);
}

void display_lcd_clear(void) {
  printf("LCD: Limpiando pantalla\r\n");
}

// ============================================================================
// MENSAJES DEL SISTEMA
// ============================================================================

void display_show_welcome(void) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║            SMART WASTE MANAGER - STM32F410RB             ║\r\n");
  printf("║                  Sistema Iniciado                        ║\r\n");
  printf("╚══════════════════════════════════════════════════════════╝\r\n");
  
  display_lcd_message("Smart Waste", "Manager Ready");
  HAL_GPIO_WritePin(LED_SISTEMA_PORT, LED_SISTEMA_PIN, GPIO_PIN_SET);
}

void display_show_detecting(void) {
  printf("Detectando material...\r\n");
  display_lcd_message("Detectando...", "Material");
  
  // Parpadear LED del sistema
  for (int i = 0; i < 3; i++) {
    HAL_GPIO_TogglePin(LED_SISTEMA_PORT, LED_SISTEMA_PIN);
    HAL_Delay(200);
  }
}

void display_show_result(ClassificationResult result) {
  if (result.isValid) {
    printf("✓ Material identificado: %s (%.1f%% confianza)\r\n", 
           result.description, result.confidence);
    
    char conf_str[10];
    sprintf(conf_str, "%.0f%%", result.confidence);
    display_lcd_message(result.description, conf_str);
    
    // Actualizar LEDs
    display_update_leds(result.material);
  } else {
    printf("✗ No se pudo identificar el material\r\n");
    display_lcd_message("Desconocido", "Reintentar");
    display_update_leds(MATERIAL_DESCONOCIDO);
  }
}

void display_show_error(const char* message) {
  printf("Error: %s\r\n", message);
  display_lcd_message("Error", message);
  display_update_leds(MATERIAL_DESCONOCIDO);
}

void display_show_statistics(Statistics *stats) {
  if (stats == NULL) return;
  
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                    ESTADÍSTICAS                          ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Total: %lu | Metal: %lu | Papel: %lu\r\n", 
         stats->total_clasificados, stats->contador_metal, stats->contador_papel);
  printf("║ Plastico: %lu | Vidrio: %lu | Errores: %lu\r\n",
         stats->contador_plastico, stats->contador_vidrio, stats->clasificaciones_erroneas);
  printf("║ Confianza promedio: %.1f%%\r\n", stats->promedio_confianza);
  printf("╚══════════════════════════════════════════════════════════╝\r\n");
  
  // Mostrar en LCD (resumido)
  char line1[16], line2[16];
  sprintf(line1, "Total: %lu", stats->total_clasificados);
  sprintf(line2, "Conf: %.0f%%", stats->promedio_confianza);
  display_lcd_message(line1, line2);
}

void display_show_container_levels(ContainerLevels levels) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                NIVELES DE CONTENEDORES                   ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Metal:    %.1f cm\r\n", levels.metal);
  printf("║ Papel:    %.1f cm\r\n", levels.papel);
  printf("║ Plástico: %.1f cm\r\n", levels.plastico);
  printf("║ Vidrio:   %.1f cm\r\n", levels.vidrio);
  printf("╚══════════════════════════════════════════════════════════╝\r\n");
  
  // Mostrar en LCD
  char line1[16], line2[16];
  sprintf(line1, "M:%.0f P:%.0f", levels.metal, levels.papel);
  sprintf(line2, "Pl:%.0f V:%.0f", levels.plastico, levels.vidrio);
  display_lcd_message(line1, line2);
}

// ============================================================================
// PRUEBAS Y DIAGNÓSTICO
// ============================================================================

void display_test_leds(void) {
  printf("Probando LEDs...\r\n");
  
  // Probar cada LED individualmente
  HAL_GPIO_WritePin(LED_METAL_PORT, LED_METAL_PIN, GPIO_PIN_SET);
  printf("LED Metal ON\r\n");
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_METAL_PORT, LED_METAL_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(LED_PAPEL_PORT, LED_PAPEL_PIN, GPIO_PIN_SET);
  printf("LED Papel ON\r\n");
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_PAPEL_PORT, LED_PAPEL_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(LED_PLASTICO_PORT, LED_PLASTICO_PIN, GPIO_PIN_SET);
  printf("LED Plástico ON\r\n");
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_PLASTICO_PORT, LED_PLASTICO_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(LED_VIDRIO_PORT, LED_VIDRIO_PIN, GPIO_PIN_SET);
  printf("LED Vidrio ON\r\n");
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_VIDRIO_PORT, LED_VIDRIO_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_SET);
  printf("LED Error ON\r\n");
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(LED_SISTEMA_PORT, LED_SISTEMA_PIN, GPIO_PIN_SET);
  printf("LED Sistema ON\r\n");
  HAL_Delay(500);
  
  printf("Prueba de LEDs completada\r\n");
}

void display_test_lcd(void) {
  printf("Probando LCD...\r\n");
  
  display_lcd_message("Test LCD", "Linea 2");
  HAL_Delay(2000);
  
  display_lcd_message("Smart Waste", "Manager");
  HAL_Delay(2000);
  
  display_lcd_message("STM32F410RB", "Ready");
  HAL_Delay(2000);
  
  display_lcd_clear();
  printf("Prueba de LCD completada\r\n");
}

void display_diagnostic(void) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                DIAGNÓSTICO DE DISPLAY                    ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ LEDs:                                                    ║\r\n");
  printf("║   Metal (PC2):     %s\r\n", 
         HAL_GPIO_ReadPin(LED_METAL_PORT, LED_METAL_PIN) ? "ON" : "OFF");
  printf("║   Papel (PC3):     %s\r\n", 
         HAL_GPIO_ReadPin(LED_PAPEL_PORT, LED_PAPEL_PIN) ? "ON" : "OFF");
  printf("║   Plástico (PC4):  %s\r\n", 
         HAL_GPIO_ReadPin(LED_PLASTICO_PORT, LED_PLASTICO_PIN) ? "ON" : "OFF");
  printf("║   Vidrio (PC5):    %s\r\n", 
         HAL_GPIO_ReadPin(LED_VIDRIO_PORT, LED_VIDRIO_PIN) ? "ON" : "OFF");
  printf("║   Error (PC6):     %s\r\n", 
         HAL_GPIO_ReadPin(LED_ERROR_PORT, LED_ERROR_PIN) ? "ON" : "OFF");
  printf("║   Sistema (PC7):   %s\r\n", 
         HAL_GPIO_ReadPin(LED_SISTEMA_PORT, LED_SISTEMA_PIN) ? "ON" : "OFF");
  printf("║ LCD: I2C configurado (simulado por UART)\r\n");
  printf("╚══════════════════════════════════════════════════════════╝\r\n\n");
}

// ============================================================================
// FIN DEL ARCHIVO
// ============================================================================