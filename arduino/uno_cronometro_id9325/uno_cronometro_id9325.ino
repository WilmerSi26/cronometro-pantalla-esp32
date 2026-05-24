#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

// Cronometro para Arduino UNO + shield TFT MCUFriend.
// La pantalla leyo ID 0x9328, pero funciona inicializando como 0x9325.

#define BLACK  0x0000
#define GREEN  0x07E0
#define WHITE  0xFFFF
#define YELLOW 0xFFE0
#define CYAN   0x07FF
#define RED    0xF800

const uint16_t TFT_DRIVER_ID = 0x9325;

const int BTN_START_STOP = A5;
const int BTN_RESET = 10;

MCUFRIEND_kbv tft;

struct DebouncedButton {
  int pin;
  bool stableState;
  bool lastReading;
  unsigned long lastChangeMs;
};

DebouncedButton startButton = {BTN_START_STOP, HIGH, HIGH, 0};
DebouncedButton resetButton = {BTN_RESET, HIGH, HIGH, 0};

bool running = false;
unsigned long startedAtMs = 0;
unsigned long accumulatedMs = 0;
unsigned long lastDrawMs = 0;
unsigned long lastDisplayedCentiseconds = 999999;

bool buttonPressed(DebouncedButton &button) {
  bool reading = digitalRead(button.pin);
  unsigned long now = millis();

  if (reading != button.lastReading) {
    button.lastChangeMs = now;
    button.lastReading = reading;
  }

  if ((now - button.lastChangeMs) > 35 && reading != button.stableState) {
    button.stableState = reading;
    return button.stableState == LOW;
  }

  return false;
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

void drawHeader() {
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, tft.width(), 34, CYAN);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 9);
  tft.print("CRONOMETRO");
}

void drawHelp() {
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(10, 200);
  tft.print("A5: iniciar/pausar");

  tft.setCursor(10, 222);
  tft.print("D10: reiniciar");
}

void drawStatus() {
  tft.fillRect(0, 162, tft.width(), 28, BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 168);

  if (running) {
    tft.setTextColor(GREEN, BLACK);
    tft.print("EN MARCHA");
  } else {
    tft.setTextColor(YELLOW, BLACK);
    tft.print("PAUSADO");
  }
}

void drawTimer(bool force = false) {
  unsigned long now = millis();
  unsigned long currentCentiseconds = elapsedMs() / 10;

  if (!force && currentCentiseconds == lastDisplayedCentiseconds) {
    return;
  }

  if (!force && (now - lastDrawMs) < 30) {
    return;
  }

  lastDrawMs = now;
  lastDisplayedCentiseconds = currentCentiseconds;

  char text[12];
  formatElapsed(elapsedMs(), text, sizeof(text));

  tft.fillRect(0, 70, tft.width(), 78, BLACK);
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(5);
  tft.setCursor(16, 92);
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
}

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(BTN_START_STOP, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  uint16_t readId = tft.readID();
  Serial.print("ID leido: 0x");
  Serial.println(readId, HEX);
  Serial.println("Iniciando TFT con 0x9325");

  tft.begin(TFT_DRIVER_ID);
  tft.setRotation(1);

  drawHeader();
  drawTimer(true);
  drawStatus();
  drawHelp();
  tft.drawRect(0, 0, tft.width(), tft.height(), RED);
}

void loop() {
  if (buttonPressed(startButton)) {
    toggleTimer();
    drawStatus();
    drawTimer(true);
  }

  if (buttonPressed(resetButton)) {
    resetTimer();
    drawStatus();
    drawTimer(true);
  }

  drawTimer();
}
