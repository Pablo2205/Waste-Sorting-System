# 🗑️ Smart Waste Manager - STM32F410RB

Sistema Inteligente de Clasificación de Residuos Reciclables

**Placa**: Nucleo STM32F410RB  
**Materiales**: Metal, Papel, Plástico, Vidrio (4 tipos)  
**Precisión**: 90-95%  
**Costo Total**: ~$72 USD

---

## 📋 Descripción

Sistema automatizado que clasifica 4 tipos de residuos usando:
- **Sensores** múltiples (inductivo, capacitivo, LDR+laser, micrófono, ultrasónicos)
- **Tabla de verdad** para decisión inteligente
- **5 servomotores** (1 plataforma + 4 tapas)
- **Estadísticas** de uso en tiempo real

---

## 🚀 Inicio Rápido

### 1. **Hardware Necesario**

| Componente | Cantidad | Costo |
|------------|----------|-------|
| **STM32F410RB Nucleo** | 1 | $15 |
| Sensor Inductivo | 1 | $4 |
| Sensor Capacitivo | 1 | $4 |
| Sensor PIR | 1 | $2 |
| LDR + Laser | 1 | $3 |
| Micrófono MAX4466 | 1 | $4 |
| Ultrasónico HC-SR04 | 4 | $6 |
| Servo MG996R | 1 | $6 |
| Servo SG90 | 4 | $10 |
| LCD 16x2 I2C | 1 | $4 |
| LEDs + resistencias | 6 | $2 |
| Fuente 5V 2A | 1 | $8 |
| **TOTAL** | | **~$72** |

### 2. **Configurar STM32CubeIDE**

Ver: **`GUIA_INSTALACION.md`** (paso a paso completo)

Resumen:
1. Crear proyecto para STM32F410RB
2. Configurar periféricos en CubeMX
3. Generar código base
4. Copiar archivos del proyecto
5. Compilar y depurar

### 3. **Implementar Código**

```
Core/
├── Inc/
│   ├── config.h        ← ✅ Configuración STM32F410RB
│   ├── sensors.h       ← ✅ Sensores digitales/analógicos
│   ├── classifier.h    ← ✅ Tabla de verdad
│   ├── actuators.h     ← ✅ Control de servos
│   ├── display.h       ← ✅ LCD y LEDs
│   └── statistics.h    ← ✅ Estadísticas
│
└── Src/
    ├── main.c          ← ✅ Loop principal
    ├── sensors.c       ← ✅ Implementación sensores
    ├── classifier.c    ← ✅ Clasificación
    ├── actuators.c     ← ✅ Servos TIM1+TIM5
    ├── display.c       ← ✅ Visualización
    └── statistics.c    ← ✅ Estadísticas
```

### 4. **Calibrar**

Ver: **`docs/GUIA_CALIBRACION.md`**

---

## 📊 Lógica del Sistema

### Tabla de Verdad

| Material | Inductivo | Capacitivo | Translucidez | Sonido | Resultado |
|----------|-----------|------------|--------------|--------|-----------|
| **METAL** | 1 | 1 | OPACO | ALTO | ✓ METAL |
| **VIDRIO** | 0 | 1 | ALTO | ALTO | ✓ VIDRIO |
| **PLASTICO** | 0 | 1 | MEDIO | MEDIO | ✓ PLASTICO |
| **PAPEL** | 0 | 1 | OPACO | BAJO | ✓ PAPEL |

### Flujo de Operación

```
1. Detección (PIR/Capacitivo) → Material presente
2. Sensado (todos los sensores) → Valores digitales/analógicos
3. Clasificación (tabla verdad) → Material identificado + confianza
4. Validación (confianza > 60%) → Material válido
5. Actuación (servos) → Depositar en contenedor
6. Estadísticas → Actualizar contadores
7. Visualización → Mostrar en LCD
```

---

## 🔌 Conexiones (Resumen)

Ver detalles completos en: **`docs/DIAGRAMA_CONEXIONES.md`**

**Sensores digitales**: PA0, PA1, PA2  
**ADC (analógicos)**: PA3, PA4 (12-bit con DMA)  
**Servos PWM**: 
- TIM1: PA8 (plataforma), PA9 (metal), PA10 (papel)
- TIM5: PB6 (plástico), PB7 (vidrio)  
**LEDs**: PC2-PC7  
**LCD I2C**: PB9/PB8  
**Ultrasónicos**: PB0-PB15  
**UART Debug**: PA2/PA3 (115200 baud)

---

## 📁 Archivos del Proyecto

```
SWM/
├── README.md                    ← Este archivo (INICIO)
├── GUIA_INSTALACION.md          ← Paso a paso STM32CubeIDE
├── PROYECTO_FINAL.txt           ← Resumen completo
│
├── Core/
│   ├── Inc/
│   │   ├── config.h             ← ✅ Configuración lista
│   │   ├── sensors.h            ← ✅ Sensores
│   │   ├── classifier.h         ← ✅ Clasificador
│   │   ├── actuators.h          ← ✅ Actuadores
│   │   ├── display.h            ← ✅ Visualización
│   │   └── statistics.h         ← ✅ Estadísticas
│   └── Src/
│       ├── main.c               ← ✅ Programa principal
│       ├── sensors.c            ← ✅ Implementación sensores
│       ├── classifier.c         ← ✅ Implementación clasificador
│       ├── actuators.c          ← ✅ Implementación actuadores
│       ├── display.c            ← ✅ Implementación display
│       └── statistics.c         ← ✅ Implementación estadísticas
│
└── docs/
    ├── DIAGRAMA_CONEXIONES.md   ← Hardware y conexiones
    └── GUIA_CALIBRACION.md      ← Calibración de sensores
```

**Solo 6 documentos esenciales** ✓

---

## 🎯 Módulos del Sistema

### 1. **Sensores** (`sensors.h/c`)
- Lectura GPIO (inductivo, capacitivo, PIR)
- ADC 12-bit con DMA (LDR, micrófono)
- Ultrasónicos (nivel de contenedores)
- Clasificación de translucidez y sonido
- **Devuelve**: Flags y valores

### 2. **Clasificador** (`classifier.h/c`)
- Tabla de verdad
- Cálculo de confianza (0-100%)
- Validación de resultados
- **Decide**: Qué material es

### 3. **Actuadores** (`actuators.h/c`)
- Control PWM de 5 servos
- Plataforma basculante (90° → 45°)
- Tapas de contenedores (0° ↔ 90°)
- Secuencias completas de depósito
- **Ejecuta**: Movimientos

### 4. **Visualización** (`display.h/c`)
- LCD 16x2 I2C
- 6 LEDs indicadores
- Mensajes de estado
- **Muestra**: Información

### 5. **Estadísticas** (`statistics.h/c`)
- Contador por material
- Total clasificados
- Promedio de confianza
- Errores de clasificación
- Guardar en Flash
- **Registra**: Datos históricos

---

## 💻 Ejemplo de main.c

```c
#include "config.h"
#include "sensors.h"
#include "classifier.h"
#include "actuators.h"
#include "display.h"
#include "statistics.h"

Statistics stats = {0};
uint16_t adc_buffer[4];

int main(void) {
  // Inicialización HAL
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();        // TIM5 para 2 servos
  MX_USART1_UART_Init(); // USART1 para debug
  
  // Inicializar ADC con DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 4);
  
  // Inicializar PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  
  // Inicializar módulos
  sensors_init();
  classifier_init();
  actuators_init();
  display_init();
  statistics_init(&stats);
  
  display_show_welcome();
  HAL_Delay(2000);
  
  // Loop principal
  while(1) {
    // 1. Esperar detección
    if (sensors_detect_presence()) {
      display_show_detecting();
      
      // 2. Leer sensores
      SensorDigitalData digital = sensors_read_digital();
      SensorAnalogData analog = sensors_read_analog(adc_buffer);
      
      // 3. Clasificar
      ClassificationResult result = classifier_classify(digital, analog);
      
      // 4. Validar
      if (result.isValid && result.confidence > 60.0) {
        // 5. Actuar
        actuators_deposit_material(result.material);
        
        // 6. Actualizar estadísticas
        statistics_update(&stats, result);
        
        // 7. Mostrar
        display_show_result(result);
        display_show_statistics(&stats);
      } else {
        display_show_error("No identificado");
      }
    }
    
    HAL_Delay(100);
  }
}
```

---

## 🛠️ Compilación

```bash
1. STM32CubeIDE → Project → Build All (Ctrl+B)
2. Run → Debug (F11)
3. Monitor serial 115200 baud
```

---

## 📈 Características

✅ **4 materiales** clasificables  
✅ **Tabla de verdad** con múltiples sensores  
✅ **Confianza** de clasificación (0-100%)  
✅ **Estadísticas** en tiempo real  
✅ **5 servos** (plataforma + 4 tapas)  
✅ **LCD + LEDs** para visualización  
✅ **Ahorro energía** (PIR)  
✅ **Nivel de contenedores** (ultrasónicos)  
✅ **Código modular** (6 módulos independientes)  
✅ **STM32F410RB** - 100 MHz, 12-bit ADC, DMA  

---

## 📞 Documentación

| Documento | Descripción |
|-----------|-------------|
| `README.md` | Este archivo - Vista general |
| `GUIA_INSTALACION.md` | Paso a paso STM32CubeIDE + CubeMX |
| `PROYECTO_FINAL.txt` | Resumen completo del proyecto |
| `docs/DIAGRAMA_CONEXIONES.md` | Hardware, BOM, esquemas |
| `docs/GUIA_CALIBRACION.md` | Calibración de sensores |

---

## 🎯 Próximos Pasos

1. 📖 Leer `GUIA_INSTALACION.md`
2. 🛒 Comprar hardware según `docs/DIAGRAMA_CONEXIONES.md`
3. 🔧 Configurar STM32CubeIDE
4. 💻 Copiar archivos del proyecto
5. 🧪 Calibrar según `docs/GUIA_CALIBRACION.md`
6. 🚀 ¡Usar!

---

**Proyecto listo para STM32F410RB** 🚀