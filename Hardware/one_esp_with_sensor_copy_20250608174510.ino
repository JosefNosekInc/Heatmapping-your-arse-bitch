#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// ==== Configuration ====
const char* ssid = "ASP_Pixel";
const char* password = "ASP_wifi";

// Django API endpoint
const char* serverUrl = "http://10.242.158.56:8000/api/receive_sensor_data/";

// DHT Sensor Settings
#define DHTPIN 15          // GPIO pin connected to DHT
#define DHTTYPE DHT22     // or DHT11
DHT dht(DHTPIN, DHTTYPE);

// Unique sensor ID
const char* sensor_id = "ESP32_TEMP_5";

// Custom X and Y coordinates
const int x = 250;
const int y = 400;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting DHT sensor...");
  dht.begin();

  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      Serial.printf("Read Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);

      // Include x and y in JSON
      String json = "{\"sensor_id\":\"" + String(sensor_id) + "\","
                    "\"temperature\":" + String(temperature) + ","
                    "\"humidity\":" + String(humidity) + ","
                    "\"x\":" + String(x) + ","
                    "\"y\":" + String(y) + "}";

      Serial.println("Sending JSON:");
      Serial.println(json);

      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(json);

      if (httpResponseCode > 0) {
        Serial.printf("✅ Data sent! HTTP status: %d\n", httpResponseCode);
        String response = http.getString();
        Serial.println("Response: " + response);
      } else {
        Serial.printf("❌ POST failed! Error: %s\n", http.errorToString(httpResponseCode).c_str());
      }

      http.end();
    } else {
      Serial.println("❗ Failed to read from DHT sensor");
    }
  } else {
    Serial.println("❌ Not connected to Wi-Fi");
  }

  delay(10000); // Wait 10 seconds before sending again
}
