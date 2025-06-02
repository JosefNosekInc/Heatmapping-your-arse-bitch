#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

typedef struct struct_message {
  uint8_t node_id;
  float temperature;
  float humidity;
} struct_message;

struct_message incomingData;

// Optional LED feedback
const int ledPin = 2; // Built-in LED (GPIO2 on many ESP32 boards)

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  if (len == sizeof(struct_message)) {
    memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             info->src_addr[0], info->src_addr[1], info->src_addr[2],
             info->src_addr[3], info->src_addr[4], info->src_addr[5]);

    Serial.printf("[%lu ms] [Node %d] From %s -> Temp: %.1f°C, Hum: %.1f%%\n",
                  millis(), incomingData.node_id, macStr,
                  incomingData.temperature, incomingData.humidity);

    // Blink LED for visual feedback
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
  } else {
    Serial.printf("Received unexpected data length: %d bytes\n", len);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // Optional: Force channel to match sender

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);
  Serial.println("ESP-NOW Receiver Ready");
}

void loop() {
  // No active work in loop
}
