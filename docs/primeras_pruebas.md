# Primeras pruebas

## 1. Revisar la pantalla

La pantalla parece un shield MCUFriend para Arduino UNO. Estos modulos pueden traer controladores distintos aunque se vean iguales.

El firmware arranca con:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);
```

Si la pantalla queda blanca, negra o con colores incorrectos, prueba cambiar esa linea en `src/main.cpp` por una de estas:

```cpp
Arduino_GFX *gfx = new Arduino_ILI9486(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_ILI9488(bus, TFT_RST, 1, false);
Arduino_GFX *gfx = new Arduino_HX8347D(bus, TFT_RST, 1, false);
```

Compila de nuevo despues de cada cambio.

## 2. Cargar con Arduino IDE

Abre:

```text
arduino/cronometro_pantalla_esp32/cronometro_pantalla_esp32.ino
```

Luego sigue `docs/arduino_ide.md`.

## 3. Compilar con PlatformIO

En VS Code con PlatformIO:

```powershell
pio run
```

## 4. Subir al ESP32 con PlatformIO

```powershell
pio run --target upload
```

Si el ESP32 no entra en modo carga, manten presionado `BOOT` cuando empiece la subida y sueltalo cuando aparezca `Writing`.

## 5. Monitor serial

```powershell
pio device monitor
```

Velocidad: `115200`.

## 6. Problemas comunes

| Sintoma | Posible causa | Que revisar |
|---|---|---|
| Pantalla blanca | Driver incorrecto | Cambiar `Arduino_ILI9341` por otro driver |
| Pantalla apagada | Alimentacion insuficiente | Probar `3V3`, luego `5V` si el shield lo soporta |
| Colores raros | Orden de datos o driver | Revisar `D0` a `D7` y controlador |
| Se reinicia el ESP32 | Corto o exceso de consumo | Revisar alimentacion y cables |
| Boton inestable | Falta pull-up | Agregar resistencia de 10 kOhm a `3V3` |

## 7. Siguiente mejora

Cuando el cronometro funcione, las siguientes mejoras naturales son:

- Agregar boton de vueltas.
- Guardar tiempos en microSD.
- Crear menu tactil si el touch esta conectado.
- Agregar buzzer de salida o cuenta regresiva.
