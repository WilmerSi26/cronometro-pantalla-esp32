# Conexion pantalla TFT MCUFriend con ESP32

La placa de la foto es un shield TFT para Arduino UNO. En lugar de SPI usa un bus paralelo de 8 bits, por eso necesita muchos pines del ESP32.

## Alimentacion

| Pantalla | ESP32 | Nota |
|---|---:|---|
| `GND` | `GND` | Tierra comun obligatoria |
| `3V3` | `3V3` | Primera prueba recomendada |
| `5V` | `5V/VIN` | Solo si el brillo no alcanza con 3V3 y el shield lo soporta |

No conectes senales de 5V a ningun GPIO del ESP32. El ESP32 trabaja a 3.3 V.

## Bus TFT propuesto

| Pin pantalla | Pin ESP32 | Nombre en codigo |
|---|---:|---|
| `LCD_RST` | GPIO4 | `TFT_RST` |
| `LCD_CS` | GPIO26 | `TFT_CS` |
| `LCD_RS` | GPIO27 | `TFT_DC` |
| `LCD_WR` | GPIO32 | `TFT_WR` |
| `LCD_RD` | GPIO33 | `TFT_RD` |
| `LCD_D0` | GPIO16 | `TFT_D0` |
| `LCD_D1` | GPIO17 | `TFT_D1` |
| `LCD_D2` | GPIO18 | `TFT_D2` |
| `LCD_D3` | GPIO19 | `TFT_D3` |
| `LCD_D4` | GPIO21 | `TFT_D4` |
| `LCD_D5` | GPIO22 | `TFT_D5` |
| `LCD_D6` | GPIO23 | `TFT_D6` |
| `LCD_D7` | GPIO25 | `TFT_D7` |

## Botones

Los botones van entre el GPIO y `GND`. El programa usa `INPUT_PULLUP`.

| Funcion | Pin ESP32 |
|---|---:|
| Inicio / pausa | GPIO13 |
| Reinicio | GPIO14 |

Cada boton va entre el GPIO y `GND`. El programa activa `INPUT_PULLUP`, asi que no necesitas resistencia externa para la primera prueba.

## Pines microSD

La pantalla trae pines `SD_SS`, `SD_DI`, `SD_DO` y `SD_SCK`. No los conectes en la primera etapa. Primero confirma que el TFT dibuja correctamente.

## Recomendacion practica

1. Conecta solo alimentacion, tierra y bus TFT.
2. Sube el firmware.
3. Si aparece el cronometro, agrega los botones.
4. Si la pantalla queda blanca, revisa controlador y cableado.
