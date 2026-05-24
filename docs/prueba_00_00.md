# Prueba inicial: mostrar 00:00

Antes de probar botones o cronometro, carga este sketch:

```text
arduino/prueba_00_00_esp32/prueba_00_00_esp32.ino
```

El objetivo es confirmar tres cosas:

1. El ESP32 sube el programa correctamente.
2. La pantalla esta alimentada.
3. El bus paralelo `LCD_D0` a `LCD_D7` y las senales de control estan bien conectadas.

## Resultado esperado

La pantalla debe encender y mostrar:

```text
00:00
```

en color verde sobre fondo negro.

## Si no aparece

En el archivo `.ino`, cambia esta linea:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);
```

Prueba una de estas:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9486(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_ILI9488(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_HX8347D(bus, TFT_RST, 1, false);
```

Sube el programa despues de cada cambio.

## Si aparece girado

Cambia:

```cpp
gfx->setRotation(1);
```

por `0`, `2` o `3`.
