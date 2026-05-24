# Cronometro touch con Arduino UNO

Usa este programa:

```text
arduino/uno_cronometro_touch_id9325/uno_cronometro_touch_id9325.ino
```

La pantalla se lee como `0x9328`, pero se inicializa con:

```cpp
tft.begin(0x9325);
```

## Botones en pantalla

No necesita botones fisicos.

| Boton en pantalla | Funcion |
|---|---|
| `INICIAR` / `PAUSAR` | Arranca o pausa el cronometro |
| `REINICIAR` | Vuelve el tiempo a `00:00.00` |

## Prueba de touch

Antes del cronometro puedes cargar:

```text
arduino/uno_touch_test_id9325/uno_touch_test_id9325.ino
```

Al tocar la pantalla muestra las coordenadas `X`, `Y` y la presion `Z`. Tambien imprime esos datos en el Monitor Serie a `9600`.

## Calibracion usada

Para tu shield se usa:

```cpp
XP = 6
XM = A2
YP = A1
YM = 7
TS_LEFT = 900
TS_RT = 103
TS_TOP = 96
TS_BOT = 904
```

Si tocas un boton y responde en otra zona, hay que ajustar esos cuatro valores `TS_*`.

En tu pantalla el touch quedo invertido en ambos ejes, por eso los sketches usan:

```cpp
INVERT_X = true
INVERT_Y = true
```

Si al probar queda corregido, no cambies nada. Si queda bien arriba/abajo pero mal izquierda/derecha, cambia solo `INVERT_X`. Si queda bien izquierda/derecha pero mal arriba/abajo, cambia solo `INVERT_Y`.

## Sobre pantalla blanca durante la carga

Si al subir un programa aparece `programmer is not responding` o `not in sync`, el codigo nuevo no se cargo. El Arduino se reinicia, corre el programa anterior y por eso la pantalla puede ponerse blanca y redibujarse.

Solucion rapida:

1. Quita el shield TFT.
2. Carga `Blink`.
3. Si `Blink` carga, vuelve a poner el shield.
4. Carga el sketch del cronometro.
5. Verifica `Herramientas > Placa > Arduino Uno` y el puerto COM correcto.
