// ========== SENDER ESP32 (Node A) COM SENSOR DHT22 ========== //
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define SEND_INTERVAL 5000
#define NODE_ID "60-1"  // Código da turma + número do nó
#define DHTPIN 5          // Pino do sensor DHT
#define DHTTYPE DHT11     // Tipo do sensor

DHT dht(DHTPIN, DHTTYPE);
int msgCount = 0;
bool controle = false;
uint8_t relayMAC[] = {0xA0, 0xB7, 0x065, 0x28, 0x08, 0x88}; // MAC real do Relay ESP32

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  Serial.print("Meu MAC (Node A): ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, relayMAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  Serial.println("Sender (Node A) pronto");
}

void loop() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Falha na leitura DHT");
    delay(SEND_INTERVAL);
    return;
  }

  msgCount++;
  if (msgCount % 5 == 0) controle = !controle;

  // Monta JSON conforme requisitos
  StaticJsonDocument<128> doc;
  doc["id"] = NODE_ID;
  doc["msgCount"] = msgCount;
  doc["temperatura"] = temp;
  doc["umidade"] = hum;
  doc["controle"] = controle;

  char buf[128];
  size_t len = serializeJson(doc, buf);
  esp_now_send(relayMAC, (uint8_t*)buf, len);

  Serial.printf("Enviado -> id=%s cnt=%d temp=%.1fC umidade=%.1f%% ctrl=%s\n",
                NODE_ID, msgCount, temp, hum, controle?"ON":"OFF");
  delay(SEND_INTERVAL);
}
