#include <esp_now.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "iPhone";       
const char* password = "bestofmeaa";   

// Server endpoint
const char* serverUrl = "http://172.20.2.2:8000/api/receive_sensor_data/";

// Structure matching ESP-NOW message format
typedef struct struct_message {
  uint8_t node_id;
  float temperature;
  float humidity;
} struct_message;

struct_message incomingData;

void sendToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("📡 Wi-Fi connected, sending data to server...");

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{";
    jsonPayload += "\"node_id\":" + String(incomingData.node_id) + ",";
    jsonPayload += "\"temperature\":" + String(incomingData.temperature, 1) + ",";
    jsonPayload += "\"humidity\":" + String(incomingData.humidity, 1);
    jsonPayload += "}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.printf("✅ Data sent! HTTP response code: %d\n", httpResponseCode);
      Serial.println("🗒️ Server response: " + response);
    } else {
      Serial.printf("❌ Failed to send POST. Error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("🚫 Wi-Fi not connected. Cannot send data.");
  }
}

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  if (len == sizeof(incomingData)) {
    memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             info->src_addr[0], info->src_addr[1], info->src_addr[2],
             info->src_addr[3], info->src_addr[4], info->src_addr[5]);

    Serial.println("📥 ESP-NOW Packet Received:");
    Serial.printf("🔹 From MAC: %s\n", macStr);
    Serial.printf("🔸 Node ID: %d\n", incomingData.node_id);
    Serial.printf("🌡️ Temp: %.1f°C | 💧 Humidity: %.1f%%\n", incomingData.temperature, incomingData.humidity);

    sendToServer();
  } else {
    Serial.printf("⚠️ Unexpected data length: %d bytes\n", len);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n🔧 Booting ESP-NOW Master...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to Wi-Fi");

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi Connected!");
    Serial.print("🌐 IP Address: ");
    Serial.println(WiFi.localIP());

    wifi_second_chan_t second;
    uint8_t current_channel = WiFi.channel(&second);
    Serial.printf("📡 Master is using Wi-Fi channel: %d\n", current_channel);

  } else {
    Serial.println("\n🚫 Failed to connect to Wi-Fi within timeout.");
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW initialization failed.");
    return;
  }

  Serial.println("📶 ESP-NOW Initialized. Waiting for data...");
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  
}
