# Cronometro con Arduino UNO y pantalla ID 0x9328

La pantalla respondio al lector de ID como:

```text
0x9328
```

pero funciono al iniciar con:

```cpp
tft.begin(0x9325);
```

Por eso el cronometro para Arduino UNO esta en:

```text
arduino/uno_cronometro_id9325/uno_cronometro_id9325.ino
```

## Botones

El shield TFT ocupa A0-A4 y D2-D9. Para no chocar con la pantalla, el cronometro usa:

| Funcion | Pin Arduino UNO | Conexion |
|---|---:|---|
| Inicio / pausa | A5 | Boton entre A5 y GND |
| Reinicio | D10 | Boton entre D10 y GND |

El programa usa `INPUT_PULLUP`, asi que el boton se activa conectando el pin a `GND`.

## Nota sobre D10

D10 tambien puede estar conectado al `CS` de la microSD del shield. Para esta etapa no uses microSD. Si luego queremos guardar tiempos en SD, moveremos el boton de reinicio a otro pin o usaremos el tactil.

## Carga

1. Abre `uno_cronometro_id9325.ino`.
2. Selecciona `Arduino Uno`.
3. Selecciona el puerto COM correcto.
4. Sube el programa.
5. Abre el Monitor Serie a `9600` si quieres confirmar el ID leido.
