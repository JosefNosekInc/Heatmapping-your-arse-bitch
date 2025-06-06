#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include "DHT.h"

#define NODE_ID 2
#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

typedef struct struct_message {
  uint8_t node_id;
  float temperature;
  float humidity;
} struct_message;

struct_message data;

// Master MAC address (replace with your master MAC)
uint8_t receiverMAC[] = { 0xEC, 0xE3, 0x34, 0xDB, 0xC0, 0xCC };

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Set Wi-Fi mode and sync to channel 6
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);  // Master’s Wi-Fi channel
  esp_wifi_set_promiscuous(false);

  Serial.println("📡 ESP-NOW Slave Starting on Channel 6");

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 6;  // Must match master's Wi-Fi channel
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(receiverMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("❌ Failed to add peer");
      return;
    }
  }

  esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("📤 Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Success" : "❌ Fail");
  });

  Serial.println("✅ ESP-NOW ready to send data");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    data.node_id = NODE_ID;
    data.temperature = temp;
    data.humidity = hum;

    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));

    Serial.printf("📦 Sending: Node %d -> Temp: %.1f°C, Hum: %.1f%%\n", NODE_ID, temp, hum);

    if (result != ESP_OK) {
      Serial.println("❌ Error sending ESP-NOW data");
    }
  } else {
    Serial.println("⚠️ Sensor read failed");
  }

  delay(2000);
}
