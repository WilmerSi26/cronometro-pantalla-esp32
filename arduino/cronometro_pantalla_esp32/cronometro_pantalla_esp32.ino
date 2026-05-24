#include <Arduino_GFX_Library.h>

// Cronometro para ESP32 + pantalla TFT tipo MCUFriend/Arduino UNO shield.
// Pantalla con bus paralelo de 8 bits: LCD_D0..LCD_D7.
// Instala en Arduino IDE la libreria "GFX Library for Arduino" de moononournation.

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

const int BTN_START_STOP = 13;
const int BTN_RESET = 14;

const uint16_t COLOR_BG = 0x0000;
const uint16_t COLOR_PANEL = 0x18E3;
const uint16_t COLOR_TEXT = 0xFFFF;
const uint16_t COLOR_MUTED = 0x8410;
const uint16_t COLOR_ACCENT = 0x07E0;
const uint16_t COLOR_WARN = 0xFBE0;

Arduino_DataBus *bus = new Arduino_ESP32PAR8(
  TFT_DC, TFT_CS, TFT_WR, TFT_RD,
  TFT_D0, TFT_D1, TFT_D2, TFT_D3,
  TFT_D4, TFT_D5, TFT_D6, TFT_D7
);

// Si la pantalla queda blanca o con colores raros, cambia este driver.
// Opciones comunes: Arduino_ILI9486, Arduino_ILI9488, Arduino_HX8347D.
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);

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

String formatElapsed(unsigned long valueMs) {
  unsigned long totalSeconds = valueMs / 1000;
  unsigned int centiseconds = (valueMs % 1000) / 10;
  unsigned int seconds = totalSeconds % 60;
  unsigned int minutes = (totalSeconds / 60) % 100;

  char buffer[12];
  snprintf(buffer, sizeof(buffer), "%02u:%02u.%02u", minutes, seconds, centiseconds);
  return String(buffer);
}

void drawHeader() {
  gfx->fillScreen(COLOR_BG);
  gfx->fillRect(0, 0, gfx->width(), 36, COLOR_PANEL);
  gfx->setTextColor(COLOR_TEXT);
  gfx->setTextSize(2);
  gfx->setCursor(10, 10);
  gfx->print("CRONOMETRO ESP32");
}

void drawStaticUi() {
  drawHeader();

  gfx->setTextSize(2);
  gfx->setTextColor(COLOR_MUTED);
  gfx->setCursor(16, gfx->height() - 58);
  gfx->print("BTN1 inicio/pausa");

  gfx->setCursor(16, gfx->height() - 32);
  gfx->print("BTN2 reinicio");
}

void drawTimer(bool force = false) {
  unsigned long now = millis();
  if (!force && (now - lastDrawMs) < 40) {
    return;
  }
  lastDrawMs = now;

  gfx->fillRect(0, 64, gfx->width(), 92, COLOR_BG);
  gfx->setTextColor(COLOR_ACCENT);
  gfx->setTextSize(5);
  gfx->setCursor(14, 86);
  gfx->print(formatElapsed(elapsedMs()));

  gfx->fillRect(0, 164, gfx->width(), 32, COLOR_BG);
  gfx->setTextColor(running ? COLOR_ACCENT : COLOR_WARN);
  gfx->setTextSize(2);
  gfx->setCursor(16, 170);
  gfx->print(running ? "EN MARCHA" : "PAUSADO");
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
  Serial.begin(115200);

  pinMode(BTN_START_STOP, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  if (!gfx->begin()) {
    Serial.println("No se pudo iniciar la pantalla.");
    while (true) {
      delay(1000);
    }
  }

  gfx->setRotation(1);
  drawStaticUi();
  drawTimer(true);
}

void loop() {
  if (buttonPressed(startButton)) {
    toggleTimer();
    drawTimer(true);
  }

  if (buttonPressed(resetButton)) {
    resetTimer();
    drawTimer(true);
  }

  drawTimer();
}
