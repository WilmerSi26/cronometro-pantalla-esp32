# Prueba con Arduino UNO como shield

Si la pantalla queda blanca incluso conectada directamente al Arduino UNO, prueba estos sketches en este orden.

## 1. Leer ID

Abre:

```text
arduino/uno_leer_id_tft/uno_leer_id_tft.ino
```

Selecciona:

```text
Herramientas > Placa > Arduino Uno
Herramientas > Puerto > COM correspondiente
```

Sube el programa y abre:

```text
Herramientas > Monitor serie
```

Configura el monitor en:

```text
9600 baudios
```

Anota el valor que aparece como:

```text
ID detectado: 0x....
```

## 2. Mostrar 00:00 forzando ILI9341

Abre:

```text
arduino/uno_prueba_00_00_ili9341/uno_prueba_00_00_ili9341.ino
```

Este sketch fuerza:

```cpp
tft.begin(0x9341);
```

El paquete de la pantalla dice `ILI9341 Driver`, por eso esta es la primera prueba correcta.

## Pines usados por el shield en Arduino UNO

Cuando la pantalla esta insertada como shield, estas conexiones ya vienen por el conector:

| Senal TFT | Arduino UNO | Funcion |
|---|---:|---|
| `LCD_RD` | A0 | Lectura desde el controlador |
| `LCD_WR` | A1 | Pulso de escritura |
| `LCD_RS` | A2 | Comando o dato |
| `LCD_CS` | A3 | Seleccion de pantalla |
| `LCD_RST` | A4 | Reset de pantalla |
| `LCD_D0` | D8 | Dato bit 0 |
| `LCD_D1` | D9 | Dato bit 1 |
| `LCD_D2` | D2 | Dato bit 2 |
| `LCD_D3` | D3 | Dato bit 3 |
| `LCD_D4` | D4 | Dato bit 4 |
| `LCD_D5` | D5 | Dato bit 5 |
| `LCD_D6` | D6 | Dato bit 6 |
| `LCD_D7` | D7 | Dato bit 7 |
| `5V` | 5V | Alimentacion del shield |
| `GND` | GND | Tierra comun |

## Si sigue blanca

Revisa esto:

1. Que el shield este insertado en un Arduino UNO R3 o compatible ATmega328P.
2. Que en Arduino IDE este seleccionada la placa `Arduino Uno`, no ESP32.
3. Que el monitor serie de `uno_leer_id_tft` no muestre `0xFFFF`, `0x0000`, `0xD3D3` o `0x00D3`.
4. Que los pines del shield no esten doblados o entrando fuera del conector.
5. Que no estes usando Arduino UNO R4. Estas librerias antiguas estan pensadas para UNO R3/ATmega328P.

Si `uno_leer_id_tft` entrega un ID valido diferente de `0x9341`, cambia en el sketch:

```cpp
tft.begin(0x9341);
```

por el ID que salio en el Monitor Serie.

## Caso detectado: ID 0x9328

Tu pantalla respondio:

```text
ID detectado: 0x9328
```

Eso indica que no debes iniciar con `0x9341`. Usa este sketch:

```text
arduino/uno_prueba_00_00_id9328/uno_prueba_00_00_id9328.ino
```

La linea importante es:

```cpp
tft.begin(0x9328);
```

Si esta prueba funciona, los siguientes programas del cronometro para Arduino UNO deben usar tambien `tft.begin(0x9328)`.
