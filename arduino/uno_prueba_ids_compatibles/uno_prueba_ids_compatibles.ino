#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

// Prueba rotativa para Arduino UNO + shield TFT.
// Cambia automaticamente entre controladores compatibles.
// Si aparece imagen, anota el ID que se muestra arriba.

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

MCUFRIEND_kbv tft;

const uint16_t ids[] = {
  0x9325,
  0x9320,
  0x9329,
  0x9341,
  0x9486,
  0x9488
};

const char *names[] = {
  "ILI9325",
  "ILI9320",
  "ILI9329",
  "ILI9341",
  "ILI9486",
  "ILI9488"
};

void drawTest(uint16_t id, const char *name, uint8_t index) {
  Serial.print("Probando init 0x");
  Serial.print(id, HEX);
  Serial.print(" ");
  Serial.println(name);

  tft.reset();
  delay(100);
  tft.begin(id);
  tft.setRotation(1);

  tft.fillScreen(BLACK);
  delay(250);
  tft.fillScreen(RED);
  delay(250);
  tft.fillScreen(GREEN);
  delay(250);
  tft.fillScreen(BLUE);
  delay(250);
  tft.fillScreen(BLACK);

  tft.drawRect(0, 0, tft.width(), tft.height(), YELLOW);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("INIT 0x");
  tft.print(id, HEX);
  tft.print(" ");
  tft.print(name);

  tft.setTextColor(CYAN);
  tft.setTextSize(3);
  tft.setCursor(10, 45);
  tft.print("Prueba ");
  tft.print(index + 1);

  tft.setTextColor(GREEN);
  tft.setTextSize(7);
  tft.setCursor(50, 105);
  tft.print("00:00");
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  uint16_t readId = tft.readID();
  Serial.print("ID leido por la pantalla: 0x");
  Serial.println(readId, HEX);
  Serial.println("Observa la pantalla. Si aparece imagen, anota el INIT mostrado.");
}

void loop() {
  for (uint8_t i = 0; i < sizeof(ids) / sizeof(ids[0]); i++) {
    drawTest(ids[i], names[i], i);
    delay(4000);
  }
}
