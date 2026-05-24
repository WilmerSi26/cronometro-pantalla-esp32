# Cronometro con ESP32 y pantalla TFT MCUFriend

Proyecto base para construir un cronometro usando un ESP32 y una pantalla TFT tipo shield Arduino UNO de MCUFriend, con bus paralelo de 8 bits.

La pantalla de las fotos tiene pines como `LCD_RST`, `LCD_CS`, `LCD_RS`, `LCD_WR`, `LCD_RD`, `LCD_D0` a `LCD_D7` y pines para microSD. Este repositorio arranca con la pantalla, dos botones fisicos y una interfaz simple de cronometro.

## Objetivo inicial

- Encender la pantalla desde el ESP32.
- Mostrar un cronometro en formato `MM:SS.cc`.
- Usar un boton para iniciar/pausar.
- Usar un boton para reiniciar.
- Dejar preparada la base para vueltas, menu tactil o registro en microSD.

## Estructura

```text
.
|-- arduino/
|   `-- cronometro_pantalla_esp32/
|       `-- cronometro_pantalla_esp32.ino
|-- platformio.ini
|-- src/
|   `-- main.cpp
|-- docs/
|   |-- arduino_ide.md
|   |-- conexion_pantalla_esp32.md
|   |-- primeras_pruebas.md
|   `-- github.md
`-- hardware/
    `-- pinout_esp32_mcu_friend.csv
```

## Software recomendado

### Opcion A: Arduino IDE

1. Instala Arduino IDE.
2. Instala el soporte ESP32.
3. Instala la libreria `GFX Library for Arduino`.
4. Abre `arduino/cronometro_pantalla_esp32/cronometro_pantalla_esp32.ino`.
5. Selecciona `ESP32 Dev Module`.
6. Compila y sube.

La guia paso a paso esta en `docs/arduino_ide.md`.

### Opcion B: PlatformIO

1. Instala VS Code.
2. Instala la extension PlatformIO IDE.
3. Abre esta carpeta como proyecto.
4. Conecta el ESP32 por USB.
5. Compila y sube con PlatformIO.

## Libreria de pantalla

Este proyecto usa:

- `Arduino_GFX_Library`

El controlador exacto de la pantalla puede variar entre `ILI9341`, `ILI9486`, `ILI9488`, `HX8347`, etc. El firmware queda configurado inicialmente para `ILI9341`, que es muy comun en estos shields. Si la pantalla queda blanca o con colores raros, revisa `docs/primeras_pruebas.md`.

## Pinout inicial

El pinout propuesto esta en `docs/conexion_pantalla_esp32.md` y en `hardware/pinout_esp32_mcu_friend.csv`.

Importante:

- Une siempre `GND` de la pantalla con `GND` del ESP32.
- Empieza alimentando la pantalla desde `3V3` si tu modulo lo permite.
- Si usas `5V` para la pantalla, no metas senales de 5V al ESP32.
- No conectes la microSD todavia; primero confirma que el TFT funciona.

## Git

Este proyecto ya esta preparado para Git. Para conectarlo a GitHub mira `docs/github.md`.
