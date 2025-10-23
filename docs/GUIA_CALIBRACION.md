# 🎯 Guía de Calibración - Smart Waste Manager

Esta guía te ayudará a calibrar correctamente todos los sensores del sistema para obtener la mejor precisión en la clasificación de materiales.

---

## 📋 Índice
1. [Preparación](#preparación)
2. [Calibración de Sensores Digitales](#calibración-de-sensores-digitales)
3. [Calibración del LDR (Translucidez)](#calibración-del-ldr-translucidez)
4. [Calibración del Micrófono (Sonido)](#calibración-del-micrófono-sonido)
5. [Calibración de Servomotores](#calibración-de-servomotores)
6. [Calibración de Sensores Ultrasónicos](#calibración-de-sensores-ultrasónicos)
7. [Pruebas de Clasificación](#pruebas-de-clasificación)

---

## 🔧 Preparación

### Materiales de Prueba Necesarios
Reúne muestras de cada tipo de material:

- **Metal**: 
  - Lata de refresco vacía
  - Tapa de metal
  - Objeto metálico pequeño

- **Papel**:
  - Hoja de papel bond
  - Servilleta
  - Papel periódico

- **Plástico**:
  - Botella PET translúcida
  - Envase plástico opaco
  - Bolsa plástica

- **Vidrio**:
  - Frasco de vidrio transparente
  - Botella de vidrio
  - Vidrio de ventana

- **Cartón**:
  - Caja de cartón corrugado
  - Cartón delgado
  - Tubo de cartón

### Herramientas
- Monitor serial de Arduino (115200 baud)
- Bloc de notas para registrar valores
- Iluminación consistente (para sensor LDR)
- Regla o cinta métrica

---

## 🔴 Calibración de Sensores Digitales

### Sensor Inductivo

**Objetivo**: Verificar que solo detecte metal

1. Activar modo calibración:
   ```
   Enviar 'c' por monitor serial
   ```

2. Probar con cada material:
   - Acercar metal → debe mostrar `Inductivo: 1`
   - Acercar otros materiales → debe mostrar `Inductivo: 0`

3. Ajustar distancia de detección:
   - Girar potenciómetro del sensor (si tiene)
   - Distancia recomendada: 2-4 mm

4. **Problema común**: Si detecta otros materiales
   - Alejar de superficies metálicas
   - Verificar alimentación (necesita 10-30V DC algunos modelos)
   - Considerar usar modelo NPN o PNP según tu conexión

### Sensor Capacitivo

**Objetivo**: Detectar presencia de cualquier material

1. Probar detección:
   - Acercar cualquier material → `Capacitivo: 1`
   - Sin material → `Capacitivo: 0`

2. Ajustar sensibilidad:
   - Girar potenciómetro en el sensor
   - Debe detectar a 1-3 cm de distancia

3. **Problema común**: No detecta o siempre activo
   - Ajustar potenciómetro
   - Verificar que no haya objetos permanentes cerca

### Sensor PIR (Movimiento)

**Objetivo**: Detectar cuando se coloca material

1. Verificar detección de movimiento:
   - Mover mano frente al sensor → `Movimiento: 1`
   - Sin movimiento → `Movimiento: 0`

2. Ajustar sensibilidad y tiempo:
   - Dos potenciómetros en el HC-SR501
   - Sensitivity: medio-alto
   - Time delay: 2-3 segundos

---

## 🟡 Calibración del LDR (Translucidez)

**Objetivo**: Diferenciar vidrio (muy translúcido), plástico (medio) y opacos

### Paso 1: Configurar Setup Físico

```
┌─────────┐        3-5 cm        ┌─────────┐
│  LASER  │ ──────────→→→─────── │   LDR   │
└─────────┘                      └─────────┘
             ↑
         [Material aquí]
```

**Importante**: 
- Mantener distancia fija entre laser y LDR
- Evitar luz ambiental directa (usar tubo o caja oscura)
- Laser debe apuntar directamente al LDR

### Paso 2: Medir Valores Base

1. Activar modo calibración (`c`)
2. Encender laser
3. Leer valor SIN material:
   ```
   LDR: [valor_max]
   ```
   Ejemplo: `LDR: 1020`

4. Leer valor CON mano (opaco total):
   ```
   LDR: [valor_min]
   ```
   Ejemplo: `LDR: 50`

### Paso 3: Medir Cada Tipo de Material

| Material | Ejemplo de Lectura | Clasificación |
|----------|-------------------|---------------|
| **Vidrio transparente** | 800-1023 | TRANSLUCIDO_ALTO |
| **Plástico PET** | 400-799 | TRANSLUCIDO_MEDIO |
| **Plástico opaco** | 100-399 | TRANSLUCIDO_OPACO |
| **Papel** | 50-200 | TRANSLUCIDO_OPACO |
| **Metal** | 30-100 | TRANSLUCIDO_OPACO |
| **Cartón** | 40-180 | TRANSLUCIDO_OPACO |

### Paso 4: Ajustar Umbrales en config.h

Basándote en tus mediciones, edita:

```cpp
// Ejemplo: Si tus mediciones fueron:
// Vidrio: 750-1023
// Plástico: 350-749
// Opacos: 0-349

#define LDR_THRESHOLD_VIDRIO_MIN    750   // Ajustar según tus mediciones
#define LDR_THRESHOLD_VIDRIO_MAX    1023
#define LDR_THRESHOLD_PLASTICO_MIN  350
#define LDR_THRESHOLD_PLASTICO_MAX  749
#define LDR_THRESHOLD_OPACO         349
```

### Paso 5: Verificación

1. Enviar `c` para modo calibración
2. Colocar cada material
3. Verificar clasificación correcta:
   ```
   Translucidez: ALTO        → Vidrio ✓
   Translucidez: MEDIO       → Plástico ✓
   Translucidez: OPACO       → Papel/Cartón/Metal ✓
   ```

---

## 🔊 Calibración del Micrófono (Sonido)

**Objetivo**: Diferenciar materiales por su respuesta acústica al ser golpeados

### Paso 1: Setup Físico

```
┌──────────┐    golpe     ┌──────────┐
│ MATERIAL │ ←─────●      │   MIC    │
└──────────┘              └──────────┘
                5-10 cm
```

**Configuración del micrófono**:
- Ganancia: media (ajustar potenciómetro si tiene)
- Distancia: 5-10 cm del material
- Evitar ruido ambiental

### Paso 2: Generar Sonido de Prueba

El sistema genera automáticamente un tono de 2000 Hz durante 200 ms:
```cpp
SOUND_TEST_FREQUENCY = 2000  // Hz
SOUND_TEST_DURATION = 200    // ms
```

### Paso 3: Medir Respuesta de Cada Material

**Procedimiento**:
1. Activar modo calibración (`c`)
2. Colocar material en plataforma
3. Sistema genera sonido y mide respuesta
4. Observar valor de `MIC`

**Tabla de Valores Típicos**:

| Material | Respuesta Típica | Clasificación | Descripción |
|----------|------------------|---------------|-------------|
| **Metal** | 650-1023 | SONIDO_MUY_ALTO | Resonancia metálica fuerte |
| **Vidrio** | 500-649 | SONIDO_ALTO | Sonido agudo, resonante |
| **Plástico** | 250-499 | SONIDO_MEDIO | Sonido amortiguado |
| **Cartón** | 150-249 | SONIDO_BAJO | Sonido sordo |
| **Papel** | 0-149 | SONIDO_MUY_BAJO | Muy amortiguado |

### Paso 4: Pruebas Sistemáticas

Para cada material, realizar 5 mediciones:

```
METAL #1: MIC = 720
METAL #2: MIC = 680
METAL #3: MIC = 710
METAL #4: MIC = 695
METAL #5: MIC = 705
Promedio: 702
```

Repetir con todos los materiales.

### Paso 5: Ajustar Umbrales

Basándote en tus promedios:

```cpp
// Ejemplo basado en mediciones:
#define MIC_THRESHOLD_METAL_MIN     650   // Metal >= 650
#define MIC_THRESHOLD_VIDRIO_MIN    500   // Vidrio >= 500
#define MIC_THRESHOLD_PLASTICO_MIN  250   // Plástico >= 250
#define MIC_THRESHOLD_CARTON_MIN    150   // Cartón >= 150
#define MIC_THRESHOLD_PAPEL_MIN     0     // Papel < 150
```

### Paso 6: Optimización Opcional

Si la clasificación no es clara, puedes:

**A) Ajustar frecuencia del tono**:
```cpp
#define SOUND_TEST_FREQUENCY    1500  // Probar 1000-3000 Hz
```

**B) Aumentar duración**:
```cpp
#define SOUND_TEST_DURATION     300   // Más tiempo = más mediciones
```

**C) Aumentar número de muestras**:
```cpp
#define MEASUREMENT_SAMPLES     20    // Más muestras = mejor promedio
```

---

## ⚙️ Calibración de Servomotores

### Servo de Plataforma Principal

**Objetivo**: Configurar ángulos precisos para depositar en cada contenedor

#### Paso 1: Posición Horizontal

1. Ejecutar modo prueba (`t`)
2. Observar posición horizontal
3. Si no está nivelado, ajustar:
   ```cpp
   #define SERVO_PLAT_HORIZONTAL   90  // Ajustar hasta nivelar
   ```

#### Paso 2: Ángulos de Inclinación

Para cada contenedor:

1. Probar ángulo por defecto
2. Verificar que el material caiga correctamente
3. Ajustar según necesidad:

```cpp
// Ajustar estos valores según tu mecánica
#define SERVO_PLAT_METAL        45    // ¿Material cae en contenedor metal?
#define SERVO_PLAT_PAPEL        135   // ¿Material cae en contenedor papel?
#define SERVO_PLAT_PLASTICO     0     // Ajustar si es necesario
#define SERVO_PLAT_VIDRIO       180   // Ajustar si es necesario
#define SERVO_PLAT_CARTON       110   // Ajustar si es necesario
```

**Rango típico**: 0-180° (depende del servo y mecánica)

#### Paso 3: Velocidad de Movimiento

Ajustar suavidad del movimiento:

```cpp
void moveServoSmooth(Servo &servo, int targetAngle, int speed = 15);
//                                                             ^^
// speed = ms entre cada grado
// Aumentar = más lento pero suave
// Disminuir = más rápido pero brusco
```

### Servos de Tapas

**Objetivo**: Configurar posiciones abierto/cerrado

#### Para cada servo de tapa:

1. Enviar `t` para modo prueba
2. Observar movimiento de tapas
3. Ajustar ángulos:

```cpp
#define SERVO_TAPA_CERRADA      0     // Tapa completamente cerrada
#define SERVO_TAPA_ABIERTA      90    // Tapa completamente abierta
```

**Notas**:
- Algunos servos pueden necesitar ángulos invertidos (90 cerrado, 0 abierto)
- Asegurar que no fuerce la mecánica (podría dañar servo)

---

## 📏 Calibración de Sensores Ultrasónicos

**Objetivo**: Medir correctamente el nivel de llenado de contenedores

### Paso 1: Instalación

1. Montar sensor en la parte superior de cada contenedor
2. Apuntando hacia abajo
3. Distancia del sensor al fondo del contenedor: 20-30 cm

### Paso 2: Medición de Referencia

1. Contenedor VACÍO:
   ```
   Enviar 'l' por serial
   Observar: "Metal: 28.5 cm"
   ```
   Este es tu valor máximo (contenedor vacío)

2. Contenedor LLENO (simulado):
   ```
   Llenar con material de prueba
   Observar: "Metal: 3.2 cm"
   ```

### Paso 3: Configurar Umbrales

```cpp
#define ULTRA_MAX_DISTANCE      30    // Ajustar según altura de tu contenedor
#define ULTRA_THRESHOLD_FULL    5     // Contenedor lleno (cm desde sensor)
#define ULTRA_THRESHOLD_WARN    10    // Advertencia (cm desde sensor)
```

**Ejemplo**:
- Contenedor de 30 cm de alto
- `ULTRA_MAX_DISTANCE = 30`
- `THRESHOLD_FULL = 5` (cuando basura está a 5 cm del sensor)
- `THRESHOLD_WARN = 10` (advertencia a 10 cm del sensor)

### Paso 4: Verificación

```
Vacío:      30 cm → OK, aceptar materiales
Advertencia: 9 cm → Advertir que se está llenando  
Lleno:       4 cm → No aceptar más material de este tipo
```

---

## ✅ Pruebas de Clasificación

### Test Individual por Material

#### Test 1: METAL
```
1. Colocar lata de metal
2. Esperar clasificación
3. Verificar:
   ✓ Inductivo: 1
   ✓ Capacitivo: 1
   ✓ Material: METAL
   ✓ Confianza: > 90%
   ✓ LED Metal encendido
   ✓ Depositado en contenedor correcto
```

#### Test 2: VIDRIO
```
1. Colocar frasco de vidrio
2. Verificar:
   ✓ Inductivo: 0
   ✓ Capacitivo: 1
   ✓ Translucidez: ALTO
   ✓ Sonido: ALTO
   ✓ Material: VIDRIO
   ✓ Confianza: > 85%
```

#### Test 3: PLASTICO
```
1. Colocar botella PET
2. Verificar:
   ✓ Translucidez: MEDIO
   ✓ Sonido: MEDIO
   ✓ Material: PLASTICO
   ✓ Confianza: > 80%
```

#### Test 4: PAPEL
```
1. Colocar hoja de papel
2. Verificar:
   ✓ Translucidez: OPACO
   ✓ Sonido: MUY_BAJO
   ✓ Material: PAPEL
   ✓ Confianza: > 75%
```

#### Test 5: CARTON
```
1. Colocar trozo de cartón
2. Verificar:
   ✓ Translucidez: OPACO
   ✓ Sonido: BAJO
   ✓ Material: CARTON
   ✓ Confianza: > 75%
```

### Test de Precisión

Realizar 10 clasificaciones de cada material y calcular tasa de éxito:

```
Material  | Correcto | Incorrecto | Precisión
----------|----------|------------|----------
Metal     | 10/10    | 0          | 100%
Vidrio    | 9/10     | 1          | 90%
Plástico  | 8/10     | 2          | 80%
Papel     | 7/10     | 3          | 70%
Cartón    | 8/10     | 2          | 80%
----------|----------|------------|----------
PROMEDIO  |          |            | 84%
```

**Objetivo**: > 80% de precisión global

### Casos Problemáticos

Si hay confusión entre materiales, anotar:

```
Problema: Papel clasificado como Cartón
Causa: Valores de sonido muy similares
Solución: 
  1. Revisar umbrales MIC_THRESHOLD
  2. Usar papel más fino vs cartón más grueso
  3. Ajustar frecuencia de sonido de prueba
```

---

## 📊 Tabla de Registro de Calibración

Usa esta tabla para documentar tus valores:

```
┌─────────────────────────────────────────────────────────────┐
│              REGISTRO DE CALIBRACIÓN                        │
├─────────────────────────────────────────────────────────────┤
│ Fecha: _______________                                      │
│                                                             │
│ SENSOR LDR:                                                 │
│   Sin material: _______                                     │
│   Vidrio:      _______  [ALTO]                              │
│   Plástico:    _______  [MEDIO]                             │
│   Papel:       _______  [OPACO]                             │
│   Metal:       _______  [OPACO]                             │
│   Cartón:      _______  [OPACO]                             │
│                                                             │
│ SENSOR MICRÓFONO:                                           │
│   Metal:       _______  [MUY_ALTO]                          │
│   Vidrio:      _______  [ALTO]                              │
│   Plástico:    _______  [MEDIO]                             │
│   Cartón:      _______  [BAJO]                              │
│   Papel:       _______  [MUY_BAJO]                          │
│                                                             │
│ SERVOS PLATAFORMA:                                          │
│   Horizontal:  _______°                                     │
│   Metal:       _______°                                     │
│   Papel:       _______°                                     │
│   Plástico:    _______°                                     │
│   Vidrio:      _______°                                     │
│   Cartón:      _______°                                     │
│                                                             │
│ ULTRASÓNICOS (distancia contenedor vacío):                 │
│   Metal:       _______ cm                                   │
│   Papel:       _______ cm                                   │
│   Plástico:    _______ cm                                   │
│   Vidrio:      _______ cm                                   │
│   Cartón:      _______ cm                                   │
│                                                             │
│ PRECISIÓN DE CLASIFICACIÓN:                                │
│   Metal:       _______% (____/10 correctos)                 │
│   Vidrio:      _______% (____/10 correctos)                 │
│   Plástico:    _______% (____/10 correctos)                 │
│   Papel:       _______% (____/10 correctos)                 │
│   Cartón:      _______% (____/10 correctos)                 │
│   PROMEDIO:    _______%                                     │
│                                                             │
│ NOTAS:                                                      │
│ ___________________________________________________________ │
│ ___________________________________________________________ │
│ ___________________________________________________________ │
└─────────────────────────────────────────────────────────────┘
```

---

## 🎓 Consejos Finales

1. **Iluminación**: Mantener iluminación constante para sensor LDR
2. **Ruido**: Ambiente silencioso para calibración de micrófono
3. **Temperatura**: Sensores pueden variar con temperatura
4. **Paciencia**: Calibración puede tomar 2-3 horas inicialmente
5. **Re-calibración**: Revisar cada semana los primeros meses
6. **Documentar**: Guardar valores que funcionen bien
7. **Probar materiales variados**: No solo un tipo de plástico, papel, etc.

---

**¡Buena calibración! 🎯**

