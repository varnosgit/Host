#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Effortless_SPIFFS.h>

#include "wireless_host.h"
#include "host_control.h"
#include "zones.h"



extern hc_message hc_mesg;
extern uint8_t hc_sendFlag;
extern uint8_t hc_recvFlag;

extern AsyncWebServer server;
extern AsyncWebSocket ws;

uint8_t modeStat = 0, fanStat = 0;

bool ledState = 0;
const int ledPin = 2;
TaskHandle_t CoreZEROTasks;

void coreZEROTasks_code( void * parameter) {
  for(;;) {
    if (hc_sendFlag)
      send_data_to_controller();
    if (receive_data_from_controller())
      hc_recvFlag = 1;
    if (hc_recvFlag)
      handle_controller_message();
    delay(2000);
  }
}

void setup(){
  //////////////////////////////////////////// inits
  Serial.begin(115200);  
  Serial2.begin(115200);
  SPIFFS.begin(true); 
  ////////////////////////////////////////// defualt values
  Serial.print("Initializing...");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 ////////////////////////////////////////// wifi & webpage setups 
  zone_reload();
  wifi_initial();
  initWebSocket();
  setup_webpages();
 ////////////////////////////////////////// second core initial
  xTaskCreatePinnedToCore(
                  coreZEROTasks_code,      /* Task function. */
                  "Task1",        /* name of task. */
                  10000,          /* Stack size of task */
                  NULL,           /* parameter of the task */
                  1,              /* priority of the task */
                  &CoreZEROTasks, /* Task handle to keep track of created task */
                  0);   
}

int clean_counter = 0; 
void loop() {
  delay(100);
  if (clean_counter++ == 20) { ws.cleanupClients(); clean_counter = 0;}
  digitalWrite(ledPin, ledState);
}

//cool 1
//cool 2
//heat 1
//heat 2
//fan --> on/auto