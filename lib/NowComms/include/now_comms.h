#pragma once

#include <esp_now.h>

void setupEspNow();
void registerReceiveCb(void (*onDataRecv)(const uint8_t*, const uint8_t*, int));
void registerTransmitCb(void (*onDataSent)(const uint8_t*, esp_now_send_status_t));
bool registerPeer(const uint8_t *peerMACAddress);
void sendData(const uint8_t *peerMACAddress, const uint8_t *data, int len);