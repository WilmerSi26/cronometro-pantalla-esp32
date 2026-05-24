#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// Cronometro con botones tactiles para Arduino UNO + shield TFT.
// Pantalla: lee 0x9328, funciona con inicializacion 0x9325.

#define BLACK    0x0000
#define BLUE     0x001F
#define GREEN    0x07E0
#define CYAN     0x07FF
#define RED      0xF800
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define DARKGRAY 0x4208

const uint16_t TFT_DRIVER_ID = 0x9325;

const int XP = 6;
const int XM = A2;
const int YP = A1;
const int YM = 7;

const int TS_LEFT = 900;
const int TS_RT = 103;
const int TS_TOP = 96;
const int TS_BOT = 904;

const int MINPRESSURE = 20;
const int MAXPRESSURE = 1000;

const int START_X = 20;
const int START_Y = 208;
const int START_W = 200;
const int START_H = 44;

const int RESET_X = 20;
const int RESET_Y = 264;
const int RESET_W = 200;
const int RESET_H = 44;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

bool running = false;
unsigned long startedAtMs = 0;
unsigned long accumulatedMs = 0;
unsigned long lastDrawMs = 0;
unsigned long lastDisplayedCentiseconds = 999999;
unsigned long lastTouchActionMs = 0;

void restoreTftPins() {
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
}

bool readTouch(int &x, int &y) {
  TSPoint p = ts.getPoint();
  restoreTftPins();

  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
    return false;
  }

  x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
  y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  x = constrain(x, 0, tft.width() - 1);
  y = constrain(y, 0, tft.height() - 1);
  return true;
}

bool insideButton(int x, int y, int bx, int by, int bw, int bh) {
  return x >= bx && x < (bx + bw) && y >= by && y < (by + bh);
}

unsigned long elapsedMs() {
  if (!running) {
    return accumulatedMs;
  }

  return accumulatedMs + (millis() - startedAtMs);
}

void formatElapsed(unsigned long valueMs, char *buffer, size_t bufferSize) {
  unsigned long totalSeconds = valueMs / 1000;
  unsigned int seconds = totalSeconds % 60;
  unsigned int minutes = (totalSeconds / 60) % 100;
  unsigned int centiseconds = (valueMs % 1000) / 10;

  snprintf(buffer, bufferSize, "%02u:%02u.%02u", minutes, seconds, centiseconds);
}

void drawButton(int x, int y, int w, int h, uint16_t fill, const char *label) {
  tft.fillRoundRect(x, y, w, h, 6, fill);
  tft.drawRoundRect(x, y, w, h, 6, WHITE);
  tft.setTextColor(WHITE, fill);
  tft.setTextSize(2);

  int textWidth = strlen(label) * 12;
  int cursorX = x + (w - textWidth) / 2;
  int cursorY = y + 14;
  tft.setCursor(cursorX, cursorY);
  tft.print(label);
}

void drawHeader() {
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, tft.width(), 34, CYAN);
  tft.setTextColor(BLACK, CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 9);
  tft.print("CRONOMETRO TOUCH");
}

void drawStatus() {
  tft.fillRect(0, 154, tft.width(), 32, BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 162);

  if (running) {
    tft.setTextColor(GREEN, BLACK);
    tft.print("EN MARCHA");
  } else {
    tft.setTextColor(YELLOW, BLACK);
    tft.print("PAUSADO");
  }

  drawButton(START_X, START_Y, START_W, START_H, running ? BLUE : GREEN, running ? "PAUSAR" : "INICIAR");
}

void drawStaticUi() {
  drawHeader();
  tft.drawRect(0, 0, tft.width(), tft.height(), DARKGRAY);
  drawButton(RESET_X, RESET_Y, RESET_W, RESET_H, RED, "REINICIAR");
}

void drawTimer(bool force = false) {
  unsigned long now = millis();
  unsigned long currentCentiseconds = elapsedMs() / 10;

  if (!force && currentCentiseconds == lastDisplayedCentiseconds) {
    return;
  }

  if (!force && (now - lastDrawMs) < 35) {
    return;
  }

  lastDrawMs = now;
  lastDisplayedCentiseconds = currentCentiseconds;

  char text[12];
  formatElapsed(elapsedMs(), text, sizeof(text));

  tft.fillRect(0, 66, tft.width(), 70, BLACK);
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(4);
  tft.setCursor(24, 88);
  tft.print(text);
}

void toggleTimer() {
  if (running) {
    accumulatedMs += millis() - startedAtMs;
    running = false;
  } else {
    startedAtMs = millis();
    running = true;
  }
}

void resetTimer() {
  running = false;
  startedAtMs = 0;
  accumulatedMs = 0;
  lastDisplayedCentiseconds = 999999;
}

void setup() {
  Serial.begin(9600);
  delay(500);

  uint16_t readId = tft.readID();
  Serial.print("ID leido: 0x");
  Serial.println(readId, HEX);
  Serial.println("Iniciando TFT con 0x9325 y touch resistivo");

  tft.begin(TFT_DRIVER_ID);
  tft.setRotation(0);

  drawStaticUi();
  drawTimer(true);
  drawStatus();
}

void loop() {
  int x, y;
  if (readTouch(x, y) && (millis() - lastTouchActionMs > 300)) {
    lastTouchActionMs = millis();
    Serial.print("Touch x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.println(y);

    if (insideButton(x, y, START_X, START_Y, START_W, START_H)) {
      toggleTimer();
      drawStatus();
      drawTimer(true);
    } else if (insideButton(x, y, RESET_X, RESET_Y, RESET_W, RESET_H)) {
      resetTimer();
      drawStatus();
      drawTimer(true);
    }
  }

  drawTimer();
}
