/**
 * @file config.h
 * @brief Configuración del sistema - Versión STM32F410RB
 * @author Smart Waste Manager
 * @date 2025
 *
 * Placa: Nucleo STM32F410RB
 * Materiales: Metal, Papel, Plástico, Vidrio (4 tipos)
 * Servos: 5 (1 plataforma + 4 tapas)
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

// ============================================================================
// CONFIGURACIÓN DE PINES GPIO - STM32F410RB
// ============================================================================

// Sensores Digitales
#define SENSOR_INDUCTIVO_PORT       GPIOA
#define SENSOR_INDUCTIVO_PIN        GPIO_PIN_0

#define SENSOR_CAPACITIVO_PORT      GPIOA
#define SENSOR_CAPACITIVO_PIN       GPIO_PIN_1

#define SENSOR_PIR_PORT             GPIOA
#define SENSOR_PIR_PIN              GPIO_PIN_2

// LEDs Indicadores
#define LED_METAL_PORT              GPIOC
#define LED_METAL_PIN               GPIO_PIN_2

#define LED_PAPEL_PORT              GPIOC
#define LED_PAPEL_PIN               GPIO_PIN_3

#define LED_PLASTICO_PORT           GPIOC
#define LED_PLASTICO_PIN            GPIO_PIN_4

#define LED_VIDRIO_PORT             GPIOC
#define LED_VIDRIO_PIN              GPIO_PIN_5

#define LED_ERROR_PORT              GPIOC
#define LED_ERROR_PIN               GPIO_PIN_6

#define LED_SISTEMA_PORT            GPIOC
#define LED_SISTEMA_PIN             GPIO_PIN_7

// Sensores Ultrasónicos (TRIG)
#define US_METAL_TRIG_PORT          GPIOB
#define US_METAL_TRIG_PIN           GPIO_PIN_0

#define US_PAPEL_TRIG_PORT          GPIOB
#define US_PAPEL_TRIG_PIN           GPIO_PIN_10

#define US_PLASTICO_TRIG_PORT       GPIOB
#define US_PLASTICO_TRIG_PIN        GPIO_PIN_12

#define US_VIDRIO_TRIG_PORT         GPIOB
#define US_VIDRIO_TRIG_PIN          GPIO_PIN_14

// Sensores Ultrasónicos (ECHO)
#define US_METAL_ECHO_PORT          GPIOB
#define US_METAL_ECHO_PIN           GPIO_PIN_1

#define US_PAPEL_ECHO_PORT          GPIOB
#define US_PAPEL_ECHO_PIN           GPIO_PIN_11

#define US_PLASTICO_ECHO_PORT       GPIOB
#define US_PLASTICO_ECHO_PIN        GPIO_PIN_13

#define US_VIDRIO_ECHO_PORT         GPIOB
#define US_VIDRIO_ECHO_PIN          GPIO_PIN_15

// ============================================================================
// CANALES ADC (Sensores Analógicos)
// ============================================================================
#define ADC_CHANNEL_LDR             ADC_CHANNEL_3  // PA3
#define ADC_CHANNEL_MIC             ADC_CHANNEL_4  // PA4
#define ADC_CHANNEL_EXTRA1          ADC_CHANNEL_5  // PA5 (peso/humedad)
#define ADC_CHANNEL_EXTRA2          ADC_CHANNEL_6  // PA6 (gas)

#define ADC_BUFFER_SIZE             4
#define ADC_RESOLUTION              4095  // 12-bit (0-4095)

// ============================================================================
// CANALES PWM (Servomotores)
// ============================================================================

// Timer 1
#define TIM_SERVO_PLATAFORMA        TIM_CHANNEL_1  // PA8
#define TIM_SERVO_METAL             TIM_CHANNEL_2  // PA9
#define TIM_SERVO_PAPEL             TIM_CHANNEL_3  // PA10

// Timer 5 (en lugar de Timer 4)
#define TIM_SERVO_PLASTICO          TIM_CHANNEL_1  // PB6
#define TIM_SERVO_VIDRIO            TIM_CHANNEL_2  // PB7

// Valores PWM para servos (20ms period, ARR=20000)
#define SERVO_MIN_PULSE             1000  // 1 ms  (0°)
#define SERVO_MAX_PULSE             2000  // 2 ms  (180°)
#define SERVO_CENTER_PULSE          1500  // 1.5 ms (90°)

// Posiciones específicas de los servos
#define SERVO_PLAT_HORIZONTAL       90     // Posición horizontal (reposo)
#define SERVO_PLAT_METAL            45     // Inclina hacia metal
#define SERVO_PLAT_PAPEL            135    // Inclina hacia papel
#define SERVO_PLAT_PLASTICO         0      // Inclina hacia plástico
#define SERVO_PLAT_VIDRIO           180    // Inclina hacia vidrio

#define SERVO_TAPA_CERRADA          0      // Tapa cerrada
#define SERVO_TAPA_ABIERTA          90     // Tapa abierta

// ============================================================================
// CONSTANTES DEL SISTEMA
// ============================================================================
#define CLASSIFICATION_DELAY_MS     2000   // Tiempo para clasificar
#define SERVO_MOVE_DELAY_MS         500    // Tiempo para movimiento de servo
#define ULTRASONIC_TIMEOUT_US       10000  // Timeout para sensor ultrasónico (10ms)

// Tiempos de operación (ms)
#define SERVO_DELAY_OPEN            500    // Tiempo para abrir tapa
#define SERVO_DELAY_TILT            1000   // Tiempo para inclinar plataforma
#define SERVO_DELAY_DROP            2000   // Tiempo para que caiga el residuo
#define SERVO_DELAY_CLOSE           500    // Tiempo para cerrar tapa

// ============================================================================
// TIPOS DE MATERIALES
// ============================================================================
typedef enum {
  MATERIAL_NINGUNO = 0,
  MATERIAL_METAL = 1,
  MATERIAL_PAPEL = 2,
  MATERIAL_PLASTICO = 3,
  MATERIAL_VIDRIO = 4,
  MATERIAL_DESCONOCIDO = 99
} MaterialType;

// ============================================================================
// ESTRUCTURAS DE DATOS
// ============================================================================

/**
 * @brief Estructura para almacenar el estado de los sensores digitales
 */
typedef struct {
  bool inductivo;    // true si detecta metal
  bool capacitivo;   // true si detecta cualquier material
  bool pir;          // true si detecta movimiento
} SensorDigitalData;

/**
 * @brief Estructura para almacenar los valores de los sensores analógicos
 */
typedef struct {
  uint16_t ldr_laser;      // Valor de LDR con láser (0-4095)
  uint16_t microfono;      // Valor de micrófono (0-4095)
  uint16_t extra1;         // Sensor adicional (peso/humedad)
  uint16_t extra2;         // Sensor adicional (gas)
} SensorAnalogData;

// ============================================================================
// NIVELES DE TRANSLUCIDEZ
// ============================================================================
typedef enum {
  TRANSLUCENCY_OPACO = 0,     // Material opaco
  TRANSLUCENCY_MEDIO = 1,     // Plástico translúcido
  TRANSLUCENCY_ALTO = 2       // Vidrio transparente
} TranslucencyLevel;

// ============================================================================
// NIVELES DE SONIDO
// ============================================================================
typedef enum {
  SOUND_BAJO = 0,             // Papel
  SOUND_MEDIO = 1,            // Plástico
  SOUND_ALTO = 2              // Metal, Vidrio
} SoundLevel;

/**
 * @brief Estructura para niveles de llenado de contenedores - 4 contenedores
 */
typedef struct {
  float metal;        // Distancia en cm
  float papel;
  float plastico;
  float vidrio;
} ContainerLevels;

/**
 * @brief Estructura para estadísticas del sistema
 */
typedef struct {
  uint32_t total_clasificados;     // Total de materiales procesados
  uint32_t contador_metal;         // Cantidad de metal
  uint32_t contador_papel;         // Cantidad de papel
  uint32_t contador_plastico;      // Cantidad de plástico
  uint32_t contador_vidrio;        // Cantidad de vidrio
  uint32_t clasificaciones_erroneas; // Errores de clasificación
  float promedio_confianza;        // Promedio de confianza
  uint32_t tiempo_operacion_horas; // Horas de operación
} Statistics;

// ============================================================================
// VARIABLES EXTERNAS (HANDLES)
// ============================================================================

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim5;  // Cambiado de htim4 a htim5
extern UART_HandleTypeDef huart1; // Cambiado de huart2 a huart1

// ============================================================================
// MACROS DE UTILIDAD
// ============================================================================

#define SET_LED(port, pin, state)   HAL_GPIO_WritePin(port, pin, state)
#define READ_SENSOR(port, pin)      HAL_GPIO_ReadPin(port, pin)
#define TOGGLE_LED(port, pin)       HAL_GPIO_TogglePin(port, pin)

// Conversión de ángulo a pulso PWM
#define ANGLE_TO_PULSE(angle)       (SERVO_MIN_PULSE + ((angle) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180))

// Mapeo de valores (similar a Arduino map())
#define MAP(x, in_min, in_max, out_min, out_max) \
  (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

#endif // CONFIG_H