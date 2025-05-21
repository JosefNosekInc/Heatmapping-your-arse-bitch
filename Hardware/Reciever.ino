#include <esp_now.h>
#include <WiFi.h>

// Updated structure: includes node_id
typedef struct struct_message {
  uint8_t node_id;
  float temperature;
  float humidity;
} struct_message;

struct_message incomingData;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  // Print MAC address of sender
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  // Print all received values
  Serial.printf("[Node %d] From %s -> Temp: %.1f°C, Hum: %.1f%%\n",
                incomingData.node_id, macStr, incomingData.temperature, incomingData.humidity);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // ESP-NOW requires station mode

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Nothing to do here
}
