#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include "DHT.h"
#include <HTTPClient.h>

#define NODE_ID 2                // Unique ID for this node
#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Replace with your Wi-Fi credentials
const char* ssid = "iPhone";
const char* password = "bestofmeaa";

// Django server endpoint
const char* serverName = "http://172.20.10.2:8000/api/receive_sensor_data/";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("🔧 Starting ESP32...");

  dht.begin();

  Serial.print("📶 Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected successfully!");
    Serial.print("🌐 Local IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi connection failed!");
  }
}

void loop() {
  Serial.println("📊 Reading temperature and humidity...");

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ Failed to read from DHT sensor.");
    delay(5000);
    return;
  }

  Serial.printf("🌡 Temperature: %.2f °C\n", temperature);
  Serial.printf("💧 Humidity: %.2f %%\n", humidity);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Add a timestamp using millis() for now
    unsigned long timestamp = millis();

    // Build JSON payload with timestamp
    String jsonData = "{";
    jsonData += "\"node_id\":" + String(NODE_ID) + ",";
    jsonData += "\"temperature\":" + String(temperature, 2) + ",";
    jsonData += "\"humidity\":" + String(humidity, 2) + ",";
    jsonData += "\"timestamp\":" + String(timestamp);
    jsonData += "}";

    Serial.print("📡 Sending data to server: ");
    Serial.println(jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.print("✅ Server response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.print("📨 Server reply: ");
      Serial.println(response);
    } else {
      Serial.print("❌ Error sending POST request. Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("⚠️ WiFi not connected. Skipping data send.");
  }

  Serial.println("⏳ Waiting 10 seconds before next reading...\n");
  delay(10000);
}