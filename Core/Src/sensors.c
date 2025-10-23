/**
 * @file sensors.c
 * @brief Implementación del módulo de sensores
 * @author Smart Waste Manager
 * @date 2025
 */

#include "sensors.h"
#include "config.h"
#include <stdio.h>
#include <math.h>

// ============================================================================
// VARIABLES PRIVADAS
// ============================================================================

static bool sensors_initialized = false;
static uint32_t ultrasonic_timeout = ULTRASONIC_TIMEOUT_US;

// ============================================================================
// INICIALIZACIÓN
// ============================================================================

void sensors_init(void) {
  if (sensors_initialized) return;
  
  // Configurar pines de sensores digitales como entrada
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // Sensor inductivo (PA0)
  GPIO_InitStruct.Pin = SENSOR_INDUCTIVO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SENSOR_INDUCTIVO_PORT, &GPIO_InitStruct);
  
  // Sensor capacitivo (PA1)
  GPIO_InitStruct.Pin = SENSOR_CAPACITIVO_PIN;
  HAL_GPIO_Init(SENSOR_CAPACITIVO_PORT, &GPIO_InitStruct);
  
  // Sensor PIR (PA2)
  GPIO_InitStruct.Pin = SENSOR_PIR_PIN;
  HAL_GPIO_Init(SENSOR_PIR_PORT, &GPIO_InitStruct);
  
  // Configurar pines de ultrasonidos
  // TRIG pins como salida
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  GPIO_InitStruct.Pin = US_METAL_TRIG_PIN;
  HAL_GPIO_Init(US_METAL_TRIG_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = US_PAPEL_TRIG_PIN;
  HAL_GPIO_Init(US_PAPEL_TRIG_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = US_PLASTICO_TRIG_PIN;
  HAL_GPIO_Init(US_PLASTICO_TRIG_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = US_VIDRIO_TRIG_PIN;
  HAL_GPIO_Init(US_VIDRIO_TRIG_PORT, &GPIO_InitStruct);
  
  // ECHO pins como entrada
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  
  GPIO_InitStruct.Pin = US_METAL_ECHO_PIN;
  HAL_GPIO_Init(US_METAL_ECHO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = US_PAPEL_ECHO_PIN;
  HAL_GPIO_Init(US_PAPEL_ECHO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = US_PLASTICO_ECHO_PIN;
  HAL_GPIO_Init(US_PLASTICO_ECHO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = US_VIDRIO_ECHO_PIN;
  HAL_GPIO_Init(US_VIDRIO_ECHO_PORT, &GPIO_InitStruct);
  
  sensors_initialized = true;
  printf("Sensores inicializados correctamente\r\n");
}

// ============================================================================
// LECTURA DE SENSORES DIGITALES
// ============================================================================

SensorDigitalData sensors_read_digital(void) {
  SensorDigitalData data = {0};
  
  if (!sensors_initialized) {
    printf("Error: Sensores no inicializados\r\n");
    return data;
  }
  
  // Leer sensores digitales
  data.inductivo = HAL_GPIO_ReadPin(SENSOR_INDUCTIVO_PORT, SENSOR_INDUCTIVO_PIN) == GPIO_PIN_SET;
  data.capacitivo = HAL_GPIO_ReadPin(SENSOR_CAPACITIVO_PORT, SENSOR_CAPACITIVO_PIN) == GPIO_PIN_SET;
  data.pir = HAL_GPIO_ReadPin(SENSOR_PIR_PORT, SENSOR_PIR_PIN) == GPIO_PIN_SET;
  
  return data;
}

// ============================================================================
// LECTURA DE SENSORES ANALÓGICOS
// ============================================================================

SensorAnalogData sensors_read_analog(uint16_t *adc_buffer) {
  SensorAnalogData data = {0};
  
  if (!sensors_initialized || adc_buffer == NULL) {
    printf("Error: ADC no inicializado o buffer NULL\r\n");
    return data;
  }
  
  // Los valores vienen del DMA en el orden configurado
  data.ldr_laser = adc_buffer[0];      // PA3 - LDR
  data.microfono = adc_buffer[1];      // PA4 - Micrófono
  data.extra1 = adc_buffer[2];         // PA5 - Sensor extra
  data.extra2 = adc_buffer[3];         // PA6 - Sensor extra
  
  return data;
}

// ============================================================================
// LECTURA DE SENSORES ULTRASÓNICOS
// ============================================================================

float sensors_read_ultrasonic(GPIO_TypeDef* trig_port, uint16_t trig_pin, 
                              GPIO_TypeDef* echo_port, uint16_t echo_pin) {
  uint32_t start_time, end_time, duration;
  
  // Enviar pulso TRIG (10us)
  HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_SET);
  HAL_Delay(1); // 10us
  HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);
  
  // Esperar ECHO alto
  start_time = HAL_GetTick();
  while (HAL_GPIO_ReadPin(echo_port, echo_pin) == GPIO_PIN_RESET) {
    if ((HAL_GetTick() - start_time) > ultrasonic_timeout) {
      return -1.0f; // Timeout
    }
  }
  
  // Medir duración del pulso ECHO
  start_time = HAL_GetTick();
  while (HAL_GPIO_ReadPin(echo_port, echo_pin) == GPIO_PIN_SET) {
    if ((HAL_GetTick() - start_time) > ultrasonic_timeout) {
      return -1.0f; // Timeout
    }
  }
  end_time = HAL_GetTick();
  
  duration = end_time - start_time;
  
  // Convertir a distancia (cm)
  // Velocidad del sonido = 343 m/s = 0.0343 cm/us
  // Distancia = (duración * velocidad) / 2
  float distance = (duration * 0.0343f) / 2.0f;
  
  return distance;
}

ContainerLevels sensors_read_container_levels(void) {
  ContainerLevels levels = {0};
  
  if (!sensors_initialized) {
    printf("Error: Sensores no inicializados\r\n");
    return levels;
  }
  
  // Leer niveles de los 4 contenedores
  levels.metal = sensors_read_ultrasonic(US_METAL_TRIG_PORT, US_METAL_TRIG_PIN,
                                        US_METAL_ECHO_PORT, US_METAL_ECHO_PIN);
  
  levels.papel = sensors_read_ultrasonic(US_PAPEL_TRIG_PORT, US_PAPEL_TRIG_PIN,
                                        US_PAPEL_ECHO_PORT, US_PAPEL_ECHO_PIN);
  
  levels.plastico = sensors_read_ultrasonic(US_PLASTICO_TRIG_PORT, US_PLASTICO_TRIG_PIN,
                                           US_PLASTICO_ECHO_PORT, US_PLASTICO_ECHO_PIN);
  
  levels.vidrio = sensors_read_ultrasonic(US_VIDRIO_TRIG_PORT, US_VIDRIO_TRIG_PIN,
                                         US_VIDRIO_ECHO_PORT, US_VIDRIO_ECHO_PIN);
  
  return levels;
}

// ============================================================================
// DETECCIÓN DE PRESENCIA
// ============================================================================

bool sensors_detect_presence(void) {
  SensorDigitalData digital = sensors_read_digital();
  
  // Detectar si hay material presente
  // Usar capacitivo como principal, PIR como confirmación
  return digital.capacitivo && digital.pir;
}

// ============================================================================
// CLASIFICACIÓN DE TRANSLUCIDEZ
// ============================================================================

TranslucencyLevel sensors_classify_translucency(uint16_t ldr_value) {
  // Convertir ADC (0-4095) a porcentaje (0-100%)
  float percentage = (float)ldr_value * 100.0f / ADC_RESOLUTION;
  
  if (percentage < 20.0f) {
    return TRANSLUCENCY_OPACO;      // Papel, Metal
  } else if (percentage < 60.0f) {
    return TRANSLUCENCY_MEDIO;      // Plástico
  } else {
    return TRANSLUCENCY_ALTO;       // Vidrio
  }
}

// ============================================================================
// CLASIFICACIÓN DE SONIDO
// ============================================================================

SoundLevel sensors_classify_sound(uint16_t mic_value) {
  // Convertir ADC (0-4095) a porcentaje (0-100%)
  float percentage = (float)mic_value * 100.0f / ADC_RESOLUTION;
  
  if (percentage < 30.0f) {
    return SOUND_BAJO;              // Papel
  } else if (percentage < 70.0f) {
    return SOUND_MEDIO;             // Plástico
  } else {
    return SOUND_ALTO;              // Metal, Vidrio
  }
}

// ============================================================================
// GENERACIÓN DE SONIDO DE PRUEBA
// ============================================================================

void sensors_generate_sound(void) {
  // Generar tono de prueba para calibración
  
  printf("Generando sonido de prueba...\r\n");
  
  // Implementación básica usando delay
  uint32_t start_time = HAL_GetTick();
  while ((HAL_GetTick() - start_time) < 200) { // 200ms
    // Toggle LED del sistema como indicador
    HAL_GPIO_TogglePin(LED_SISTEMA_PORT, LED_SISTEMA_PIN);
    HAL_Delay(50);
  }
}

// ============================================================================
// CALIBRACIÓN DE SENSORES
// ============================================================================

void sensors_calibrate(void) {
  printf("Iniciando calibración de sensores...\r\n");
  
  // Leer valores de referencia
  SensorDigitalData digital = sensors_read_digital();
  // SensorAnalogData analog = sensors_read_analog(adc_buffer);
  
  printf("Valores de calibración:\r\n");
  printf("  Inductivo: %s\r\n", digital.inductivo ? "ALTO" : "BAJO");
  printf("  Capacitivo: %s\r\n", digital.capacitivo ? "ALTO" : "BAJO");
  printf("  PIR: %s\r\n", digital.pir ? "ALTO" : "BAJO");
  // printf("  LDR: %d (%.1f%%)\r\n", analog.ldr_laser, 
  //        (float)analog.ldr_laser * 100.0f / ADC_RESOLUTION);
  // printf("  Micrófono: %d (%.1f%%)\r\n", analog.microfono,
  //        (float)analog.microfono * 100.0f / ADC_RESOLUTION);
  
  printf("Calibración completada\r\n");
}

// ============================================================================
// DIAGNÓSTICO DE SENSORES
// ============================================================================

void sensors_diagnostic(void) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                DIAGNÓSTICO DE SENSORES                   ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  
  // Sensores digitales
  SensorDigitalData digital = sensors_read_digital();
  printf("║ Digitales:                                                ║\r\n");
  printf("║   Inductivo (PA0):  %s\r\n", digital.inductivo ? "✓ DETECTADO" : "✗ NO");
  printf("║   Capacitivo (PA1): %s\r\n", digital.capacitivo ? "✓ DETECTADO" : "✗ NO");
  printf("║   PIR (PA2):        %s\r\n", digital.pir ? "✓ MOVIMIENTO" : "✗ NO");
  
  // Sensores analógicos (comentado por ahora)
  // SensorAnalogData analog = sensors_read_analog(adc_buffer);
  printf("║ Analógicos: (ADC configurado)\r\n");
  
  // Ultrasónicos
  ContainerLevels levels = sensors_read_container_levels();
  printf("║ Ultrasónicos:                                            ║\r\n");
  printf("║   Metal:             %.1f cm\r\n", levels.metal);
  printf("║   Papel:             %.1f cm\r\n", levels.papel);
  printf("║   Plástico:          %.1f cm\r\n", levels.plastico);
  printf("║   Vidrio:            %.1f cm\r\n", levels.vidrio);
  
  printf("╚══════════════════════════════════════════════════════════╝\r\n\n");
}

// ============================================================================
// FIN DEL ARCHIVO
// ============================================================================