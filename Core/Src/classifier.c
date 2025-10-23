/**
 * @file classifier.c
 * @brief Implementación del clasificador de materiales
 * @author Smart Waste Manager
 * @date 2025
 */

#include "classifier.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// ============================================================================
// VARIABLES PRIVADAS
// ============================================================================

static bool classifier_initialized = false;

// ============================================================================
// INICIALIZACIÓN
// ============================================================================

void classifier_init(void) {
  if (classifier_initialized) return;
  
  classifier_initialized = true;
  printf("Clasificador inicializado\r\n");
}

// ============================================================================
// CLASIFICACIÓN PRINCIPAL
// ============================================================================

ClassificationResult classifier_classify(SensorDigitalData digital, SensorAnalogData analog) {
  ClassificationResult result = {0};
  
  if (!classifier_initialized) {
    printf("Error: Clasificador no inicializado\r\n");
    result.material = MATERIAL_DESCONOCIDO;
    result.isValid = false;
    result.confidence = 0.0f;
    strcpy(result.description, "Error");
    return result;
  }
  
  // Clasificar translucidez y sonido
  TranslucencyLevel translucidez = sensors_classify_translucency(analog.ldr_laser);
  SoundLevel sonido = sensors_classify_sound(analog.microfono);
  
  // TABLA DE VERDAD - 4 MATERIALES
  if (digital.inductivo && digital.capacitivo && translucidez == TRANSLUCENCY_OPACO && sonido == SOUND_ALTO) {
    // METAL: Inductivo=1, Capacitivo=1, Opaco, Sonido Alto
    result.material = MATERIAL_METAL;
    result.confidence = classifier_calculate_confidence(digital, analog, MATERIAL_METAL);
    strcpy(result.description, "Metal");
  }
  else if (!digital.inductivo && digital.capacitivo && translucidez == TRANSLUCENCY_ALTO && sonido == SOUND_ALTO) {
    // VIDRIO: Inductivo=0, Capacitivo=1, Alto, Sonido Alto
    result.material = MATERIAL_VIDRIO;
    result.confidence = classifier_calculate_confidence(digital, analog, MATERIAL_VIDRIO);
    strcpy(result.description, "Vidrio");
  }
  else if (!digital.inductivo && digital.capacitivo && translucidez == TRANSLUCENCY_MEDIO && sonido == SOUND_MEDIO) {
    // PLÁSTICO: Inductivo=0, Capacitivo=1, Medio, Sonido Medio
    result.material = MATERIAL_PLASTICO;
    result.confidence = classifier_calculate_confidence(digital, analog, MATERIAL_PLASTICO);
    strcpy(result.description, "Plástico");
  }
  else if (!digital.inductivo && digital.capacitivo && translucidez == TRANSLUCENCY_OPACO && sonido == SOUND_BAJO) {
    // PAPEL: Inductivo=0, Capacitivo=1, Opaco, Sonido Bajo
    result.material = MATERIAL_PAPEL;
    result.confidence = classifier_calculate_confidence(digital, analog, MATERIAL_PAPEL);
    strcpy(result.description, "Papel");
  }
  else {
    // NO IDENTIFICADO
    result.material = MATERIAL_DESCONOCIDO;
    result.confidence = 0.0f;
    strcpy(result.description, "Desconocido");
  }
  
  // Validar resultado
  result.isValid = classifier_validate_result(result);
  
  return result;
}

// ============================================================================
// CÁLCULO DE CONFIANZA
// ============================================================================

float classifier_calculate_confidence(SensorDigitalData digital, SensorAnalogData analog, MaterialType material) {
  float confidence = 0.0f;
  float sensor_matches = 0.0f;
  float total_sensors = 4.0f; // inductivo, capacitivo, translucidez, sonido
  
  // Clasificar sensores
  TranslucencyLevel translucidez = sensors_classify_translucency(analog.ldr_laser);
  SoundLevel sonido = sensors_classify_sound(analog.microfono);
  
  switch (material) {
    case MATERIAL_METAL:
      // Debe ser: inductivo=1, capacitivo=1, opaco, sonido alto
      if (digital.inductivo) sensor_matches += 1.0f;
      if (digital.capacitivo) sensor_matches += 1.0f;
      if (translucidez == TRANSLUCENCY_OPACO) sensor_matches += 1.0f;
      if (sonido == SOUND_ALTO) sensor_matches += 1.0f;
      break;
      
    case MATERIAL_VIDRIO:
      // Debe ser: inductivo=0, capacitivo=1, alto, sonido alto
      if (!digital.inductivo) sensor_matches += 1.0f;
      if (digital.capacitivo) sensor_matches += 1.0f;
      if (translucidez == TRANSLUCENCY_ALTO) sensor_matches += 1.0f;
      if (sonido == SOUND_ALTO) sensor_matches += 1.0f;
      break;
      
    case MATERIAL_PLASTICO:
      // Debe ser: inductivo=0, capacitivo=1, medio, sonido medio
      if (!digital.inductivo) sensor_matches += 1.0f;
      if (digital.capacitivo) sensor_matches += 1.0f;
      if (translucidez == TRANSLUCENCY_MEDIO) sensor_matches += 1.0f;
      if (sonido == SOUND_MEDIO) sensor_matches += 1.0f;
      break;
      
    case MATERIAL_PAPEL:
      // Debe ser: inductivo=0, capacitivo=1, opaco, sonido bajo
      if (!digital.inductivo) sensor_matches += 1.0f;
      if (digital.capacitivo) sensor_matches += 1.0f;
      if (translucidez == TRANSLUCENCY_OPACO) sensor_matches += 1.0f;
      if (sonido == SOUND_BAJO) sensor_matches += 1.0f;
      break;
      
    default:
      return 0.0f;
  }
  
  // Calcular confianza base (0-100%)
  confidence = (sensor_matches / total_sensors) * 100.0f;
  
  // Bonificación por valores analógicos precisos
  float analog_bonus = 0.0f;
  
  switch (material) {
    case MATERIAL_METAL:
      // Metal debe tener valores altos en micrófono
      if (analog.microfono > 3000) analog_bonus += 5.0f;
      break;
      
    case MATERIAL_VIDRIO:
      // Vidrio debe tener valores muy altos en LDR
      if (analog.ldr_laser > 3500) analog_bonus += 5.0f;
      break;
      
    case MATERIAL_PLASTICO:
      // Plástico debe tener valores medios en ambos
      if (analog.ldr_laser > 1500 && analog.ldr_laser < 3000) analog_bonus += 3.0f;
      if (analog.microfono > 1000 && analog.microfono < 2500) analog_bonus += 3.0f;
      break;
      
    case MATERIAL_PAPEL:
      // Papel debe tener valores bajos en ambos
      if (analog.ldr_laser < 1500) analog_bonus += 3.0f;
      if (analog.microfono < 1500) analog_bonus += 3.0f;
      break;
      
    default:
      break;
  }
  
  confidence += analog_bonus;
  
  // Limitar a 0-100%
  if (confidence > 100.0f) confidence = 100.0f;
  if (confidence < 0.0f) confidence = 0.0f;
  
  return confidence;
}

// ============================================================================
// VALIDACIÓN
// ============================================================================

bool classifier_validate_result(ClassificationResult result) {
  // Verificar que el material sea válido
  if (result.material == MATERIAL_DESCONOCIDO || result.material == MATERIAL_NINGUNO) {
    return false;
  }
  
  // Verificar confianza mínima
  if (result.confidence < MIN_CONFIDENCE_THRESHOLD) {
    return false;
  }
  
  return true;
}

// ============================================================================
// UTILIDADES
// ============================================================================

const char* classifier_get_material_description(MaterialType material) {
  switch (material) {
    case MATERIAL_METAL:    return "Metal";
    case MATERIAL_PAPEL:    return "Papel";
    case MATERIAL_PLASTICO: return "Plástico";
    case MATERIAL_VIDRIO:   return "Vidrio";
    case MATERIAL_NINGUNO:  return "Ninguno";
    case MATERIAL_DESCONOCIDO: return "Desconocido";
    default:                return "Error";
  }
}

void classifier_show_truth_table(void) {
  printf("\n╔══════════════════════════════════════════════════════════╗\r\n");
  printf("║                    TABLA DE VERDAD                       ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Material  │ Inductivo │ Capacitivo │ Translucidez │ Sonido ║\r\n");
  printf("╠══════════════════════════════════════════════════════════╣\r\n");
  printf("║ Metal     │     1     │     1      │   Opaco     │ Alto   ║\r\n");
  printf("║ Vidrio    │     0     │     1      │   Alto      │ Alto   ║\r\n");
  printf("║ Plástico  │     0     │     1      │   Medio     │ Medio  ║\r\n");
  printf("║ Papel     │     0     │     1      │   Opaco     │ Bajo   ║\r\n");
  printf("╚══════════════════════════════════════════════════════════╝\r\n\n");
}

void classifier_calibrate(void) {
  printf("Iniciando calibración del clasificador...\r\n");
  
  // Mostrar tabla de verdad
  classifier_show_truth_table();
  
  printf("Umbrales de confianza:\r\n");
  printf("  Mínimo: %.1f%%\r\n", MIN_CONFIDENCE_THRESHOLD);
  printf("  Alto:   %.1f%%\r\n", HIGH_CONFIDENCE_THRESHOLD);
  
  printf("Calibración completada\r\n");
}

// ============================================================================
// FIN DEL ARCHIVO
// ============================================================================