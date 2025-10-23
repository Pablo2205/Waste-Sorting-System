# 🔌 Diagrama de Conexiones - Smart Waste Manager

## 📋 Vista General del Sistema

```
┌─────────────────────────────────────────────────────────────────┐
│                        ARDUINO MEGA 2560                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  SENSORES DIGITALES        SENSORES ANALÓGICOS                  │
│  ┌──────────────┐          ┌──────────────┐                     │
│  │ Inductivo→2  │          │ LDR    → A0  │                     │
│  │ Capacitivo→3 │          │ MIC    → A1  │                     │
│  │ PIR      →4  │          └──────────────┘                     │
│  └──────────────┘                                                │
│                                                                  │
│  SERVOMOTORES              ULTRASÓNICOS                          │
│  ┌──────────────┐          ┌──────────────────┐                 │
│  │ Plataforma→A4│          │ Metal T:5  E:6   │                 │
│  │ T.Metal  →A5 │          │ Papel T:7  E:8   │                 │
│  │ T.Papel  →13 │          │ Plast T:9  E:10  │                 │
│  │ T.Plast  →A6 │          │ Vidro T:11 E:12  │                 │
│  │ T.Vidrio →A7 │          │ Cartn T:A2 E:A3  │                 │
│  │ T.Carton →22 │          └──────────────────┘                 │
│  └──────────────┘                                                │
│                                                                  │
│  LEDs                      I2C (LCD)                             │
│  ┌──────────────┐          ┌──────────────┐                     │
│  │ Metal   →24  │          │ SDA → 20     │                     │
│  │ Papel   →25  │          │ SCL → 21     │                     │
│  │ Plast   →26  │          └──────────────┘                     │
│  │ Vidrio  →27  │                                                │
│  │ Carton  →28  │          SPEAKER                               │
│  │ Error   →29  │          ┌──────────────┐                     │
│  │ Sistema →30  │          │ Speaker→ 23  │                     │
│  └──────────────┘          └──────────────┘                     │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🔴 SENSORES DIGITALES

### Sensor Inductivo (Detecta Metal)
```
Sensor Inductivo
   VCC  →  5V Arduino
   GND  →  GND Arduino
   OUT  →  Pin 2 Digital
```

### Sensor Capacitivo (Detecta Material)
```
Sensor Capacitivo
   VCC  →  5V Arduino
   GND  →  GND Arduino
   OUT  →  Pin 3 Digital
```

### Sensor PIR (Detecta Movimiento)
```
Sensor PIR
   VCC  →  5V Arduino
   GND  →  GND Arduino
   OUT  →  Pin 4 Digital
```

---

## 🟢 SENSORES ANALÓGICOS

### Módulo Laser + LDR (Translucidez)
```
Módulo Laser:
   VCC  →  5V Arduino
   GND  →  GND Arduino

LDR (Fotoresistor):
   Pin 1  →  5V
   Pin 2  →  Pin A0 + Resistor 10kΩ → GND

Esquema divisor de voltaje:
   5V ──┬── LDR ──┬── A0
        │         │
        │         └── 10kΩ → GND
```

### Módulo Micrófono
```
Módulo Micrófono (MAX4466 o similar)
   VCC  →  5V Arduino
   GND  →  GND Arduino
   OUT  →  Pin A1
```

---

## 🟡 SENSORES ULTRASÓNICOS (HC-SR04)

### Contenedor Metal
```
HC-SR04 #1
   VCC   →  5V Arduino
   TRIG  →  Pin 5
   ECHO  →  Pin 6
   GND   →  GND Arduino
```

### Contenedor Papel
```
HC-SR04 #2
   VCC   →  5V Arduino
   TRIG  →  Pin 7
   ECHO  →  Pin 8
   GND   →  GND Arduino
```

### Contenedor Plástico
```
HC-SR04 #3
   VCC   →  5V Arduino
   TRIG  →  Pin 9
   ECHO  →  Pin 10
   GND   →  GND Arduino
```

### Contenedor Vidrio
```
HC-SR04 #4
   VCC   →  5V Arduino
   TRIG  →  Pin 11
   ECHO  →  Pin 12
   GND   →  GND Arduino
```

### Contenedor Cartón
```
HC-SR04 #5
   VCC   →  5V Arduino
   TRIG  →  Pin A2
   ECHO  →  Pin A3
   GND   →  GND Arduino
```

---

## ⚙️ SERVOMOTORES

### ⚠️ IMPORTANTE - Alimentación de Servos
```
NO alimentar servos desde el pin 5V del Arduino!
Usar fuente externa de 5-6V, 2A mínimo

Conexión:
┌─────────────────────────┐
│  Fuente Externa 5-6V    │
│  (2A mínimo)            │
└────┬─────────────┬──────┘
     │             │
     │ +           │ -
     ↓             ↓
   [VCC]         [GND] ← Conectar GND común con Arduino
```

### Servo Plataforma Principal
```
Servo 1 (MG996R recomendado - mayor torque)
   VCC    →  +5V fuente externa
   GND    →  GND común (fuente + Arduino)
   SEÑAL  →  Pin A4
```

### Servos de Tapas de Contenedores
```
Servo Tapa Metal
   VCC    →  +5V fuente externa
   GND    →  GND común
   SEÑAL  →  Pin A5

Servo Tapa Papel
   VCC    →  +5V fuente externa
   GND    →  GND común
   SEÑAL  →  Pin 13

Servo Tapa Plástico
   VCC    →  +5V fuente externa
   GND    →  GND común
   SEÑAL  →  Pin A6

Servo Tapa Vidrio
   VCC    →  +5V fuente externa
   GND    →  GND común
   SEÑAL  →  Pin A7

Servo Tapa Cartón
   VCC    →  +5V fuente externa
   GND    →  GND común
   SEÑAL  →  Pin 22
```

---

## 💡 LEDs INDICADORES

### Conexión de LEDs (con resistencias)
```
Cada LED:
   Pin Arduino → Resistencia 220Ω → Ánodo LED (+) → Cátodo LED (-) → GND

LED Metal:     Pin 24 → 220Ω → LED → GND
LED Papel:     Pin 25 → 220Ω → LED → GND
LED Plástico:  Pin 26 → 220Ω → LED → GND
LED Vidrio:    Pin 27 → 220Ω → LED → GND
LED Cartón:    Pin 28 → 220Ω → LED → GND
LED Error:     Pin 29 → 220Ω → LED → GND (usar LED ROJO)
LED Sistema:   Pin 30 → 220Ω → LED → GND (usar LED VERDE)
```

---

## 📺 DISPLAY LCD 16x2 con I2C

### Módulo I2C LCD
```
Módulo LCD I2C
   VCC  →  5V Arduino
   GND  →  GND Arduino
   SDA  →  Pin 20 (SDA) en Mega
   SCL  →  Pin 21 (SCL) en Mega

Nota: En Arduino Uno, SDA=A4, SCL=A5
```

### Verificar Dirección I2C
```cpp
// Usar I2C Scanner para encontrar dirección
// Dirección común: 0x27 o 0x3F
```

---

## 🔊 SPEAKER / BUZZER

### Buzzer Piezoeléctrico
```
Buzzer
   Pin +  →  Pin 23
   Pin -  →  GND

O usar transistor NPN para mayor volumen:
   Pin 23 → Resistencia 1kΩ → Base NPN
   Emisor → GND
   Colector → Buzzer (-) 
   Buzzer (+) → 5V
```

---

## 🔋 ALIMENTACIÓN DEL SISTEMA

### Opción 1: Alimentación Dual
```
Arduino Mega:
   - USB (5V, 500mA) para programación
   - Jack DC (7-12V, 1A) para operación

Servos:
   - Fuente externa 5-6V, 2-3A
   - GND común con Arduino
```

### Opción 2: Fuente Única
```
Fuente 5V, 5A:
   - Alimenta Arduino por pin 5V
   - Alimenta todos los servos
   - Asegurar capacidad de corriente suficiente
```

---

## 📐 DIAGRAMA MECÁNICO SUGERIDO

```
Vista Superior:

                    [LCD Display]
                    
    ┌───────────────────────────────────┐
    │                                   │
    │         Plataforma Basculante     │
    │         (Servo Plataforma)        │
    │                                   │
    └────────────┬──────────────────────┘
                 │
                 ↓ Inclinación
         
    ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐
    │Metal│  │Papel│  │Plast│  │Vidro│  │Cartn│
    │ [S] │  │ [S] │  │ [S] │  │ [S] │  │ [S] │
    └─────┘  └─────┘  └─────┘  └─────┘  └─────┘
    
    S = Servo de tapa
    
Posición de sensores:
- Inductivo: Cerca de la plataforma
- Capacitivo: Bajo la plataforma
- LDR: Lateral con laser apuntando
- Micrófono: Cerca de speaker
- Ultrasónicos: En tapa de cada contenedor
```

---

## 🛠️ LISTA DE MATERIALES (BOM)

### Electrónica
- [x] 1x Arduino Mega 2560
- [x] 1x Sensor Inductivo (LJ12A3-4-Z/BX o similar)
- [x] 1x Sensor Capacitivo (TL-Q5MC1 o similar)
- [x] 1x Sensor PIR (HC-SR501)
- [x] 5x Sensor Ultrasónico HC-SR04
- [x] 1x Módulo Laser 5mW + LDR
- [x] 1x Módulo Micrófono MAX4466
- [x] 6x Servomotor (1x MG996R + 5x SG90)
- [x] 1x LCD 16x2 con módulo I2C
- [x] 7x LEDs (5 colores + 1 rojo + 1 verde)
- [x] 7x Resistencias 220Ω
- [x] 1x Resistencia 10kΩ (LDR)
- [x] 1x Buzzer piezoeléctrico
- [x] 1x Fuente 5V 3A
- [x] Cables jumper MM/MF/FF
- [x] Protoboard o PCB

### Mecánica
- [ ] Estructura de soporte (madera/acrílico)
- [ ] 5x Contenedores
- [ ] Mecanismo de plataforma basculante
- [ ] Bisagras para tapas
- [ ] Tornillos y soportes

---

## 🧪 VERIFICACIÓN DE CONEXIONES

### Checklist Pre-Encendido
- [ ] Verificar polaridad de todos los componentes
- [ ] GND común entre Arduino y fuente externa
- [ ] Servos NO conectados a 5V del Arduino
- [ ] Resistencias en serie con LEDs
- [ ] Conexiones I2C correctas (SDA/SCL)
- [ ] No hay cortocircuitos
- [ ] Cables firmemente conectados

### Primera Prueba
1. Conectar solo Arduino (sin servos)
2. Cargar programa de prueba
3. Verificar LEDs y LCD
4. Conectar servos uno por uno
5. Probar sensores individualmente

---

**¡Verificar dos veces antes de energizar!** ⚡

