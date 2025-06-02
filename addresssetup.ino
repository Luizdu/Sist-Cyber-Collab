
// ========== SKETCH PARA IDENTIFICAR MAC ADDRESS ==========
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.print("Endereço MAC deste ESP32: ");
  Serial.println(WiFi.macAddress());
}

void loop() { /* nada a fazer */ }
