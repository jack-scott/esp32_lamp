#pragma once

struct RGB_colour{
    int r;
    int g;
    int b;
};

struct HSV_colour{
    int h;
    int s;
    int v;
};


struct HSV_float{
    float h;
    float s;
    float v;
};


class ColorHSV {
public:
    ColorHSV(HSV_float hsv_input);
    HSV_float applyBounds(HSV_float hsv_input);
    HSV_float normalize(HSV_float hsv_input);
    float hsvToComponent(HSV_float hsv_input, float k);
    void set_h(float h);
    void set_s(float s);
    void set_v(float v);
    float get_h();
    float get_s();
    float get_v();
    RGB_colour toRGB();

private:
    HSV_float hsv;
};
