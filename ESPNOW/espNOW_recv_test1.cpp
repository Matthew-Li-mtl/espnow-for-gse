// ESPNOW ping-pong test 1 - PONG side
// Waits for a ping and sends the same number straight back.

#include <WiFi.h>
#include <esp_now.h>

// MAC of the PING board (espNOW_send_test1)
uint8_t peer[] = {0x88, 0x57, 0x21, 0xB1, 0xBB, 0xCC};

void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  uint32_t ping;
  if (len != sizeof(ping)) return;
  memcpy(&ping, data, sizeof(ping));
  Serial.printf("Got ping %u, sending pong\n", ping);
  esp_now_send(peer, (uint8_t *)&ping, sizeof(ping));
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

void loop() {}
