#include "wireless_host.h"

const char* ssid = "HUAWEI-2.4G-g4ci"; const char* password = "db7nf4dk";

extern bool ledState;
extern const int ledPin;
extern TFT_eSPI tft;
extern TFT_eSprite logtxt1;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void wifi_initial(void)
{
      // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  int con_counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    con_counter++;
    Serial.println("Connecting to WiFi.."); display_log_print("Connecting to WiFi..");
    if (con_counter > 4) ESP.restart();
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP()); display_log_print("IP: " + WiFi.localIP().toString());
}

void setup_webpages(void)
{
    // Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", index_html, processor);
  // });

      // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/_index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

    // Route to load style.css file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

      // Route for root / web page
  server.on("/sec.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/sec.html", String(), false, processor);
  });
  // Start server
  server.begin();
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

void notifyClients(int dd) {
  if (dd)
  ws.textAll(String(ledState));
  else
  ws.textAll("2");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      tft.fillRect(0, 0, 150, 50, ledState ? TFT_GREEN : TFT_BLACK);
      notifyClients(0);
    }
    if (strcmp((char*)data, "newp") == 0) {
      ledState = !ledState;
      tft.fillRect(0, 0, 150, 50, ledState ? TFT_RED : TFT_BLACK);
      notifyClients(1);
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

