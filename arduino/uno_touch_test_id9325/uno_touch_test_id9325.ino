#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// Prueba de touch para Arduino UNO + shield TFT.
// Pantalla: lee 0x9328, funciona con inicializacion 0x9325.

#define BLACK  0x0000
#define BLUE   0x001F
#define GREEN  0x07E0
#define WHITE  0xFFFF
#define YELLOW 0xFFE0
#define RED    0xF800

const uint16_t TFT_DRIVER_ID = 0x9325;

const int XP = 6;
const int XM = A2;
const int YP = A1;
const int YM = 7;

const int TS_LEFT = 900;
const int TS_RT = 103;
const int TS_TOP = 96;
const int TS_BOT = 904;

const bool INVERT_X = true;
const bool INVERT_Y = true;
const int TOUCH_OFFSET_X = -18;
const int TOUCH_OFFSET_Y = 18;

const int MINPRESSURE = 20;
const int MAXPRESSURE = 1000;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void restoreTftPins() {
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
}

bool readTouch(int &x, int &y, int &z) {
  TSPoint p = ts.getPoint();
  restoreTftPins();

  z = p.z;
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
    return false;
  }

  x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
  y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  x = constrain(x, 0, tft.width() - 1);
  y = constrain(y, 0, tft.height() - 1);

  if (INVERT_X) {
    x = tft.width() - 1 - x;
  }

  if (INVERT_Y) {
    y = tft.height() - 1 - y;
  }

  x = constrain(x + TOUCH_OFFSET_X, 0, tft.width() - 1);
  y = constrain(y + TOUCH_OFFSET_Y, 0, tft.height() - 1);

  return true;
}

void setup() {
  Serial.begin(9600);
  delay(500);

  tft.begin(TFT_DRIVER_ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("TOUCH TEST");

  tft.setTextSize(1);
  tft.setCursor(10, 42);
  tft.print("Toca la pantalla");
  tft.drawRect(0, 0, tft.width(), tft.height(), BLUE);
}

void loop() {
  int x, y, z;
  if (readTouch(x, y, z)) {
    Serial.print("x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.print(y);
    Serial.print(" z=");
    Serial.println(z);

    tft.fillRect(10, 70, 220, 42, BLACK);
    tft.setTextColor(YELLOW, BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 70);
    tft.print("X:");
    tft.print(x);
    tft.print(" Y:");
    tft.print(y);
    tft.setCursor(10, 94);
    tft.print("Z:");
    tft.print(z);

    tft.fillCircle(x, y, 4, GREEN);
    delay(80);
  }
}
