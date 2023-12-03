#include <Wire.h>
#include <FastLED.h>
// #include <hsv_to_rgb.h>
#include <custom_types.h>   
#include <colour_utils.h>

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


class DeliciousFrostyFruit {
public:
    CRGB* strip;
    int num_leds;

    float h_min;
    float h_max;
    float s_min;
    float s_max;

    HSV_float cmd_colour;

    float update_speed;
    float h_increment;
    float s_increment;

    DeliciousFrostyFruit(CRGB* strip, int num_leds):
        strip(strip),
        num_leds(num_leds),
        cmd_colour({0.0f, 0.0f, 0.0f}) {
        this->h_min = 8.0f;
        this->h_max = 12.0f;
        this->s_min = 238.0f;
        this->s_max = 245.0f;

        float h_start = static_cast<float>(random(this->h_min, this->h_max));
        float s_start = static_cast<float>(random(this->s_min, this->s_max));

        
        this->cmd_colour.h = h_start;
        this->cmd_colour.s = s_start;
        this->cmd_colour.v = 50.0f;

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

            std::pair<float, int> bounce_h_result = bounce(this->cmd_colour.h, h_last_direction, this->h_min, this->h_max, this->h_increment);
            this->cmd_colour.h = bounce_h_result.first;
            h_last_direction = bounce_h_result.second;

            std::pair<float, int> bounce_s_result = bounce(this->cmd_colour.s, s_last_direction, this->s_min, this->s_max, this->s_increment);
            this->cmd_colour.s = bounce_s_result.first;
            s_last_direction = bounce_s_result.second;

            this->cmd_colour = applyBounds(this->cmd_colour);
            CHSV hsv_out = CHSV(this->cmd_colour.h, this->cmd_colour.s, this->cmd_colour.v);
            CRGB rgb_out;
            // hsv2rgb_rainbow(hsv_out, rgb_out);       //retains more yellow
            hsv2rgb_spectrum(hsv_out, rgb_out);         //original colourspace for frosty fruit
            this->strip[curr_led] = rgb_out;

            delay(this->update_speed * 1000);
            FastLED.show();
            //print out the hsv before after and rgb
            Serial.printf("h: %f, s: %f, v: %f\n", this->cmd_colour.h, this->cmd_colour.s, this->cmd_colour.v);
            Serial.printf("h: %d, s: %d, v: %d\n", hsv_out.h, hsv_out.s, hsv_out.v);
            Serial.printf("r: %d, g: %d, b: %d\n", rgb_out.r, rgb_out.g, rgb_out.b);


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

