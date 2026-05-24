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
| `LCD_RD` | GPIO33 o `3V3` | Lectura desactivada |
| `LCD_D0` | GPIO13 | Dato 0 |
| `LCD_D1` | GPIO14 | Dato 1 |
| `LCD_D2` | GPIO18 | Dato 2 |
| `LCD_D3` | GPIO19 | Dato 3 |
| `LCD_D4` | GPIO21 | Dato 4 |
| `LCD_D5` | GPIO22 | Dato 5 |
| `LCD_D6` | GPIO23 | Dato 6 |
| `LCD_D7` | GPIO25 | Dato 7 |

## Importante

`LCD_RD` puede ir al GPIO33, donde el programa lo mantiene en alto, o directo a `3V3`.

Esta version evita GPIO16 y GPIO17 porque algunas placas ESP32 los usan internamente para memoria PSRAM/flash. Si tu ESP32 es WROOM comun puede que funcionen, pero GPIO13 y GPIO14 son mas seguros para esta prueba.

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
