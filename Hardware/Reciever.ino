#include <esp_now.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "LACMAGO_PEPA 3302";
const char* password = "92i4(W58";

// Django server URL
const char* serverUrl = "http://http://127.0.0.1:8000/api/receive_sensor_data/";

typedef struct struct_message {
  uint8_t node_id;
  float temperature;
  float humidity;
} struct_message;

struct_message incomingData;

void sendToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Prepare JSON payload
    String jsonPayload = "{";
    jsonPayload += "\"node_id\":" + String(incomingData.node_id) + ",";
    jsonPayload += "\"temperature\":" + String(incomingData.temperature, 1) + ",";
    jsonPayload += "\"humidity\":" + String(incomingData.humidity, 1);
    jsonPayload += "}";

    int httpResponseCode = http.POST(jsonPayload);
    Serial.printf("HTTP POST response code: %d\n", httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  Serial.printf("[Node %d] From %s -> Temp: %.1f°C, Hum: %.1f%%\n",
                incomingData.node_id, macStr, incomingData.temperature, incomingData.humidity);

  sendToServer();
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Nothing needed here
}

