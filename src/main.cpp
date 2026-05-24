#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// Pantalla tipo MCUFriend/Arduino UNO shield con bus paralelo 8 bits.
// Ajusta estos pines si tu cableado cambia.
constexpr int TFT_CS = 26;   // LCD_CS
constexpr int TFT_DC = 27;   // LCD_RS / DC
constexpr int TFT_WR = 32;   // LCD_WR
constexpr int TFT_RD = 33;   // LCD_RD
constexpr int TFT_RST = 4;   // LCD_RST

constexpr int TFT_D0 = 16;
constexpr int TFT_D1 = 17;
constexpr int TFT_D2 = 18;
constexpr int TFT_D3 = 19;
constexpr int TFT_D4 = 21;
constexpr int TFT_D5 = 22;
constexpr int TFT_D6 = 23;
constexpr int TFT_D7 = 25;

constexpr int BTN_START_STOP = 13;
constexpr int BTN_RESET = 14;

constexpr uint16_t COLOR_BG = 0x0000;
constexpr uint16_t COLOR_PANEL = 0x18E3;
constexpr uint16_t COLOR_TEXT = 0xFFFF;
constexpr uint16_t COLOR_MUTED = 0x8410;
constexpr uint16_t COLOR_ACCENT = 0x07E0;
constexpr uint16_t COLOR_WARN = 0xFBE0;

Arduino_DataBus *bus = new Arduino_ESP32PAR8(
    TFT_DC, TFT_CS, TFT_WR, TFT_RD,
    TFT_D0, TFT_D1, TFT_D2, TFT_D3, TFT_D4, TFT_D5, TFT_D6, TFT_D7);

// Si tu pantalla no responde, cambia este constructor por otro driver compatible:
// Arduino_ILI9486, Arduino_ILI9488, Arduino_HX8347D, etc.
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1, false);

struct DebouncedButton {
  int pin;
  bool stableState = HIGH;
  bool lastReading = HIGH;
  unsigned long lastChangeMs = 0;
};

DebouncedButton startButton{BTN_START_STOP};
DebouncedButton resetButton{BTN_RESET};

bool running = false;
unsigned long startedAtMs = 0;
unsigned long accumulatedMs = 0;
unsigned long lastDrawMs = 0;

bool buttonPressed(DebouncedButton &button) {
  const bool reading = digitalRead(button.pin);
  const unsigned long now = millis();

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
  const unsigned long totalSeconds = valueMs / 1000;
  const unsigned int centiseconds = (valueMs % 1000) / 10;
  const unsigned int seconds = totalSeconds % 60;
  const unsigned int minutes = (totalSeconds / 60) % 100;

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
  const unsigned long now = millis();
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
