# 🔧 Guía de Instalación - Smart Waste Manager STM32F410RB

Guía paso a paso para configurar y compilar el proyecto en STM32CubeIDE.

---

## 📋 **Requisitos Previos**

### Software Necesario:
- **STM32CubeIDE** (versión 1.13.0 o superior)
- **STM32CubeMX** (integrado en CubeIDE)
- **STM32F4 HAL Drivers** (incluidos en CubeIDE)

### Hardware:
- **Nucleo STM32F410RB** (o compatible)
- Cable USB (incluido con Nucleo)
- Componentes según `docs/DIAGRAMA_CONEXIONES.md`

---

## 🚀 **Paso 1: Crear Proyecto en STM32CubeIDE**

### 1.1 Abrir STM32CubeIDE
1. Abrir **STM32CubeIDE**
2. Seleccionar **File → New → STM32 Project**

### 1.2 Seleccionar Microcontrolador
1. En la pestaña **"Board Selector"**
2. Buscar **"STM32F410RB"**
3. Seleccionar **"NUCLEO-F410RB"**
4. Click **"Next"**

### 1.3 Configurar Proyecto
```
Project Name: SmartWasteManager
Project Type: STM32CubeIDE
Toolchain: STM32CubeIDE
```

---

## ⚙️ **Paso 2: Configurar Periféricos en CubeMX**

### 2.1 Configurar Clock (Clock Configuration)

1. **HSE (High Speed External)**
   - ✅ **Habilitar**: HSE
   - **Frecuencia**: `8 MHz` (cristal externo del Nucleo)

2. **PLL Configuration**
   - **PLL Source**: `HSE` (no HSI)
   - **PLLM**: `/8` (8 MHz ÷ 8 = 1 MHz)
   - **PLLN**: `× 100` (1 MHz × 100 = 100 MHz)
   - **PLLP**: `/2` (100 MHz ÷ 2 = 100 MHz)
   - **PLLQ**: `/4` (para USB, no necesario)

3. **System Clock Mux**
   - **SYSCLK Source**: `PLLCLK`
   - **SYSCLK**: `100 MHz` ✅

4. **AHB Prescaler**
   - **HCLK**: `/1` (100 MHz ÷ 1 = 100 MHz) ✅

5. **APB Prescalers**
   - **APB1**: `/2` (100 MHz ÷ 2 = 50 MHz) ✅
   - **APB2**: `/1` (100 MHz ÷ 1 = 100 MHz) ✅

### 2.2 Configurar GPIO (Pinout & Configuration)

#### A) **Sensores Digitales**
```
PA0  → GPIO_Input  (Sensor Inductivo)
PA1  → GPIO_Input  (Sensor Capacitivo)  
PA2  → GPIO_Input  (Sensor PIR)
```

#### B) **Sensores Analógicos (ADC)**
```
PA3  → ADC1_IN3    (LDR + Laser)
PA4  → ADC1_IN4    (Micrófono)
PA5  → ADC1_IN5    (Sensor Extra 1)
PA6  → ADC1_IN6    (Sensor Extra 2)
```

#### C) **LEDs Indicadores**
```
PC2  → GPIO_Output  (LED Metal)
PC3  → GPIO_Output  (LED Papel)
PC4  → GPIO_Output  (LED Plástico)
PC5  → GPIO_Output  (LED Vidrio)
PC6  → GPIO_Output  (LED Error)
PC7  → GPIO_Output  (LED Sistema)
```

#### D) **Sensores Ultrasónicos**
```
PB0  → GPIO_Output  (US Metal TRIG)
PB1  → GPIO_Input   (US Metal ECHO)
PB10 → GPIO_Output  (US Papel TRIG)
PB11 → GPIO_Input   (US Papel ECHO)
PB12 → GPIO_Output  (US Plástico TRIG)
PB13 → GPIO_Input   (US Plástico ECHO)
PB14 → GPIO_Output  (US Vidrio TRIG)
PB15 → GPIO_Input   (US Vidrio ECHO)
```

#### E) **Servomotores (PWM)**
```
PA8  → TIM1_CH1     (Servo Plataforma)
PA9  → TIM1_CH2     (Servo Metal)
PA10 → TIM1_CH3     (Servo Papel)
PB6  → TIM5_CH1     (Servo Plástico)
PB7  → TIM5_CH2     (Servo Vidrio)
```

#### F) **I2C - LCD**
```
PB8  → I2C1_SCL     (LCD SCL)
PB9  → I2C1_SDA     (LCD SDA)
```

#### G) **UART - Debug**
```
PA2  → USART1_TX    (Debug TX)
PA3  → USART1_RX    (Debug RX)
```

### 2.3 Configurar Periféricos

#### A) **ADC1 - Sensores Analógicos**
1. Click en **"ADC1"**
2. Configurar:
   ```
   Mode: Injected conversion mode
   Injected conversion mode: Regular conversion mode
   
   Channel 3: IN3 (PA3 - LDR)
   Channel 4: IN4 (PA4 - Micrófono)
   Channel 5: IN5 (PA5 - Extra 1)
   Channel 6: IN6 (PA6 - Extra 2)
   
   Parameter Settings:
     Resolution: 12-bit (15 ADCCLK cycles)
     Data Alignment: Right alignment
     Scan Conversion Mode: Enabled
     Continuous Conversion Mode: Disabled
     Discontinuous Conversion Mode: Disabled
     Injected End Of Conversion: Interrupt
   ```

#### B) **DMA - ADC**
1. Click en **"DMA Settings"**
2. Añadir:
   ```
   DMA Request: ADC1
   Channel: DMA1 Channel 1
   Direction: Peripheral to Memory
   Priority: High
   Mode: Circular
   Data Width: Half Word
   ```

#### C) **Timer 1 - PWM para Servos (3 servos)**
1. Click en **"TIM1"**
2. Configurar:
   ```
   Clock Source: Internal Clock
   
   Channel 1: PWM Generation CH1 (PA8 - Servo Plataforma)
   Channel 2: PWM Generation CH2 (PA9 - Servo Metal)
   Channel 3: PWM Generation CH3 (PA10 - Servo Papel)
   
   Parameter Settings:
     Prescaler: 100-1  (timer a 1 MHz)
     Counter Period (ARR): 20000-1  (20 ms = 50 Hz)
     Pulse (CCR): 1500  (posición inicial 90°)
   
   PWM Settings:
     Mode: PWM mode 1
   ```

#### D) **Timer 5 - PWM para Servos (2 servos)**
1. Click en **"TIM5"**
2. Configurar:
   ```
   Clock Source: Internal Clock
   
   Channel 1: PWM Generation CH1 (PB6 - Servo Plástico)
   Channel 2: PWM Generation CH2 (PB7 - Servo Vidrio)
   
   Parameter Settings:
     Prescaler: 100-1  (timer a 1 MHz)
     Counter Period (ARR): 20000-1  (20 ms = 50 Hz)
     Pulse (CCR): 1500  (posición inicial 90°)
   
   PWM Settings:
     Mode: PWM mode 1
   ```

#### E) **I2C1 - LCD**
1. Click en **"I2C1"**
2. Configurar:
   ```
   I2C Mode: I2C
   I2C Speed Mode: Standard Mode (100 kHz)
   ```

#### F) **USART1 - Debug**
1. Click en **"USART1"**
2. Configurar:
   ```
   Mode: Asynchronous
   Hardware Flow Control: Disable
   
   Parameter Settings:
     Baud Rate: 115200 Bits/s
     Word Length: 8 Bits (including Parity)
     Parity: None
     Stop Bits: 1
   ```

---

## 💻 **Paso 3: Generar Código**

### 3.1 Generar Código Base
1. Click **"Generate Code"** (Ctrl+G)
2. Seleccionar **"Open Project"** cuando termine
3. Esperar a que se compile sin errores

### 3.2 Verificar Compilación
1. **Project → Build All** (Ctrl+B)
2. Debe compilar sin errores
3. Si hay errores, revisar configuración de periféricos

---

## 📁 **Paso 4: Integrar Archivos del Proyecto**

### 4.1 Copiar Archivos de Headers
Copiar a `Core/Inc/`:
- `config.h`
- `sensors.h`
- `classifier.h`
- `actuators.h`
- `display.h`
- `statistics.h`

### 4.2 Copiar Archivos de Implementación
Copiar a `Core/Src/`:
- `sensors.c`
- `classifier.c`
- `actuators.c`
- `display.c`
- `statistics.c`

### 4.3 Reemplazar main.c
Reemplazar el `main.c` generado con el del proyecto.

---

## 🔧 **Paso 5: Configurar Includes**

### 5.1 Verificar Includes en main.c
```c
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

// Archivos del proyecto
#include "config.h"
#include "sensors.h"
#include "classifier.h"
#include "actuators.h"
#include "display.h"
#include "statistics.h"
#include <stdio.h>
```

### 5.2 Verificar Includes en otros archivos
Cada archivo `.c` debe incluir su correspondiente `.h` y `config.h`.

---

## 🛠️ **Paso 6: Compilar y Depurar**

### 6.1 Compilar Proyecto
1. **Project → Clean** (limpiar proyecto)
2. **Project → Build All** (Ctrl+B)
3. Verificar que compile sin errores

### 6.2 Configurar Debug
1. **Run → Debug Configurations**
2. Seleccionar **"STM32 C/C++ Application"**
3. Configurar:
   ```
   Project: SmartWasteManager
   C/C++ Application: SmartWasteManager.elf
   Debugger: STM32 Cortex-M
   ```

### 6.3 Ejecutar Debug
1. **Run → Debug** (F11)
2. El programa debe iniciar en `main()`
3. Verificar que se ejecute sin errores

---

## 📊 **Paso 7: Verificar Funcionamiento**

### 7.1 Monitor Serial
1. Abrir **Window → Show View → Serial Monitor**
2. Configurar:
   ```
   Port: COMx (puerto del Nucleo)
   Baud Rate: 115200
   Data Bits: 8
   Parity: None
   Stop Bits: 1
   ```

### 7.2 Mensajes Esperados
```
╔══════════════════════════════════════════════════════════╗
║            SMART WASTE MANAGER - STM32F410RB             ║
║                  Sistema Iniciado                        ║
╚══════════════════════════════════════════════════════════╝

Smart Waste Manager STM32F410RB - Iniciado
Materiales: Metal, Papel, Plástico, Vidrio
Servos: TIM1 (3) + TIM5 (2)
UART: USART1 para debug
Sensores inicializados correctamente
Clasificador inicializado
Actuadores inicializados
Display inicializado
Estadísticas inicializadas desde cero
```

---

## ⚠️ **Solución de Problemas**

### Error: "No such file or directory"
- **Causa**: Archivos no copiados correctamente
- **Solución**: Verificar que todos los archivos estén en las carpetas correctas

### Error: "Undefined reference"
- **Causa**: Función no implementada o include faltante
- **Solución**: Verificar includes y implementaciones

### Error: "ADC not initialized"
- **Causa**: ADC no configurado correctamente
- **Solución**: Revisar configuración de ADC en CubeMX

### Error: "PWM not working"
- **Causa**: Timer no configurado correctamente
- **Solución**: Revisar configuración de TIM1 y TIM5

### Error: "UART not working"
- **Causa**: USART1 no configurado correctamente
- **Solución**: Revisar configuración de USART1

---

## ✅ **Verificación Final**

### Checklist de Verificación:
- [ ] Proyecto compila sin errores
- [ ] Debug funciona correctamente
- [ ] Monitor serial muestra mensajes
- [ ] LEDs se encienden/apagan
- [ ] Servos responden a comandos
- [ ] Sensores leen valores
- [ ] Clasificador funciona
- [ ] Estadísticas se actualizan

---

## 🎯 **Próximos Pasos**

1. **Calibrar sensores** según `docs/GUIA_CALIBRACION.md`
2. **Conectar hardware** según `docs/DIAGRAMA_CONEXIONES.md`
3. **Probar clasificación** con materiales reales
4. **Ajustar umbrales** según resultados
5. **¡Usar el sistema!**

---

**¡Proyecto configurado correctamente!** 🚀