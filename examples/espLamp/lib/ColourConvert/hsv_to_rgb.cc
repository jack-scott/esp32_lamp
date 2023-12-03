#include <hsv_to_rgb.h>
#include <math.h>
#include <algorithm> // Include the <algorithm> header for std::min and std::max
#include <cmath>

/**
 * https://en.wikipedia.org/wiki/HSL_and_HSV#Color_conversion_formulae
*/
ColorHSV::ColorHSV(HSV_float hsv_input) {
    HSV_float bounds = applyBounds(hsv_input);
    this->hsv.h = bounds.h;
    this->hsv.s = bounds.s;
    this->hsv.v = bounds.v;
}

HSV_float ColorHSV::applyBounds(HSV_float hsv_input) {
    HSV_float hsv_out;
    hsv_out.h = std::max(0.0f, fmodf(hsv_input.h, 360.0f));
    hsv_out.s = std::min(255.0f, std::max(0.0f, hsv_input.s));
    hsv_out.v = std::min(255.0f, std::max(0.0f, hsv_input.v));
    return hsv_out;
}


HSV_float ColorHSV::normalize(HSV_float hsv_input) {
    HSV_float hsv_out;
    hsv_out.h = hsv_input.h;
    hsv_out.s = hsv_input.s / 255.0f;
    hsv_out.v = hsv_input.v / 255.0f;
    return hsv_out;
}

float ColorHSV::hsvToComponent(HSV_float hsv_input, float k) {
    return ( hsv_input.v - hsv_input.v * hsv_input.s * std::max(0.0f, std::min(1.0f, std::min(k, 4.0f - k))) );
}

void ColorHSV::set_h(float h) {
    this->hsv.h = h;
}

void ColorHSV::set_s(float s) {
    this->hsv.s = s;
}

void ColorHSV::set_v(float v) {
    this->hsv.v = v;
}

float ColorHSV::get_h() {
    return this->hsv.h;
}

float ColorHSV::get_s() {
    return this->hsv.s;
}

float ColorHSV::get_v() {
    return this->hsv.v;
}


RGB_colour ColorHSV::toRGB() {
    HSV_float bounded_hsv = applyBounds(this->hsv);

    HSV_float normalized_hsv = normalize(bounded_hsv);

    float r_k = fmodf((5 + normalized_hsv.h / 60.0f), 6);
    float r = hsvToComponent(normalized_hsv, r_k);

    float g_k = fmodf((3 + normalized_hsv.h / 60.0f), 6);
    float g = hsvToComponent(normalized_hsv, g_k);

    float b_k = fmodf((1 + normalized_hsv.h / 60.0f), 6);
    float b = hsvToComponent(normalized_hsv, b_k);

    RGB_colour rgb_out;
    rgb_out.r = static_cast<int>(r * 255);
    rgb_out.g = static_cast<int>(g * 255);
    rgb_out.b = static_cast<int>(b * 255);

    return rgb_out;
}

