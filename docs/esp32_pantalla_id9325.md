# ESP32 con pantalla TFT ID 0x9328 / init 0x9325

Tu proyecto final puede ir con ESP32 usando la misma pantalla, pero no es I2C. La pantalla usa bus paralelo de 8 bits.

## Prueba inicial

Carga este sketch:

```text
arduino/esp32_prueba_00_00_id9325/esp32_prueba_00_00_id9325.ino
```

Debe mostrar:

```text
00:00
```

## Conexiones ESP32

| Pantalla | ESP32 | Funcion |
|---|---:|---|
| `GND` | `GND` | Tierra comun |
| `5V` | `5V/VIN` | Alimentacion del shield |
| `LCD_RST` | GPIO4 | Reset |
| `LCD_CS` | GPIO26 | Seleccion de pantalla |
| `LCD_RS` | GPIO27 | Comando/dato |
| `LCD_WR` | GPIO32 | Escritura |
| `LCD_RD` | `3V3` | Lectura desactivada |
| `LCD_D0` | GPIO16 | Dato 0 |
| `LCD_D1` | GPIO17 | Dato 1 |
| `LCD_D2` | GPIO18 | Dato 2 |
| `LCD_D3` | GPIO19 | Dato 3 |
| `LCD_D4` | GPIO21 | Dato 4 |
| `LCD_D5` | GPIO22 | Dato 5 |
| `LCD_D6` | GPIO23 | Dato 6 |
| `LCD_D7` | GPIO25 | Dato 7 |

## Importante

`LCD_RD` no va al ESP32 en esta prueba. Conectalo directo a `3V3`.

Los pines de microSD:

```text
SD_SS, SD_DI, SD_DO, SD_SCK
```

son SPI de la tarjeta SD, no I2C de la pantalla.

## Libreria

Este sketch usa solo:

```cpp
#include <Adafruit_GFX.h>
```

La inicializacion `ILI9325` fue portada directamente al sketch porque `GFX Library for Arduino` no trae un driver `Arduino_ILI9325`.
