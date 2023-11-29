#include <Wire.h>
#include <FastLED.h>

// #define CAMERA_MODEL_TTGO_T_CAMERA_PLUS

#define FASTLED_ESP32_I2S

#define NUM_LEDS 22
#define LED_PIN 23

int sweep(int input_val, int min, int max, int increment) {
    if (increment <= 0) {
        printf("Increment must be greater than 0\n");
        return input_val;
    }
    if (input_val < min) {
        input_val = min;
    } else if (input_val < max) {
        input_val += increment;
    } else {
        input_val = min;
    }
    return input_val;
}

float sweep(float input_val, float min, float max, float increment) {
    if (increment <= 0) {
        printf("Increment must be greater than 0\n");
        return input_val;
    }
    if (input_val < min) {
        input_val = min;
    } else if (input_val < max) {
        input_val += increment;
    } else {
        input_val = min;
    }
    return input_val;
}


std::pair<int, int> bounce(int input_val, int last_direction, int min, int max, int increment) {
    if (input_val < max && last_direction == 1) {
        input_val += increment;
    } else if (input_val >= max && last_direction == 1) {
        input_val -= increment;
        last_direction = -1;
    } else if (input_val > min && last_direction == -1) {
        input_val -= increment;
    } else if (input_val <= min && last_direction == -1) {
        input_val += increment;
        last_direction = 1;
    }
    return std::make_pair(input_val, last_direction);
}


std::pair<float, int> bounce(float input_val, int last_direction, float min, float max, float increment) {
    if (input_val < max && last_direction == 1) {
        input_val += increment;
    } else if (input_val >= max && last_direction == 1) {
        input_val -= increment;
        last_direction = -1;
    } else if (input_val > min && last_direction == -1) {
        input_val -= increment;
    } else if (input_val <= min && last_direction == -1) {
        input_val += increment;
        last_direction = 1;
    }
    return std::make_pair(input_val, last_direction);
}



struct RGB_colour
{
    int r;
    int g;
    int b;
};

struct HSV_colour
{
    int h;
    int s;
    int v;
};

struct HSV_float
{
    float h;
    float s;
    float v;
};

class ColorHSV {
public:
    /**
     * https://en.wikipedia.org/wiki/HSL_and_HSV#Color_conversion_formulae
    */
    ColorHSV(HSV_float hsv_input) {
        this->hsv.h = hsv_input.h;
        HSV_float bounds = applyBounds(hsv_input);
        this->hsv.s = bounds.s;
        this->hsv.v = bounds.v;
    }

    HSV_colour applyBounds(HSV_colour hsv_input) {
        HSV_colour hsv_out;
        hsv_out.h = hsv_input.h % 360;
        hsv_out.s = std::min(255, std::max(0, hsv_input.s));
        hsv_out.v = std::min(255, std::max(0, hsv_input.v));
        return hsv_out;
    }

    HSV_float applyBounds(HSV_float hsv_input) {
        HSV_float hsv_out;
        hsv_out.h = hsv_input.h - std::remainder(hsv_input.h, 360.0f);
        hsv_out.s = std::min(255.0f, std::max(0.0f, hsv_input.s));
        hsv_out.v = std::min(255.0f, std::max(0.0f, hsv_input.v));
        return hsv_out;
    }

    HSV_float normalize(HSV_colour hsv_input) {
        HSV_float hsv_out;
        hsv_out.h = static_cast<float>(hsv_input.h);
        hsv_out.s = static_cast<float>(hsv_input.s) / 255;
        hsv_out.v = static_cast<float>(hsv_input.v) / 255;
        return hsv_out;
    }
    
    HSV_float normalize(HSV_float hsv_input) {
        HSV_float hsv_out;
        hsv_out.h = hsv_input.h;
        hsv_out.s = hsv_input.s / 255.0f;
        hsv_out.v = hsv_input.v / 255.0f;
        return hsv_out;
    }

    float hsvToComponent(HSV_float hsv_input, float k) {
        return ( hsv_input.v - hsv_input.v * hsv_input.s * std::max(float(0), std::min(k, float(4) - k)) );
    }

    void set_h(float h) {
        this->hsv.h = h;
    }

    void set_s(float s) {
        this->hsv.s = s;
    }

    void set_v(float v) {
        this->hsv.v = v;
    }
    
    float get_h() {
        return this->hsv.h;
    }

    float get_s() {
        return this->hsv.s;
    }

    float get_v() {
        return this->hsv.v;
    }


    RGB_colour toRGB() {
        HSV_float bounded_hsv = applyBounds(this->hsv);

        HSV_float normalized_hsv = normalize(bounded_hsv);

        float r_k = std::remainder((5 + normalized_hsv.h / 60), 6);
        float r = hsvToComponent(normalized_hsv, r_k);

        float g_k = std::remainder((3 + normalized_hsv.h / 60), 6);
        float g = hsvToComponent(normalized_hsv, g_k);

        float b_k = std::remainder((1 + normalized_hsv.h / 60), 6);
        float b = hsvToComponent(normalized_hsv, b_k);

        RGB_colour rgb_out;
        rgb_out.r = static_cast<int>(r * 255);
        rgb_out.g = static_cast<int>(g * 255);
        rgb_out.b = static_cast<int>(b * 255);

        return rgb_out;
    }

private:
    HSV_float hsv;
};




class DeliciousFrostyFruit {
public:
    CRGB* strip;
    int num_leds;

    float h_min;
    float h_max;
    float s_min;
    float s_max;
    float v;

    ColorHSV cmd_colour;
    float update_speed;
    float h_increment;
    float s_increment;

    DeliciousFrostyFruit(CRGB* strip, int num_leds):
        strip(strip),
        num_leds(num_leds),
        cmd_colour(ColorHSV(HSV_float{0.0f, 0.0f, 0.0f})){

        this->h_min = 8.0f;
        this->h_max = 12.0f;
        this->s_min = 238.0f;
        this->s_max = 245.0f;
        this->v = 50.0f;

        float h_start = static_cast<float>(random(this->h_min, this->h_max));
        float s_start = static_cast<float>(random(this->s_min, this->s_max));

        
        this->cmd_colour.set_h(h_start);
        this->cmd_colour.set_s(s_start);
        this->cmd_colour.set_v(this->v);

        this->update_speed = 0.08f;
        this->h_increment = 0.1;
        this->s_increment = 0.05f;
    }

    void run() {
        int h_last_direction = 1;
        int s_last_direction = 1;
        int led_last_direction = 1;
        int curr_led = 0;

        while (true) {
            std::pair<float, int> bounce_result = bounce(curr_led, led_last_direction, 0, this->num_leds - 1, 1);
            curr_led = bounce_result.first;
            led_last_direction = bounce_result.second;

            std::pair<float, int> bounce_h_result = bounce(this->cmd_colour.get_h(), h_last_direction, this->h_min, this->h_max, this->h_increment);
            this->cmd_colour.set_h(bounce_h_result.first);
            h_last_direction = bounce_h_result.second;

            std::pair<float, int> bounce_s_result = bounce(this->cmd_colour.get_s(), s_last_direction, this->s_min, this->s_max, this->s_increment);
            this->cmd_colour.set_s(bounce_s_result.first);
            s_last_direction = bounce_s_result.second;

            RGB_colour rgb_out = this->cmd_colour.toRGB();

            this->strip[curr_led] = CRGB(rgb_out.r, rgb_out.g, rgb_out.b);

            delay(this->update_speed * 1000);
            FastLED.show();
            Serial.print("h:"); Serial.println(this->cmd_colour.get_h());
            Serial.print("s:");Serial.println(this->cmd_colour.get_s());
            Serial.print("v:");Serial.println(this->cmd_colour.get_v());
            Serial.print("r:");Serial.println(rgb_out.r);
            Serial.print("g:");Serial.println(rgb_out.g);
            Serial.print("b:");Serial.println(rgb_out.b);
            Serial.println("-----");
        }
    }
};

CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}

void loop() {
    DeliciousFrostyFruit(leds, NUM_LEDS).run();
}

