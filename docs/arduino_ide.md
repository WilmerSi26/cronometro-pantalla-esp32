# Cargar el programa con Arduino IDE

Este documento es para cargar el cronometro directamente desde Arduino IDE al ESP32.

## 1. Instalar soporte para ESP32

1. Abre Arduino IDE.
2. Ve a `Archivo > Preferencias`.
3. En `Gestor de URLs adicionales de tarjetas` agrega:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

4. Ve a `Herramientas > Placa > Gestor de tarjetas`.
5. Busca `esp32`.
6. Instala `esp32 by Espressif Systems`.

## 2. Instalar libreria de pantalla

1. Ve a `Programa > Incluir Libreria > Administrar bibliotecas`.
2. Busca:

```text
GFX Library for Arduino
```

3. Instala la libreria de `moononournation`.

## 3. Abrir primero la prueba 00:00

Abre este archivo:

```text
arduino/prueba_00_00_esp32/prueba_00_00_esp32.ino
```

Esta version solo muestra `00:00`. Sirve para confirmar que el cableado y el controlador de pantalla funcionan.

## 4. Abrir luego el cronometro completo

Abre este archivo:

```text
arduino/cronometro_pantalla_esp32/cronometro_pantalla_esp32.ino
```

La carpeta y el archivo deben tener el mismo nombre. Arduino IDE lo necesita asi.

## 5. Seleccionar placa

En Arduino IDE selecciona:

```text
Herramientas > Placa > esp32 > ESP32 Dev Module
```

Configuracion recomendada:

| Opcion | Valor |
|---|---|
| Upload Speed | 115200 o 921600 |
| CPU Frequency | 240 MHz |
| Flash Frequency | 80 MHz |
| Flash Mode | QIO |
| Partition Scheme | Default |

## 6. Seleccionar puerto

Conecta el ESP32 por USB y selecciona:

```text
Herramientas > Puerto
```

Si no aparece el puerto, probablemente falta el driver USB del chip de tu placa, comunmente CH340 o CP210x.

## 7. Subir

Presiona `Subir`.

Si aparece `Connecting...` y no avanza:

1. Manten presionado el boton `BOOT` del ESP32.
2. Cuando Arduino IDE empiece a escribir, suelta `BOOT`.
3. Si no reinicia solo, presiona `EN` o `RESET`.

## 8. Si la pantalla no muestra imagen

En el archivo `.ino`, busca esta linea:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);
```

Prueba uno por uno estos controladores:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9486(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_ILI9488(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_HX8347D(bus, TFT_RST, 1, false);
```

Despues de cada cambio, vuelve a subir el programa.

## 9. Botones

Los botones van conectados asi:

| Funcion | Conexion |
|---|---|
| Inicio / pausa | GPIO13 a GND |
| Reinicio | GPIO14 a GND |

No necesitan resistencia externa para empezar, porque el programa usa `INPUT_PULLUP`.
