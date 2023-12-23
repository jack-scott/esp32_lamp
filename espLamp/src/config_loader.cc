/*
this file grabs config from compiler flags and converts them to a struct to be passed around
if you are getting compile errors about missing flags then copy the private_config.template.ini 
from the config folder and rename it private_config.ini, fill in the values then run the build 
again
*/

#include "config_loader.h"

void convertMACAddress(String inputMACAddress, uint8_t* targetMacAddress) {
  char* token = strtok(const_cast<char*>(inputMACAddress.c_str()), ":");
  for (int i = 0; i < 6; i++) {
    targetMacAddress[i] = strtol(token, nullptr, 16);
    token = strtok(nullptr, ":");
  }
}


bool loadConfig(DeviceConfig& config) {
  #ifndef TARGETMAC
    #error Need to provide TARGETMAC
  #endif
  convertMACAddress(TARGETMAC, config.targetMacAddress);
  return true;
}
