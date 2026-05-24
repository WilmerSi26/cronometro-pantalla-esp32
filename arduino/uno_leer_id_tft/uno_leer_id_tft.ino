#include <MCUFRIEND_kbv.h>

// Prueba para Arduino UNO + shield TFT MCUFriend.
// Abre el Monitor Serial a 9600 baudios y revisa el ID detectado.

MCUFRIEND_kbv tft;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Leyendo ID de pantalla TFT MCUFriend...");
  Serial.println("Shield esperado: control A0-A4, datos D2-D9");

  uint16_t id = tft.readID();
  Serial.print("ID detectado: 0x");
  Serial.println(id, HEX);

  if (id == 0x9341) {
    Serial.println("Controlador detectado: ILI9341");
  } else if (id == 0x9486) {
    Serial.println("Controlador detectado: ILI9486");
  } else if (id == 0x9488) {
    Serial.println("Controlador detectado: ILI9488");
  } else if (id == 0xFFFF || id == 0x0000 || id == 0xD3D3 || id == 0x00D3) {
    Serial.println("ID invalido: revisar contacto del shield, RD, RESET o alimentacion.");
  } else {
    Serial.println("ID no esperado. Anotalo para forzarlo en tft.begin(ID).");
  }
}

void loop() {
}
