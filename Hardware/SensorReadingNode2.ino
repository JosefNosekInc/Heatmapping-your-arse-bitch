#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define NODE_ID 2
#define DHTPIN 15
#define DHTTYPE DHT22

const char* ssid = "wifiname"; 
const char* password = "wifipassword";
const char* serverURL = "http://192.169.169.69/api/data/";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(115200);
dht.begin();

WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi");

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println();
Serial.println("WiFi connected");
}

void loop() {
float temp = dht.readTemperature();
float hum = dht.readHumidity();

if (!isnan(temp) && !isnan(hum)) {
Serial.printf("Node %d -> Temp: %.1f°C, Hum: %.1f%%\n", NODE_ID, temp, hum);


if (WiFi.status() == WL_CONNECTED) {
  HTTPClient http;

  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  String jsonPayload = "{\"node_id\": " + String(NODE_ID) +
                       ", \"temperature\": " + String(temp, 1) +
                       ", \"humidity\": " + String(hum, 1) + "}";

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.printf("POST OK, response code: %d\n", httpResponseCode);
  } else {
    Serial.printf("POST failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
} else {
  Serial.println("WiFi disconnected");
}
} else {
Serial.println("Sensor read failed");
}

delay(500); // send every 5 seconds
}
