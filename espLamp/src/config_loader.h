#pragma once

#include <Arduino.h>
#include <cstdint>

struct DeviceConfig
{
    uint8_t targetMacAddress[6];
};

bool loadConfig(DeviceConfig& config);
void convertMACAddress(String inputMACAddress, uint8_t* targetMacAddress);