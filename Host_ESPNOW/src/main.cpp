#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <display.h>
#include <wireless_host.h>

bool ledState = 0;
const int ledPin = 2;
TaskHandle_t CoreZEROTasks;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite logtxt1 = TFT_eSprite(&tft); // Sprite object stext1

extern AsyncWebServer server;
extern AsyncWebSocket ws;


void setup(){
  display_init();
  Serial.begin(115200);
  SPIFFS.begin(true);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  wifi_initial();
  initWebSocket();
  setup_webpages();
}

void loop() {
  delay(1000);
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}