/**
 * @file actuators.c
 * @brief Implementación del módulo de actuadores
 * @author Smart Waste Manager
 * @date 2025
 */

#include "actuators.h"
#include <stdio.h>

// ============================================================================
// VARIABLES PRIVADAS
// ============================================================================

static bool actuators_initialized = false;

// Posiciones actuales de los servos
static uint8_t platform_angle = SERVO_PLAT_HORIZONTAL;
static uint8_t metal_cover_angle = SERVO_TAPA_CERRADA;
static uint8_t papel_cover_angle = SERVO_TAPA_CERRADA;
static uint8_t plastico_cover_angle = SERVO_TAPA_CERRADA;
static uint8_t vidrio_cover_angle = SERVO_TAPA_CERRADA;

// ============================================================================
// INICIALIZACIÓN
// ============================================================================

void actuators_init(void) {
  if (actuators_initialized) return;
  
  // Establecer posición de reposo
  actuators_set_rest_position();
  
  actuators_initialized = true;
  printf("Actuadores inicializados\r\n");
}

// ============================================================================
// CONVERSIÓN ÁNGULO A PWM
// ============================================================================

uint16_t actuators_angle_to_pwm(uint8_t angle) {
  // Limitar ángulo a 0-180°
  if (angle > 180) angle = 180;
  
  // Convertir ángulo a pulso PWM (1000-2000 us)
  // 0° = 1000 us, 90° = 1500 us, 180° = 2000 us
  uint16_t pulse = SERVO_MIN_PULSE + ((angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180);
  
  return pulse;
}

// ============================================================================
// MOVIMIENTO DE SERVOS
// ============================================================================

bool actuators_move_servo(uint8_t servo, uint8_t angle) {
  if (!actuators_initialized) {
    printf("Error: Actuadores no inicializados\r\n");
    return false;
  }
  
  uint16_t pwm_value = actuators_angle_to_pwm(angle);
  
  switch (servo) {
    case 1: // Plataforma (TIM1_CH1)
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_value);
      platform_angle = angle;
      break;
      
    case 2: // Metal (TIM1_CH2)
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_value);
      metal_cover_angle = angle;
      break;
      
    case 3: // Papel (TIM1_CH3)
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_value);
      papel_cover_angle = angle;
      break;
      
    case 4: // Plástico (TIM5_CH1) - CORREGIDO
      __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, pwm_value);
      plastico_cover_angle = angle;
      break;
      
    case 5: // Vidrio (TIM5_CH2) - CORREGIDO
      __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pwm_value);
      vidrio_cover_angle = angle;
      break;
      
    default:
      printf("Error: Servo %d no válido\r\n", servo);
      return false;
  }
  
  return true;
}

// ============================================================================
// MOVIMIENTO DE PLATAFORMA
// ============================================================================

bool actuators_move_platform(uint8_t angle) {
  printf("Moviendo plataforma a %d°\r\n", angle);
  
  bool success = actuators_move_servo(1, angle);
  if (success) {
    HAL_Delay(SERVO_DELAY_TILT);
  }
  
  return success;
}

// ============================================================================
// CONTROL DE CONTENEDORES
// ============================================================================

bool actuators_open_container(MaterialType material) {
  printf("Abriendo contenedor de %s\r\n", classifier_get_material_description(material));
  
  uint8_t servo = 0;
  switch (material) {
    case MATERIAL_METAL:    servo = 2; break;
    case MATERIAL_PAPEL:    servo = 3; break;
    case MATERIAL_PLASTICO: servo = 4; break;
    case MATERIAL_VIDRIO:   servo = 5; break;
    default:
      printf("Error: Material no válido para contenedor\r\n");
      return false;
  }
  
  bool success = actuators_move_servo(servo, SERVO_TAPA_ABIERTA);
  if (success) {
    HAL_Delay(SERVO_DELAY_OPEN);
  }
  
  return success;
}

bool actuators_close_container(MaterialType material) {
  printf("Cerrando contenedor de %s\r\n", classifier_get_material_description(material));
  
  uint8_t servo = 0;
  switch (material) {
    case MATERIAL_METAL:    servo = 2; break;
    case MATERIAL_PAPEL:    servo = 3; break;
    case MATERIAL_PLASTICO: servo = 4; break;
    case MATERIAL_VIDRIO:   servo = 5; break;
    default:
      printf("Error: Material no válido para contenedor\r\n");
      return false;
  }
  
  bool success = actuators_move_servo(servo, SERVO_TAPA_CERRADA);
  if (success) {
    HAL_Delay(SERVO_DELAY_CLOSE);
  }
  
  return success;
}

// ============================================================================
// SECUENCIA COMPLETA DE DEPÓSITO
// ============================================================================

bool actuators_execute_deposit_sequence(MaterialType material) {
  printf("Iniciando secuencia de depósito para %s\r\n", classifier_get_material_description(material));
  
  // 1. Mover plataforma a posición del material
  uint8_t platform_angle = 0;
  switch (material) {
    case MATERIAL_METAL:    platform_angle = SERVO_PLAT_METAL; break;
    case MATERIAL_PAPEL:    platform_angle = SERVO_PLAT_PAPEL; break;
    case MATERIAL_PLASTICO: platform_angle = SERVO_PLAT_PLASTICO; break;
    case MATERIAL_VIDRIO:   platform_angle = SERVO_PLAT_VIDRIO; break;
    default:
      printf("Error: Material no válido\r\n");
      return false;
  }
  
  if (!actuators_move_platform(platform_angle)) {
    return false;
  }
  
  // 2. Abrir tapa del contenedor
  if (!actuators_open_container(material)) {
    return false;
  }
  
  // 3. Esperar a que caiga el material
  printf("Esperando caída del material...\r\n");
  HAL_Delay(SERVO_DELAY_DROP);
  
  // 4. Cerrar tapa del contenedor
  if (!actuators_close_container(material)) {
    return false;
  }
  
  // 5. Regresar plataforma a posición horizontal
  if (!actuators_move_platform(SERVO_PLAT_HORIZONTAL)) {
    return false;
  }
  
  printf("Secuencia de depósito completada\r\n");
  return true;
}

// ============================================================================
// DEPÓSITO DE MATERIAL (FUNCIÓN PRINCIPAL)
// ============================================================================

bool actuators_deposit_material(MaterialType material) {
  if (!actuators_initialized) {
    printf("Error: Actuadores no inicializados\r\n");
    return false;
  }
  
  // Verificar que el material sea válido
  if (material == MATERIAL_DESCONOCIDO || material == MATERIAL_NINGUNO) {
    printf("Error: Material no válido para depósito\r\n");
    return false;
  }
  
  // Ejecutar secuencia completa
  return actuators_execute_deposit_sequence(material);
}

// ============================================================================
// POSICIÓN DE REPOSO
// ============================================================================

void actuators_set_rest_position(void) {
  printf("Estableciendo posición de reposo...\r\n");
  
  // Plataforma horizontal
  actuators_move_servo(1, SERVO_PLAT_HORIZONTAL);
  
  // Todas las tapas cerradas
  actuators_move_servo(2, SERVO_TAPA_CERRADA); // Metal
  actuators_move_servo(3, SERVO_TAPA_CERRADA); // Papel
  actuators_move_servo(4, SERVO_TAPA_CERRADA); // Plástico
  actuators_move_servo(5, SERVO_TAPA_CERRADA); // Vidrio
  
  HAL_Delay(1000); // Esperar a que se posicionen
  printf("Posición de reposo establecida\r\n");
}

// ============================================================================
// PRUEBAS Y DIAGNÓSTICO
// ============================================================================

void actuators_test_all_servos(void) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                PRUEBA DE SERVOMOTORES                    ║\r\n");
  printf("╚══════════════════════════════════════════════════════════╝\r\n");
  
  // Probar cada servo individualmente
  for (uint8_t servo = 1; servo <= 5; servo++) {
    printf("Probando servo %d...\r\n", servo);
    
    // Mover a 0°
    actuators_move_servo(servo, 0);
    HAL_Delay(1000);
    
    // Mover a 90°
    actuators_move_servo(servo, 90);
    HAL_Delay(1000);
    
    // Mover a 180°
    actuators_move_servo(servo, 180);
    HAL_Delay(1000);
    
    // Regresar a 90°
    actuators_move_servo(servo, 90);
    HAL_Delay(500);
  }
  
  // Regresar a posición de reposo
  actuators_set_rest_position();
  
  printf("Prueba de servos completada\r\n");
}

void actuators_show_status(void) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                ESTADO DE ACTUADORES                      ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Servo 1 (Plataforma): %3d°\r\n", platform_angle);
  printf("║ Servo 2 (Metal):      %3d°\r\n", metal_cover_angle);
  printf("║ Servo 3 (Papel):      %3d°\r\n", papel_cover_angle);
  printf("║ Servo 4 (Plástico):   %3d°\r\n", plastico_cover_angle);
  printf("║ Servo 5 (Vidrio):     %3d°\r\n", vidrio_cover_angle);
  printf("╚══════════════════════════════════════════════════════════╝\r\n\n");
}

// ============================================================================
// FIN DEL ARCHIVO
// ============================================================================