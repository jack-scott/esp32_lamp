#include <now_comms.h>
#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>
#include <Debug.hpp>


void setupEspNow()
{
    WiFi.mode(WIFI_STA);
    String myMacAddress = WiFi.macAddress();
    String myIpAddress = WiFi.localIP().toString();
    DEBUG("---- My details -----");
    DEBUG("MAC: " + myMacAddress);
    DEBUG("IP: " + myIpAddress);
    //Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      DEBUG("Error initializing ESP-NOW");
      return;
    }else{
      DEBUG("ESP-NOW Initialized");
    }
}

void printMACaddress(){
  DEBUG("MAC address: ");
  DEBUG(WiFi.macAddress());
}

void registerReceiveCb(void (*onDataRecv)(const uint8_t*, const uint8_t*, int)){
  esp_now_register_recv_cb(onDataRecv);
}

void registerTransmitCb(void (*onDataSent)(const uint8_t*, esp_now_send_status_t)){
  esp_now_register_send_cb(onDataSent);
}

bool registerPeer(const uint8_t *peerMACAddress){
  // Look at this for dynamic pairing ideas using broadcast https://dronebotworkshop.com/esp-now/
  esp_now_peer_info_t peerInfo = {};
  // Register peer
  memcpy(peerInfo.peer_addr, peerMACAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
      DEBUG("Failed to add peer");
      return 0;
  }else{
      DEBUG("Peer added");
      return 1;
  }
}

void sendData(const uint8_t *peerMACAddress, const uint8_t *data, int len){
  esp_err_t result = esp_now_send(peerMACAddress, data, len);
  if (result == ESP_OK) {
    DEBUG("Sent with success");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    DEBUG("ESPNOW not Init.");
  }
  else if (result == ESP_ERR_ESPNOW_ARG) {
    DEBUG("Invalid Argument");
  }
  else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    DEBUG("Internal Error");
  }
  else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    DEBUG("ESP_ERR_ESPNOW_NO_MEM");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    DEBUG("Peer not found.");
  }
  else {
    DEBUG("Not sure what happened");
  }
}