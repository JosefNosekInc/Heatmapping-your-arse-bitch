#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"     // Required for esp_wifi_set_channel()
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

// ⚠ Replace with the *actual MAC address of your receiver*
uint8_t receiverMAC[] = { 0x14, 0x33, 0x5C, 0x03, 0xD3, 0x6C };


void setup() {
  Serial.begin(115200);
  dht.begin();

  // Set WiFi to station mode & channel
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);  // Ensure both ESPs are on channel 1

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }
  Serial.println("✅ ESP-NOW Initialized");

  // Register peer (the receiver)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0; // Use current channel
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(receiverMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("❌ Failed to add peer");
      return;
    } else {
      Serial.println("✅ Peer added successfully");
    }
  } else {
    Serial.println("ℹ Peer already exists");
  }

  // Register send callback
  esp_now_register_send_cb([](const uint8_t *mac, esp_now_send_status_t status) {
    Serial.print("📤 Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Success" : "❌ Fail");
  });

  Serial.println("📡 Sender Ready");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    data.node_id = NODE_ID;
    data.temperature = temp;
    data.humidity = hum;

    Serial.printf("[%lu ms] Node %d -> Temp: %.1f°C, Hum: %.1f%%\n",
                  millis(), NODE_ID, temp, hum);

    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));

    if (result != ESP_OK) {
      Serial.println("❌ esp_now_send() returned error");
    }
  } else {
    Serial.println("⚠ Sensor read failed (NaN)");
  }

  delay(5000); // Send data every 5 seconds
}
