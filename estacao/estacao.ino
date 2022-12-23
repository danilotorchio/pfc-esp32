#include "heltec.h"

#define BAND 915E6

bool firstLoop = true;

void setupLoRa() {
  Heltec.display->drawString(0, 0, "Setting LoRa...");
  Heltec.display->display();

  LoRa.setSpreadingFactor(12);        // Fator de espalhamento
  LoRa.setSignalBandwidth(250E3);     // Largura de banda
  LoRa.setCodingRate4(5);             // Codding rate
  LoRa.setPreambleLength(6);          // Comprimento do preâmbulo
  LoRa.setSyncWord(0x12);             // Palavra de sincronização
  LoRa.crc();

  Heltec.display->drawString(90, 0, "OK");
  Heltec.display->display();
}

void setup() {
  Heltec.begin(true, true, true, true, BAND);

  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

  setupLoRa();
}

void readData() {
  int packetSize = LoRa.parsePacket();

  if (packetSize > 0) {
    Serial.println("Leitura recebida:");
    
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.println();
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("------");
  }
}

void loop() {
  if (firstLoop) {
    Heltec.display->drawString(0, 20, "All settings done!");
    Heltec.display->display();
    firstLoop = false;
  }

  readData();
}
