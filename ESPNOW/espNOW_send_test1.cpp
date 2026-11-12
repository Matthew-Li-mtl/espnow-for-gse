// ESPNOW ping-pong test 1 - PING side
// Sends a numbered ping every second, prints the pong that comes back.

#include <WiFi.h>
#include <esp_now.h>

// MAC of the PONG board (espNOW_recv_test1)
uint8_t peer[] = {0x88, 0x57, 0x21, 0xB2, 0x40, 0x8C};

uint32_t pingCount = 0;

void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  uint32_t pong;
  if (len != sizeof(pong)) return;
  memcpy(&pong, data, sizeof(pong));
  Serial.printf("Got pong %u\n", pong);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer, 6);
  esp_now_add_peer(&peerInfo);
}

void loop() {
  pingCount++;
  esp_now_send(peer, (uint8_t *)&pingCount, sizeof(pingCount));
  Serial.printf("Sent ping %u\n", pingCount);
  delay(1000);
}
