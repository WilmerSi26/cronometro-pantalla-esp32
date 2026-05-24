#include <Adafruit_GFX.h>

// Prueba para ESP32 + shield TFT MCUFriend 2.4".
// La pantalla se lee como 0x9328 en Arduino UNO, pero funciona con init ILI9325.
// Esta version usa bus paralelo de 8 bits directamente, sin Arduino_GFX.

#define BLACK 0x0000
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define RED   0xF800
#define BLUE  0x001F

const int TFT_CS = 26;   // LCD_CS
const int TFT_RS = 27;   // LCD_RS / CD
const int TFT_WR = 32;   // LCD_WR
const int TFT_RD = 33;   // LCD_RD, tambien puede ir directo a 3V3
const int TFT_RST = 4;   // LCD_RST

// Evitamos GPIO16/GPIO17 porque algunos ESP32 los usan para memoria interna.
const int TFT_D0 = 13;
const int TFT_D1 = 14;
const int TFT_D2 = 18;
const int TFT_D3 = 19;
const int TFT_D4 = 21;
const int TFT_D5 = 22;
const int TFT_D6 = 23;
const int TFT_D7 = 25;

const int DATA_PINS[8] = {
  TFT_D0, TFT_D1, TFT_D2, TFT_D3, TFT_D4, TFT_D5, TFT_D6, TFT_D7
};

const uint16_t TFTLCD_DELAY = 0xFFFF;

const uint16_t ILI932X_REGS[][2] = {
  {0x00, 0x0001},
  {TFTLCD_DELAY, 50},
  {0x01, 0x0100},
  {0x02, 0x0700},
  {0x03, 0x1030},
  {0x04, 0x0000},
  {0x08, 0x0202},
  {0x09, 0x0000},
  {0x0A, 0x0000},
  {0x0C, 0x0000},
  {0x0D, 0x0000},
  {0x0F, 0x0000},
  {0x10, 0x0000},
  {0x11, 0x0007},
  {0x12, 0x0000},
  {0x13, 0x0000},
  {TFTLCD_DELAY, 200},
  {0x10, 0x1690},
  {0x11, 0x0227},
  {TFTLCD_DELAY, 50},
  {0x12, 0x001A},
  {TFTLCD_DELAY, 50},
  {0x13, 0x1800},
  {0x29, 0x002A},
  {TFTLCD_DELAY, 50},
  {0x30, 0x0000},
  {0x31, 0x0000},
  {0x32, 0x0000},
  {0x35, 0x0206},
  {0x36, 0x0808},
  {0x37, 0x0007},
  {0x38, 0x0201},
  {0x39, 0x0000},
  {0x3C, 0x0000},
  {0x3D, 0x0000},
  {0x20, 0x0000},
  {0x21, 0x0000},
  {0x50, 0x0000},
  {0x51, 0x00EF},
  {0x52, 0x0000},
  {0x53, 0x013F},
  {0x60, 0xA700},
  {0x61, 0x0003},
  {0x6A, 0x0000},
  {0x90, 0x0010},
  {0x92, 0x0000},
  {0x93, 0x0003},
  {0x95, 0x1100},
  {0x97, 0x0000},
  {0x98, 0x0000},
  {0x07, 0x0133}
};

class ILI9325_ESP32_8BIT : public Adafruit_GFX {
public:
  ILI9325_ESP32_8BIT() : Adafruit_GFX(240, 320) {}

  void begin() {
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_RS, OUTPUT);
    pinMode(TFT_WR, OUTPUT);
    pinMode(TFT_RD, OUTPUT);
    pinMode(TFT_RST, OUTPUT);

    for (int i = 0; i < 8; i++) {
      pinMode(DATA_PINS[i], OUTPUT);
    }

    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TFT_RS, HIGH);
    digitalWrite(TFT_WR, HIGH);
    digitalWrite(TFT_RD, HIGH);

    digitalWrite(TFT_RST, HIGH);
    delay(20);
    digitalWrite(TFT_RST, LOW);
    delay(40);
    digitalWrite(TFT_RST, HIGH);
    delay(200);

    for (unsigned int i = 0; i < (sizeof(ILI932X_REGS) / sizeof(ILI932X_REGS[0])); i++) {
      uint16_t reg = ILI932X_REGS[i][0];
      uint16_t value = ILI932X_REGS[i][1];

      if (reg == TFTLCD_DELAY) {
        delay(value);
      } else {
        writeRegister(reg, value);
      }
    }

    setAddressWindow(0, 0, width() - 1, height() - 1);
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color) override {
    if (x < 0 || y < 0 || x >= width() || y >= height()) {
      return;
    }

    writeRegister(0x20, x);
    writeRegister(0x21, y);
    writeRegister(0x22, color);
  }

  void fillScreen(uint16_t color) {
    fillRect(0, 0, width(), height(), color);
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
    if (x < 0) {
      w += x;
      x = 0;
    }
    if (y < 0) {
      h += y;
      y = 0;
    }
    if ((x + w) > width()) {
      w = width() - x;
    }
    if ((y + h) > height()) {
      h = height() - y;
    }
    if (w <= 0 || h <= 0) {
      return;
    }

    setAddressWindow(x, y, x + w - 1, y + h - 1);
    writeCommand(0x22);
    digitalWrite(TFT_RS, HIGH);
    digitalWrite(TFT_CS, LOW);
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
      write16(color);
    }
    digitalWrite(TFT_CS, HIGH);
  }

private:
  void write8(uint8_t value) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(DATA_PINS[i], (value >> i) & 0x01);
    }
    delayMicroseconds(1);
    digitalWrite(TFT_WR, LOW);
    delayMicroseconds(2);
    digitalWrite(TFT_WR, HIGH);
    delayMicroseconds(1);
  }

  void write16(uint16_t value) {
    write8(value >> 8);
    write8(value & 0xFF);
  }

  void writeCommand(uint16_t command) {
    digitalWrite(TFT_CS, LOW);
    digitalWrite(TFT_RS, LOW);
    write16(command);
    digitalWrite(TFT_CS, HIGH);
  }

  void writeRegister(uint16_t reg, uint16_t value) {
    digitalWrite(TFT_CS, LOW);
    digitalWrite(TFT_RS, LOW);
    write16(reg);
    digitalWrite(TFT_RS, HIGH);
    write16(value);
    digitalWrite(TFT_CS, HIGH);
  }

  void setAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    writeRegister(0x50, x1);
    writeRegister(0x51, x2);
    writeRegister(0x52, y1);
    writeRegister(0x53, y2);
    writeRegister(0x20, x1);
    writeRegister(0x21, y1);
  }
};

ILI9325_ESP32_8BIT tft;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("ESP32 + TFT ILI9325 paralelo 8-bit");
  Serial.println("LCD_RD puede ir a GPIO33 o fijo a 3V3");

  tft.begin();
  tft.fillScreen(RED);
  delay(400);
  tft.fillScreen(GREEN);
  delay(400);
  tft.fillScreen(BLUE);
  delay(400);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, tft.width(), tft.height(), RED);

  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.setCursor(16, 18);
  tft.print("ESP32 ILI9325");

  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(5);
  tft.setCursor(26, 132);
  tft.print("00:00");
}

void loop() {
}
