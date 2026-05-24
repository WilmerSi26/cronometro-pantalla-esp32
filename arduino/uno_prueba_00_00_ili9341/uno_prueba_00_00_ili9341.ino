#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

// Prueba minima para Arduino UNO + shield TFT 2.4" ILI9341.
// Debe mostrar "00:00" sobre fondo negro.

#define BLACK 0x0000
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define RED   0xF800

MCUFRIEND_kbv tft;

void setup() {
  Serial.begin(9600);
  delay(1000);

  uint16_t readId = tft.readID();
  Serial.print("ID leido: 0x");
  Serial.println(readId, HEX);

  // El paquete que agregaste dice ILI9341, por eso forzamos 0x9341.
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("ILI9341 TEST");

  tft.setTextColor(GREEN);
  tft.setTextSize(7);
  tft.setCursor(50, 95);
  tft.print("00:00");

  tft.drawRect(0, 0, tft.width(), tft.height(), RED);
}

void loop() {
}
