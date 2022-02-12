// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <display.h>

#include "webpage1.h"

// Replace with your network credentials
const char* ssid = "Varnos5";
const char* password = "toolesag";

bool ledState = 0;
const int ledPin = 2;

TaskHandle_t CoreZEROTasks;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite logtxt1 = TFT_eSprite(&tft); // Sprite object stext1

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


////////////////////////////////////////////////////////////////////////////////////
void coreZEROTasks_code( void * pvParameters ){
  for(;;){
    delay(4);
  } 
}

void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  display_init();
  display_log_init();   display_log_print("Initialising...");
  delay(100); Serial.begin(115200);   display_log_print("Serial Debug connect!");
  logtxt1.drawNumber(getCpuFrequencyMhz(), 71, 240, 2);
  delay(300); display_log_print("CPU Freq.:     MHz");
  xTaskCreatePinnedToCore(
                    coreZEROTasks_code,      /* Task function. */
                    "Task1",        /* name of task. */
                    10000,          /* Stack size of task */
                    NULL,           /* parameter of the task */
                    1,              /* priority of the task */
                    &CoreZEROTasks, /* Task handle to keep track of created task */
                    0);             /* pin task to core 0 */                  
  delay(500); 
  delay(200); display_log_print("2nd Core setup!");
  //delay(100); timer_init(); display_log_print("Timers connected!");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi.."); display_log_print("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP()); display_log_print(WiFi.localIP().toString());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}