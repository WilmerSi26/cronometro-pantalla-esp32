# Diagnostico ESP32: leer ID de la pantalla

Si la pantalla queda blanca con ESP32, primero hay que confirmar si el ESP32 puede leer el controlador.

Carga:

```text
arduino/esp32_leer_id_tft/esp32_leer_id_tft.ino
```

## Conexion para esta prueba

En esta prueba `LCD_RD` si debe ir conectado al ESP32:

| Pantalla | ESP32 |
|---|---:|
| `LCD_RD` | GPIO33 |

El resto:

| Pantalla | ESP32 |
|---|---:|
| `LCD_RST` | GPIO4 |
| `LCD_CS` | GPIO26 |
| `LCD_RS` | GPIO27 |
| `LCD_WR` | GPIO32 |
| `LCD_D0` | GPIO13 |
| `LCD_D1` | GPIO14 |
| `LCD_D2` | GPIO18 |
| `LCD_D3` | GPIO19 |
| `LCD_D4` | GPIO21 |
| `LCD_D5` | GPIO22 |
| `LCD_D6` | GPIO23 |
| `LCD_D7` | GPIO25 |

Abre el Monitor Serie a:

```text
115200 baudios
```

## Como interpretar

Si ves algo parecido a:

```text
reg(0x00): 93 28
```

el bus esta bien y podemos ajustar la escritura/inicializacion.

Si ves:

```text
00 00
FF FF
D3 D3
```

o valores fijos sin sentido, el problema es cableado, niveles logicos o alimentacion.

## Nota importante sobre niveles logicos

El Arduino UNO entrega senales de 5 V. El ESP32 entrega senales de 3.3 V.

Si el shield alimenta sus buffers `74HC245` con 5 V, puede que el ESP32 no alcance el nivel alto requerido y la pantalla no responda, aunque el mismo shield funcione perfecto en Arduino UNO.

En ese caso hay dos soluciones:

1. Alimentar la logica del shield a 3.3 V si el modulo lo permite.
2. Usar conversores de nivel de 3.3 V a 5 V para las senales `LCD_*`.
