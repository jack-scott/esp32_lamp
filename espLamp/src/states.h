#pragma once

enum ColourMode{
    FROSTYFRUIT,
    BLUE
};


enum SystemState{
    OFF,
    FADEIN,
    FADEOUT,
    NOMINAL
};

typedef struct FadeState{
    bool h_last_direction;
    bool s_last_direction;
    bool led_last_direction;
    int curr_led;
    int last_led;
    int curr_brightness;
} FadeState;


typedef struct LampState {
    ColourMode colour_mode;
    SystemState sys_state;
    FadeState fade_state;
    uint8_t brightness;
    bool web_enabled;

}LampState;


void loadDefaults(LampState& curr_state){
    curr_state.colour_mode = BLUE;
    curr_state.sys_state = NOMINAL;
    curr_state.brightness = 50;

    curr_state.fade_state.h_last_direction = 1;
    curr_state.fade_state.s_last_direction = 1;
    curr_state.fade_state.led_last_direction = 1;
    curr_state.fade_state.last_led = 0;
    curr_state.fade_state.curr_led = 1;
    curr_state.fade_state.curr_brightness = curr_state.brightness;
}

LampState loadState(){
    //TODO load from mem
    LampState sys;
    loadDefaults(sys);
    return sys;
}

void printState(LampState state){
    Serial.print("Colour: ");
    Serial.println(state.colour_mode);
    Serial.print("Sys state:");
    Serial.println(state.sys_state);
    Serial.print("Brightness:");
    Serial.println(state.brightness);
    Serial.print("Web enabled: ");
    Serial.println(state.web_enabled);

    Serial.print("H dir: ");
    Serial.println(state.fade_state.h_last_direction);
    Serial.print("S dir: ");
    Serial.println(state.fade_state.s_last_direction);
    Serial.print("led dir: ");

    Serial.println(state.fade_state.led_last_direction);
    Serial.print("Curr led: ");
    Serial.println(state.fade_state.curr_led);
    Serial.print("Curr brightness: ");
    Serial.println(state.fade_state.curr_brightness);
}