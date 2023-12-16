#include <Wire.h>
#include <FastLED.h>
#include <custom_types.h>   
#include <colour_utils.h>
#include <now_comms.h>
#include <lamp_control_types.h>
#include "states.h"
#include <Preferences.h>


#define FASTLED_ESP32_I2S

#ifdef CAMERA_MODEL_TTGO_T_CAMERA_PLUS
#define NUM_LEDS 22
#define LED_PIN 23
#endif

#ifdef ALIEXPRESS_ESP32C3
#define NUM_LEDS 24
#define LED_PIN 6
#endif

#ifdef TENERGY_ESP32S3
#define NUM_LEDS 12
#define LED_PIN 23
#endif

LampState state;

HSV_float float_leds[NUM_LEDS];
CRGB leds[NUM_LEDS];

void toggleState(){
    if(state.sys_state == SystemState::OFF || state.sys_state == SystemState::FADEOUT){
        state.sys_state = SystemState::FADEIN;
    }else{
        state.sys_state = SystemState::FADEOUT;
    }
}

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


class GentleColourChange {
public:
    GentleColourChange(){

    }
    GentleColourChange(float h_min, float h_max, float s_min, float s_max, float update_speed, float h_increment, float s_increment):
    h_min_(h_min),
    h_max_(h_max),
    s_min_(s_min),
    s_max_(s_max),
    update_speed_(update_speed),
    h_increment_(h_increment),
    s_increment_(s_increment)
    {

    }

    void initColour(){
        state.fade_state.curr_led = 0;
        float start_h = static_cast<float>(random(this->h_min_, this->h_max_));
        float start_s = static_cast<float>(random(this->s_min_, this->s_max_));
        for (int i = 0; i < NUM_LEDS; i++) {
            float_leds[i].h = start_h;
            float_leds[i].s = start_s;
            float_leds[i].v = state.fade_state.curr_brightness;
        }
    }

    void tick(){
        state.fade_state.last_led = state.fade_state.curr_led;
        HSV_float last_led = float_leds[state.fade_state.last_led];
        std::pair<float, int> bounce_result = bounce(state.fade_state.curr_led, state.fade_state.led_last_direction, 0, NUM_LEDS - 1, 1);
        state.fade_state.curr_led = bounce_result.first;
        state.fade_state.led_last_direction = bounce_result.second;

        // if (state == true) {
        std::pair<float, int> bounce_h_result = bounce(last_led.h, state.fade_state.h_last_direction, this->h_min_, this->h_max_, this->h_increment_);
        float_leds[state.fade_state.curr_led].h = bounce_h_result.first;
        state.fade_state.h_last_direction = bounce_h_result.second;

        std::pair<float, int> bounce_s_result = bounce(last_led.s, state.fade_state.s_last_direction, this->s_min_, this->s_max_, this->s_increment_);
        float_leds[state.fade_state.curr_led].s = bounce_s_result.first;
        state.fade_state.s_last_direction = bounce_s_result.second;


        float_leds[state.fade_state.curr_led].v = state.fade_state.curr_brightness;

        float_leds[state.fade_state.curr_led] = applyBounds(float_leds[state.fade_state.curr_led]);
        CHSV hsv_out = CHSV(float_leds[state.fade_state.curr_led].h, float_leds[state.fade_state.curr_led].s, float_leds[state.fade_state.curr_led].v);
        // hsv2rgb_rainbow(hsv_out, rgb_out);       //retains more yellow
        CRGB rgb_out;
        hsv2rgb_spectrum(hsv_out, rgb_out);         //original colourspace for frosty fruit
        // }else{
            // rgb_out = CRGB(0,0,0);
        // }

        leds[state.fade_state.curr_led] = rgb_out;
    }
private:
    float h_min_;
    float h_max_;
    float s_min_;
    float s_max_;
    float update_speed_;
    float h_increment_;
    float s_increment_;
};


GentleColourChange frosty_fruit;

LampControl local_state;

void recieveCb(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
//   LampControl myData;
  memcpy(&local_state, incomingData, sizeof(local_state));
  // Update the structures with the new incoming data
  Serial.print("state: ");
  Serial.println(local_state.state);
  Serial.print("brightness: ");
  Serial.println(local_state.brightness);
  Serial.println();

//   if (local_state.state == 1) {
//     toggleState();
//   }

//   if(local_state.enable_web == 1){
//     state.web_enabled = !state.web_enabled;
//   }
}

void runColourModifier(){
    switch (state.colour_mode)
    {
    case ColourMode::FROSTYFRUIT:
        frosty_fruit.tick();
        break;
    case ColourMode::BLUE:
        break;
    default:
        Serial.println("Unknown colour somehow");
        break;
    }
}



void runStateModifier(){
    switch (state.sys_state)
    {
    case SystemState::OFF:
        break;
    case SystemState::FADEIN:
        // if(state.fade_state.curr_brightness == 0 && state.sys_state != SystemState::OFF){
        //     state.fade_state.led_last_direction = !state.fade_state.led_last_direction;
        //     state.fade_state.curr_brightness = state.brightness;
        // }else if(state.sys_state == SystemState::OFF)
        // {
        state.fade_state.last_led=0;
        float_leds[state.fade_state.last_led].v = state.brightness;
        // first_led.v = state.brightness;first_led
        state.fade_state.curr_led = 1;
        state.fade_state.led_last_direction = -1;
        state.fade_state.curr_brightness = state.brightness;
    // }
    // else if (state.fade_state.curr_brightness == state.brightness && state.fade_state.curr_led == NUM_LEDS)
    // {
        state.sys_state = SystemState::NOMINAL;
        // }
        break;
    case SystemState::FADEOUT:
        // if(state.fade_state.curr_brightness > 0){
            state.fade_state.last_led=NUM_LEDS;
            float_leds[state.fade_state.last_led].v = state.brightness;
            // first_led.v = state.brightness;
            state.fade_state.curr_led = NUM_LEDS - 1;
            state.fade_state.led_last_direction = -1;
            state.fade_state.curr_brightness = 0;
        // }else if (state.fade_state.curr_brightness == 0 &&  state.fade_state.curr_led == 0)
        // {
            state.sys_state = SystemState::OFF;
        // }
        break;
    case SystemState::NOMINAL:
        break;
    default:
        Serial.println("UnknowUnknownn state somehow");
        break;
    }
}

// void setupWebServer(){
//     #ifdef USE_INTRANET
//     WiFi.begin(LOCAL_SSID, LOCAL_PASS);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.print("IP address: "); Serial.println(WiFi.localIP());
//     assigned_ip = WiFi.localIP();
//     #endif

//     // if you don't have #define USE_INTRANET, here's where you will creat and access point
//     // an intranet with no internet connection. But Clients can connect to your intranet and see
//     // the web page you are about to serve up
//     #ifndef USE_INTRANET
//     // Replace with your network credentials
//     WiFi.softAP(AP_SSID, AP_PASS);
//     delay(100);
//     WiFi.softAPConfig(PageIP, gateway, subnet);
//     delay(100);
//     assigned_ip = WiFi.softAPIP();
//     Serial.print("IP address: "); Serial.println(assigned_ip);
//     #endif
// }

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

    // Initialise the NowComms library
    setupEspNow();
    registerReceiveCb(recieveCb);
    state = loadState();
    printState(state);
    float h_min = 8.0f;
    float h_max = 12.0f;
    float s_min = 238.0f;
    float s_max = 245.0f;
    
    float update_speed = 0.08f; //not used
    float h_increment = 0.1;
    float s_increment = 0.05f;
    frosty_fruit = GentleColourChange(h_min, h_max, s_min, s_max, update_speed, h_increment, s_increment);
    frosty_fruit.initColour();
    printMACaddress();
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

void checkLocalState(){
    if (local_state.state == 1) {
        toggleState();
        local_state.state = 0;
    }

    if(local_state.enable_web == 1){
        state.web_enabled = !state.web_enabled;
        local_state.enable_web = 0;
    }
}

void loop() {
    // checkLocalState();
    // runColourModifier();
    // runStateModifier();
    frosty_fruit.tick();
    // leds[0] = CRGB(0,0,0);
    FastLED.show();
    printState(state);
    // printLEDsfast(leds, NUM_LEDS);
    // printLEDsHSV(float_leds, NUM_LEDS);
    // delay(1000);
    delay(0.08*1000);
}

/*
Lamp features:

 need a state manager
 manage the change between web page serving and running leds
         -- just check that i cant run both, maybe its fine
         -- need to change the colour modes, functions/classes?
         -- Fade in fade out, works well with the global led values
         -- maybe the state management is just the async fromt eh controller and hte run different functions depending
         -- would make it easy to add new colours and stuff
 need to have memeory that is not in RAM, maybe a config file that can be loaded
        -- wifi selection
        -- colour choices
        -- new colour modes {optional}
 fix the webpage to actually change between colour modes, this is where global variables can be good
        -- can also do it with a state struct fairly easily
 connect controller functions properly
        -- click and hold to turn on and off
        -- single click to send a coloured pulse {optional}
        -- another click pattern to enable the webpage? (check if this is need or not, maybe just run all the time)

 stretch:
    -- watchdog to keep us running


Controller features:

 low power states
        -- need to run in deep sleep until woken on interrupt from the button
 set the status led functions up properly
        --  double flash for error
        -- single for good

 todo list
    ** Add and maange an expandable state struct
    ** Allow web server to update the state struct
    ** Test leds and webserver
    ** Change the light modifier to update all leds at the same time for cleaner effects
    ** Change fade in/fade out function to be a specific colour modifier that starts from the top or bottom
    ** Add some better colour options
    ** If webserver + LEDs is not viable then make a way to change between the 2 states. Might need to be careful with 
    wifi settings here
    ** Look at controller wake on interrupt functions. might be hard to actually test or work on
    ** Save config to persistant memory

*/
