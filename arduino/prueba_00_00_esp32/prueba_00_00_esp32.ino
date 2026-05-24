#include <Arduino_GFX_Library.h>

// Prueba minima para ESP32 + pantalla TFT MCUFriend.
// Debe mostrar solamente "00:00" en el centro de la pantalla.

const int TFT_CS = 26;   // LCD_CS
const int TFT_DC = 27;   // LCD_RS / DC
const int TFT_WR = 32;   // LCD_WR
const int TFT_RD = 33;   // LCD_RD
const int TFT_RST = 4;   // LCD_RST

const int TFT_D0 = 16;
const int TFT_D1 = 17;
const int TFT_D2 = 18;
const int TFT_D3 = 19;
const int TFT_D4 = 21;
const int TFT_D5 = 22;
const int TFT_D6 = 23;
const int TFT_D7 = 25;

const uint16_t COLOR_BG = 0x0000;
const uint16_t COLOR_TEXT = 0x07E0;

Arduino_DataBus *bus = new Arduino_ESP32PAR8(
  TFT_DC, TFT_CS, TFT_WR, TFT_RD,
  TFT_D0, TFT_D1, TFT_D2, TFT_D3,
  TFT_D4, TFT_D5, TFT_D6, TFT_D7
);

// Si la pantalla queda blanca, prueba Arduino_ILI9486 o Arduino_ILI9488.
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);

void setup() {
  Serial.begin(115200);

  if (!gfx->begin()) {
    Serial.println("No se pudo iniciar la pantalla.");
    while (true) {
      delay(1000);
    }
  }

  gfx->setRotation(1);
  gfx->fillScreen(COLOR_BG);
  gfx->setTextColor(COLOR_TEXT);
  gfx->setTextSize(7);

  const char *text = "00:00";
  const int textSize = 7;
  const int textWidth = 5 * 6 * textSize;
  const int textHeight = 8 * textSize;
  int16_t x = (gfx->width() - textWidth) / 2;
  int16_t y = (gfx->height() - textHeight) / 2;
  gfx->setCursor(x, y);
  gfx->print(text);
}

void loop() {
}
