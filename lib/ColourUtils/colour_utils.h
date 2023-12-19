#pragma once

#include <custom_types.h>
#include <algorithm> // Include the <algorithm> header for std::min and std::max
#include <cmath>
#include <cstdint> // Include the header file for uint8_t

HSV_uint8 floatToFastLEDFormat(HSV_float hsv_input){
    HSV_uint8 hsv_out;
    hsv_out.h = static_cast<uint8_t>((hsv_input.h / 360.0f) * 255.0f);
    hsv_out.s = static_cast<uint8_t>(hsv_input.s);
    hsv_out.v = static_cast<uint8_t>(hsv_input.v);
    return hsv_out;
}

HSV_float applyBounds(HSV_float hsv_input) {
    HSV_float hsv_out;
    hsv_out.h = std::max(0.0f, fmodf(hsv_input.h, 360.0f));
    hsv_out.s = std::min(255.0f, std::max(0.0f, hsv_input.s));
    hsv_out.v = std::min(255.0f, std::max(0.0f, hsv_input.v));
    return hsv_out;
}


void printLEDsHSV(HSV_float* leds, int num_leds){
    for (int i = 0; i < num_leds; i++) {
        Serial.print("LEDhsv: ");
        Serial.print(i);
        Serial.print(" H: ");
        Serial.print(leds[i].h);
        Serial.print(" S: ");
        Serial.print(leds[i].s);
        Serial.print(" V: ");
        Serial.println(leds[i].v);
    }
}

void printLEDsfast(CRGB* leds, int num_leds){
    for (int i = 0; i < num_leds; i++) {
        Serial.print("LED fast: ");
        Serial.print(i);
        Serial.print(" R: ");
        Serial.print(leds[i].r);
        Serial.print(" G: ");
        Serial.print(leds[i].g);
        Serial.print(" B: ");
        Serial.println(leds[i].b);
    }
}
