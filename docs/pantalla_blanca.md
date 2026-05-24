# Diagnostico: pantalla blanca brillante

Si la pantalla queda blanca brillante, la retroiluminacion esta encendida, pero el controlador TFT no esta recibiendo comandos validos.

## Prueba 1: LCD_RD fijo a 3V3

Haz este cambio fisico:

| Pantalla | Conexion |
|---|---|
| `LCD_RD` | `3V3` del ESP32 |

No conectes `LCD_RD` a GPIO33 para esta prueba.

`LCD_RD` es la senal de lectura. Como solo vamos a escribir en pantalla, debe permanecer en nivel alto. Si queda flotando o en bajo, muchos shields se quedan blancos.

El sketch `arduino/prueba_00_00_esp32/prueba_00_00_esp32.ino` ya esta preparado para esta prueba usando:

```cpp
const int TFT_RD = GFX_NOT_DEFINED;
```

## Prueba 2: confirmar alimentacion

| Pantalla | ESP32 | Funcion |
|---|---|---|
| `GND` | `GND` | Tierra comun |
| `5V` | `5V/VIN` | Alimenta backlight y regulador del shield |

En estos shields tipo Arduino UNO normalmente se usa `5V`. El pin `3V3` puede ser salida del regulador del shield o alimentacion logica, segun la version. No alimentes `5V` y `3V3` al mismo tiempo si no estas seguro.

## Prueba 3: senales de control

| Pantalla | ESP32 | Que hace |
|---|---:|---|
| `LCD_RST` | GPIO4 | Reinicia el controlador TFT |
| `LCD_CS` | GPIO26 | Habilita la pantalla |
| `LCD_RS` | GPIO27 | Selecciona comando o dato |
| `LCD_WR` | GPIO32 | Pulso para escribir datos |
| `LCD_RD` | 3V3 | Lectura desactivada |

Si `LCD_CS`, `LCD_RS` o `LCD_WR` estan intercambiados, la pantalla queda blanca.

## Prueba 4: bus de datos

| Pantalla | ESP32 |
|---|---:|
| `LCD_D0` | GPIO16 |
| `LCD_D1` | GPIO17 |
| `LCD_D2` | GPIO18 |
| `LCD_D3` | GPIO19 |
| `LCD_D4` | GPIO21 |
| `LCD_D5` | GPIO22 |
| `LCD_D6` | GPIO23 |
| `LCD_D7` | GPIO25 |

Si un dato esta cruzado, normalmente se ven colores raros o basura. Si todo esta blanco, sospecha primero de `RST`, `CS`, `RS`, `WR`, `RD` o del controlador incorrecto.

## Prueba 5: cambiar controlador

En el `.ino`, cambia esta linea:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9488(bus, TFT_RST, 1, false);
```

Prueba una por una:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_ILI9486(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_ILI9488(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_HX8347D(bus, TFT_RST, 1, false);
```

Sube el programa despues de cada cambio.

## Nota importante sobre logica de 3.3 V

El ESP32 trabaja con senales de 3.3 V. Si el shield espera senales tipo Arduino UNO de 5 V, puede que no reconozca los niveles altos del ESP32. Si todo lo anterior falla, conviene probar el shield primero con un Arduino UNO para leer el ID del controlador o usar conversores de nivel.
