#include "heltec.h"
#include "WiFi.h"
#include "time.h"

#define BAND 915E6

const char * WIFI_SSID = "LIVE TIM_7524_2G";
const char * WIFI_PASS = "kp4c4mct4k";

bool firstLoop = true;

void setupWiFi() {
  WiFi.disconnect(true);
  delay(100);

  Heltec.display->drawString(0, 0, "Setting WiFi...");
  Heltec.display->display();

  WiFi.mode(WIFI_MODE_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  byte count = 0;

  while (WiFi.status() != WL_CONNECTED && count < 10) {
    count++;
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Heltec.display->drawString(90, 0, "OK");
  } else {
    Heltec.display->drawString(90, 0, "Failed");
  }

  Heltec.display->display();
  delay(100);
}

void setupRTC() {
  Heltec.display->drawString(0, 10, "Setting RTC...");
  Heltec.display->display();

  struct tm timeinfo;
  configTime(0, 0, "pool.ntp.org");

  if (!getLocalTime(&timeinfo)) {
    Heltec.display->drawString(90, 10, "Failed");
    Heltec.display->display();
    return;
  }  

  setenv("TZ", "<-03>3", 1);
  tzset();

  Heltec.display->drawString(90, 10, "OK");
  Heltec.display->display();
}

void setupLoRa() {
  Heltec.display->drawString(0, 20, "Setting LoRa...");
  Heltec.display->display();

  LoRa.setSpreadingFactor(12);        // Fator de espalhamento
  LoRa.setSignalBandwidth(250E3);     // Largura de banda
  LoRa.setCodingRate4(5);             // Codding rate
  LoRa.setPreambleLength(6);          // Comprimento do preâmbulo
  LoRa.setSyncWord(0x12);             // Palavra de sincronização
  LoRa.crc();

  Heltec.display->drawString(90, 20, "OK");
  Heltec.display->display();
}

void setup() {
  Heltec.begin(true, true, true, true, BAND);

  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

  setupWiFi();
  setupRTC();
  setupLoRa();
}

void sendData() {
  Serial.println("Enviando leitura...");
  time_t tt = time(NULL);

  LoRa.beginPacket();
  LoRa.printf("Read=145.67;Timestamp=%d", int32_t(tt));
  LoRa.endPacket();
}

void loop() {
  if (firstLoop) {
    Heltec.display->drawString(0, 40, "All settings done! ");
    Heltec.display->display();

    firstLoop = false;
  }

  sendData();
  delay(2000);
}
