#include <Wire.h>
#include <FastLED.h>
#include <custom_types.h>   
#include <colour_utils.h>
#include <custom_animations.h>

#include <now_comms.h>
#include <lamp_control_types.h>
#include "states.h"
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>  // standard library

#include "wifi_server.h"

#ifdef DEBUG_OUT
  #define WAIT_FOR_SERIAL 1000
#else
  #define WAIT_FOR_SERIAL 0
#endif

#include <Debug.hpp>

#ifdef CAMERA_MODEL_TTGO_T_CAMERA_PLUS
#define NUM_LEDS 22
#define LED_PIN 23
#define FASTLED_ESP32_I2S
#endif

#ifdef ALIEXPRESS_ESP32C3
#define NUM_LEDS 24
#define LED_PIN 10
// #define LED_PIN 6
#endif

#ifdef TENERGY_ESP32S3  //prototype for plane
#define NUM_LEDS 12
#define LED_PIN 23
#define FASTLED_ESP32_I2S
#endif


#ifdef SEEED_XIAO_ESP32S3
#define NUM_LEDS 24
#define LED_PIN 4 //9
#define FASTLED_ESP32_I2S
#endif


LampState state;

HSV_float float_leds[NUM_LEDS];
CRGB leds[NUM_LEDS];

class GentleColourChange {
public:
    GentleColourChange(){

    }
    GentleColourChange(int num_leds, float h_min, float h_max, float s_min, float s_max, float update_speed, float h_increment, float s_increment):
    num_leds_(num_leds),
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
        for (int i = 0; i < this->num_leds_; i++) {
            float_leds[i].h = start_h;
            float_leds[i].s = start_s;
            float_leds[i].v = state.fade_state.curr_brightness;
        }
        this->curr_colour.h = start_h;
        this->curr_colour.s = start_s;
        this->curr_colour.v = state.fade_state.curr_brightness;
    }

    // TODO: Time how long this takes to run. Might be very slow
    void tick(){
        HSV_float last_led = this->curr_colour;

        std::pair<float, int> bounce_h_result = bounce(last_led.h, state.fade_state.h_last_direction, this->h_min_, this->h_max_, this->h_increment_);
        this->curr_colour.h = bounce_h_result.first;
        state.fade_state.h_last_direction = bounce_h_result.second;

        std::pair<float, int> bounce_s_result = bounce(last_led.s, state.fade_state.s_last_direction, this->s_min_, this->s_max_, this->s_increment_);
        this->curr_colour.s = bounce_s_result.first;
        state.fade_state.s_last_direction = bounce_s_result.second;


        this->curr_colour.v = state.fade_state.curr_brightness;

        float_leds[state.fade_state.curr_led] = applyBounds(this->curr_colour);

        //potentially i want to update curr_colour with the bounds here, but i dont think it matters

        HSV_uint8 hsv_uint = floatToFastLEDFormat(float_leds[state.fade_state.curr_led]);
        CHSV hsv_out = CHSV(hsv_uint.h, hsv_uint.s, hsv_uint.v);
        // hsv2rgb_rainbow(hsv_out, rgb_out);       //retains more yellow
        CRGB rgb_out;
        hsv2rgb_spectrum(hsv_out, rgb_out);         //original colourspace for frosty fruit

        leds[state.fade_state.curr_led] = rgb_out;

        std::pair<int, int> bounce_result = bounce(state.fade_state.curr_led, state.fade_state.led_last_direction, 0, this->num_leds_ - 1, 1);
        state.fade_state.curr_led = bounce_result.first;
        state.fade_state.led_last_direction = bounce_result.second;
    }
private:
    int num_leds_;
    float h_min_;
    float h_max_;
    float s_min_;
    float s_max_;
    float update_speed_;
    float h_increment_;
    float s_increment_;
    HSV_float curr_colour = {0, 0, 0};

};


GentleColourChange frosty_fruit;
LampWebPage web_page;

LampControl local_state;

hw_timer_t *led_update_timer = NULL;


void recieveCb(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
//   LampControl myData;
  memcpy(&local_state, incomingData, sizeof(local_state));
  // Update the structures with the new incoming data
  DEBUG("state: ");
  DEBUG(local_state.state);
  DEBUG("brightness: ");
  DEBUG(local_state.brightness);
  DEBUG("");

//   if (local_state.state == 1) {
//     state.sys_state = toggleSystemState(state.sys_state);
//   }

//   if(local_state.enable_web == 1){
//     state.web_enabled = !state.web_enabled;
//   }
}

void initCurrColour(){
    switch (state.colour_mode)  
    {
    case ColourMode::FROSTYFRUIT:
        frosty_fruit.initColour();
        break;
    case ColourMode::BLUE:
        break;
    default:
        DEBUG("Unknown colour somehow");
        break;
    }
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
        DEBUG("Unknown colour somehow");
        break;
    }
}


// This logic needs a rewrite, too hard to read

// void runStateModifier(){
//     switch (state.sys_state)
//     {
//     case SystemState::OFF:
//         break;
//     case SystemState::FADEIN:
//         if(state.fade_state.curr_brightness == 0 && state.sys_state != SystemState::OFF){
//             state.fade_state.led_last_direction = !state.fade_state.led_last_direction;
//             state.fade_state.curr_brightness = state.brightness;
//         }else if(state.sys_state == SystemState::OFF)
//         {
//             state.fade_state.last_led=0;
//             float_leds[state.fade_state.last_led].v = state.brightness;
//             // first_led.v = state.brightness;first_led
//             state.fade_state.curr_led = 1;
//             state.fade_state.led_last_direction = -1;
//             state.fade_state.curr_brightness = state.brightness;
//         }
//         else if (state.fade_state.curr_brightness == state.brightness && state.fade_state.curr_led == NUM_LEDS)
//         {
//             state.sys_state = SystemState::NOMINAL;
//         }
//         break;
//     case SystemState::FADEOUT:
//         if(state.fade_state.curr_brightness > 0){
//             state.fade_state.last_led=NUM_LEDS;
//             float_leds[state.fade_state.last_led].v = state.brightness;
//             // first_led.v = state.brightness;
//             state.fade_state.curr_led = NUM_LEDS - 1;
//             state.fade_state.led_last_direction = -1;
//             state.fade_state.curr_brightness = 0;
//         }else if (state.fade_state.curr_brightness == 0 &&  state.fade_state.curr_led == 0)
//         {
//             state.sys_state = SystemState::OFF;
//         }
//         break;
//     case SystemState::NOMINAL:
//         break;
//     default:
//         DEBUG("UnknowUnknownn state somehow");
//         break;
//     }
// }

void runStateModifier(){
    switch (state.sys_state)
    {
    case SystemState::OFF:
        break;
    case SystemState::FADEIN:
        DEBUG("############### FADEIN ###############");
        printState(state);
        if(state.fade_state.curr_brightness == 0 && state.last_sys_state == SystemState::OFF){ 
            state.fade_state.curr_led = 0;
            state.fade_state.led_last_direction = 1;
            state.fade_state.curr_brightness = state.brightness;
            initCurrColour();
        }else if(state.fade_state.curr_brightness == 0 && state.last_sys_state == SystemState::FADEOUT){
            state.fade_state.led_last_direction = 1;
            state.fade_state.curr_brightness = state.brightness;
        }else if (state.fade_state.curr_led == NUM_LEDS - 1)
        {
            setSystemState(SystemState::NOMINAL, state);
        }else{
            runColourModifier();
        }
        break;
    case SystemState::FADEOUT:
        DEBUG("############### FADEOUT ###############");
        printState(state);
        if(state.fade_state.curr_brightness > 0){
            if(state.last_sys_state == SystemState::FADEIN){
                state.fade_state.led_last_direction = -1;
                state.fade_state.curr_brightness = 0;
            }else{  // Ideally this is only happening when in NOMINAL state but this is safe from any system state
                state.fade_state.curr_led = NUM_LEDS - 1;
                state.fade_state.led_last_direction = -1;
                state.fade_state.curr_brightness = 0;
            }
        }else if (state.fade_state.curr_led == 0)
        {
            float_leds[state.fade_state.curr_led].v = 0;
            leds[state.fade_state.curr_led] = CRGB(0, 0, 0);
            setSystemState(SystemState::OFF, state);
        }else {
            float_leds[state.fade_state.curr_led].v = 0;
            leds[state.fade_state.curr_led] = CRGB(0, 0, 0);
            state.fade_state.curr_led--;
        }
        break;
    case SystemState::NOMINAL:
        runColourModifier();
        break;
    default:
        DEBUG("UnknowUnknownn state somehow");
        break;
    }
}


//TODO: Get led update running in here
void IRAM_ATTR ledCallback(){
    // runColourModifier();
    // FastLED.show();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}



void setup() {
    Serial.begin(115200);
    delay(1000); // give me time to bring up serial monitor
    DEBUG("Starting up");
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    // WiFi.mode(WIFI_OFF);
    // setupWebServer();
    // Initialise the NowComms library
    setupEspNow();
    // web_page.SetupServer();
    pinMode(LED_BUILTIN, OUTPUT);
    registerReceiveCb(recieveCb);
    state = loadState();
    printState(state);
    float h_min = 8.0f;
    float h_max = 12.0f;
    float s_min = 238.0f;
    float s_max = 245.0f;
    // float h_min = 7.5f;
    // float h_max = 12.5f;
    // float s_min = 228.0f;
    // float s_max = 245.0f;
    
    float update_speed = 0.08f; //not used
    float h_increment = 0.1;
    float s_increment = 0.05f;
    frosty_fruit = GentleColourChange(NUM_LEDS, h_min, h_max, s_min, s_max, update_speed, h_increment, s_increment);
    frosty_fruit.initColour();
    printMACaddress();
    // web_page.printIp();

    led_update_timer = timerBegin(0, 80, true);    //timer 0, div 80, is 1Mhz clock
    timerAttachInterrupt(led_update_timer, &ledCallback, true);
    timerAlarmWrite(led_update_timer, 1000000, true); //set time in us
    timerAlarmEnable(led_update_timer);

}

void checkLocalState(){
    if (local_state.state == 1) {
        toggleSystemState(state);
        local_state.state = 0;
    }
    if(local_state.enable_web == 1){
        state.web_enabled = !state.web_enabled;
        local_state.enable_web = 0;
    }
}

unsigned long cur_ms=millis();

void loop() {
    if (millis() - cur_ms > 100) {
        cur_ms = millis();
        runStateModifier();
        // runColourModifier();
        FastLED.show();
    }
    checkLocalState();
    // runColourModifier();
    // runStateModifier();
    // FastLED.show();
    // printState(state);
    // printLEDsfast(leds, NUM_LEDS);
    // printLEDsHSV(float_leds, NUM_LEDS);
    // delay(1000);
    // web_page.tick();
    delay(1);
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
    ** Adjust the lamp cad to use ESP32-S3 Seed

    ** Allow web server to update the state struct
    ** Test leds and webserver
    ** Change the light modifier to update all leds at the same time for cleaner effects
    ** Change fade in/fade out function to be a specific colour modifier that starts from the top or bottom
    ** Add some better colour options
    ** If webserver + LEDs is not viable then make a way to change between the 2 states. Might need to be careful with 
    wifi settings here
    ** Look at controller wake on interrupt functions. might be hard to actually test or work on
    ** Save config to persistant memory
    ** Fix the issue with serial print making everything slow when not connected 
*/
