// Lector de ID para ESP32 + pantalla TFT MCUFriend 8-bit.
// Sirve para confirmar si el ESP32 realmente se comunica con la pantalla.
//
// IMPORTANTE:
// En esta prueba LCD_RD debe ir a GPIO33, NO directo a 3V3.

const int TFT_CS = 26;   // LCD_CS
const int TFT_RS = 27;   // LCD_RS / CD
const int TFT_WR = 32;   // LCD_WR
const int TFT_RD = 33;   // LCD_RD
const int TFT_RST = 4;   // LCD_RST

const int TFT_D0 = 13;
const int TFT_D1 = 14;
const int TFT_D2 = 18;
const int TFT_D3 = 19;
const int TFT_D4 = 21;
const int TFT_D5 = 22;
const int TFT_D6 = 23;
const int TFT_D7 = 25;

const int DATA_PINS[8] = {
  TFT_D0, TFT_D1, TFT_D2, TFT_D3,
  TFT_D4, TFT_D5, TFT_D6, TFT_D7
};

void setDataWrite() {
  for (int i = 0; i < 8; i++) {
    pinMode(DATA_PINS[i], OUTPUT);
  }
}

void setDataRead() {
  for (int i = 0; i < 8; i++) {
    pinMode(DATA_PINS[i], INPUT);
  }
}

void write8(uint8_t value) {
  setDataWrite();
  for (int i = 0; i < 8; i++) {
    digitalWrite(DATA_PINS[i], (value >> i) & 0x01);
  }

  delayMicroseconds(1);
  digitalWrite(TFT_WR, LOW);
  delayMicroseconds(2);
  digitalWrite(TFT_WR, HIGH);
  delayMicroseconds(1);
}

uint8_t read8() {
  uint8_t value = 0;

  setDataRead();
  delayMicroseconds(2);
  digitalWrite(TFT_RD, LOW);
  delayMicroseconds(5);

  for (int i = 0; i < 8; i++) {
    if (digitalRead(DATA_PINS[i])) {
      value |= (1 << i);
    }
  }

  digitalWrite(TFT_RD, HIGH);
  delayMicroseconds(2);
  return value;
}

void writeCommand16(uint16_t command) {
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_RS, LOW);
  digitalWrite(TFT_RD, HIGH);
  write8(command >> 8);
  write8(command & 0xFF);
  digitalWrite(TFT_CS, HIGH);
}

uint8_t readRegister8(uint16_t reg, uint8_t index) {
  writeCommand16(reg);

  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_RS, HIGH);
  digitalWrite(TFT_WR, HIGH);

  uint8_t value = 0;
  for (uint8_t i = 0; i <= index; i++) {
    value = read8();
  }

  digitalWrite(TFT_CS, HIGH);
  setDataWrite();
  return value;
}

void resetTft() {
  digitalWrite(TFT_RST, HIGH);
  delay(20);
  digitalWrite(TFT_RST, LOW);
  delay(50);
  digitalWrite(TFT_RST, HIGH);
  delay(200);
}

void printRegister(uint16_t reg, uint8_t count, const char *label) {
  Serial.print("reg(0x");
  if (reg < 0x10) {
    Serial.print("0");
  }
  Serial.print(reg, HEX);
  Serial.print("):");

  for (uint8_t i = 0; i < count; i++) {
    uint8_t value = readRegister8(reg, i);
    Serial.print(" ");
    if (value < 0x10) {
      Serial.print("0");
    }
    Serial.print(value, HEX);
  }

  Serial.print("  ");
  Serial.println(label);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_RS, OUTPUT);
  pinMode(TFT_WR, OUTPUT);
  pinMode(TFT_RD, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  setDataWrite();

  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TFT_RS, HIGH);
  digitalWrite(TFT_WR, HIGH);
  digitalWrite(TFT_RD, HIGH);

  resetTft();

  Serial.println("ESP32 lee registros de TFT MCUFriend");
  Serial.println("Si todo va bien, debe aparecer algo como 0x9328.");
  Serial.println();

  printRegister(0x00, 2, "ID estilo ILI932x");
  printRegister(0x04, 4, "Manufacturer ID");
  printRegister(0x09, 5, "Status");
  printRegister(0xD3, 4, "ILI9341/ILI9488 ID");
}

void loop() {
}
