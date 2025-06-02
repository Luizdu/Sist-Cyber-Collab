// ========== FINAL RECEIVER ESP32 (Node C) ========== 
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define LED_PIN 2

// Callback com assinatura esp_now_recv_info_t*
void OnDataRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  // Pisca LED
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);

  // Obtém MAC do remetente
  const uint8_t* mac_addr = info->src_addr;

  // Converte payload em string JSON
  char json[len + 1];
  memcpy(json, data, len);
  json[len] = '\0';

  // Imprime MAC e conteúdo JSON
  Serial.print("Receptor Final recebeu de ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" -> ");
  Serial.println(json);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  Serial.print("Meu MAC (Node C): ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  // Registra callback com assinatura esp_now_recv_info_t*
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receptor Final pronto");
}

void loop() {
  delay(100);
}