// ========== RELAY ESP32 (Node B) ========== //
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define RELAY_ID "60-2"
uint8_t receiverMAC[] = {0x10, 0x06, 0x1C, 0x86, 0x21, 0xB0}; // MAC real do Receptor Final

// Callback com a assinatura antiga (esp_now_recv_info_t*)
void OnDataRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  // obtém o MAC de quem enviou
  const uint8_t* mac_addr = info->src_addr;

  // converte payload em string JSON
  char json[len + 1];
  memcpy(json, data, len);
  json[len] = '\0';

  // imprime MAC de origem e JSON
  Serial.print("Relay recebeu de ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" -> ");
  Serial.println(json);

  // retransmite ao receptor final
  esp_now_send(receiverMAC, data, len);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.print("Meu MAC (Node B): ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ESP-NOW");
    return;
  }

  // registra callback com assinatura esp_now_recv_info_t*
  esp_now_register_recv_cb(OnDataRecv);

  // configura peer para o receptor final
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  Serial.println("Relay (Node B) pronto");
}

void loop() {
  delay(100);
}
