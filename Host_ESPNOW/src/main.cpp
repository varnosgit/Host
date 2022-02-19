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

char rxdata[5];

extern AsyncWebServer server;
extern AsyncWebSocket ws;

void coreZEROTasks_code( void * parameter) {
  for(;;) {

    delay(500);
    // Serial2.write("12345");
    // if (Serial2.available()) 
    // {
    //   Serial2.read(rxdata, 5);
    //   Serial.println(rxdata);
    // }
    
    // delay(50);
    // byte n = Serial2.available();  //3:
    //  if(n != 0) //4:
    //  {           
    //      byte m = Serial.readBytesUntil('\n', rxdata, 5);  //5:
    //      rxdata[m] = '\0';  //6:
    //      Serial.print(String(rxdata)); Serial.println("rxdata");//7:
    //  }
  }
}

void setup(){
  //////////////////////////////////////////// inits
  Serial.begin(115200);   Serial2.begin(115200);
  SPIFFS.begin(true);
  // display_init();  display_log_init();   display_log_print("Initializing...");
  ////////////////////////////////////////// defualt values

  Serial.print("Initializing...");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 ////////////////////////////////////////// wifi & webpage setups 
  wifi_initial();
  initWebSocket();
  setup_webpages();

  xTaskCreatePinnedToCore(
                  coreZEROTasks_code,      /* Task function. */
                  "Task1",        /* name of task. */
                  10000,          /* Stack size of task */
                  NULL,           /* parameter of the task */
                  1,              /* priority of the task */
                  &CoreZEROTasks, /* Task handle to keep track of created task */
                  0);   
}


void loop() {
  delay(1000);
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}