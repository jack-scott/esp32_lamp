#ifndef STATES_H
#define STATES_H

#include <Debug.hpp>

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
    int h_last_direction;
    int s_last_direction;
    int led_last_direction;
    int curr_led;
    int curr_brightness;
} FadeState;


typedef struct LampState {
    ColourMode colour_mode;
    SystemState sys_state;
    SystemState last_sys_state;
    FadeState fade_state;
    uint8_t brightness;
    bool web_enabled;

}LampState;


void loadDefaults(LampState& curr_state){
    curr_state.colour_mode = FROSTYFRUIT;
    curr_state.last_sys_state = OFF;
    curr_state.sys_state = NOMINAL;
    curr_state.brightness = 50;

    curr_state.fade_state.h_last_direction = 1;
    curr_state.fade_state.s_last_direction = 1;
    curr_state.fade_state.led_last_direction = 1;
    curr_state.fade_state.curr_led = 0;
    curr_state.fade_state.curr_brightness = curr_state.brightness;
}

LampState loadState(){
    //TODO load from mem
    LampState sys;
    loadDefaults(sys);
    return sys;
}

void printState(LampState state){
    DEBUG("Colour: ");
    DEBUG(state.colour_mode);
    DEBUG("Sys state:");
    DEBUG(state.sys_state);
    DEBUG("Last sys state:");
    DEBUG(state.last_sys_state);
    DEBUG("Brightness:");
    DEBUG(state.brightness);
    DEBUG("Web enabled: ");
    DEBUG(state.web_enabled);

    DEBUG("H dir: ");
    DEBUG(state.fade_state.h_last_direction);
    DEBUG("S dir: ");
    DEBUG(state.fade_state.s_last_direction);
    DEBUG("led dir: ");

    DEBUG(state.fade_state.led_last_direction);
    DEBUG("Curr led: ");
    DEBUG(state.fade_state.curr_led);
    DEBUG("Curr brightness: ");
    DEBUG(state.fade_state.curr_brightness);
}

void setSystemState(SystemState new_state, LampState& curr_state){
    curr_state.last_sys_state = curr_state.sys_state;
    curr_state.sys_state = new_state;
}

void toggleSystemState(LampState& curr_state){
    if(curr_state.sys_state == SystemState::OFF || curr_state.sys_state == SystemState::FADEOUT){
        setSystemState(SystemState::FADEIN, curr_state);
    }else{
        setSystemState(SystemState::FADEOUT, curr_state);
    }
}


#endif  