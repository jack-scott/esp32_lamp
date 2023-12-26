#include <Arduino.h>
#include <OneButton.h>
#include <now_comms.h>
#include <lamp_control_types.h>

#ifdef DEBUG_OUT
  #define WAIT_FOR_SERIAL 1000
#else
  #define WAIT_FOR_SERIAL 0
#endif

#include <Debug.hpp>

#include "config_loader.h"


#ifdef CAMERA_MODEL_TTGO_T_CAMERA_PLUS
#define PIN_INPUT 23
#endif

#ifdef ALIEXPRESS_ESP32C3
#define PIN_INPUT 6
#define LED_PIN 8
#endif

#ifdef SEEED_XIAO_ESP32C3
#define PIN_INPUT 3
#define VBAT_PIN 2
#endif

#define MILLI_TILL_SLEEP 3000

OneButton *button;
DeviceConfig myConfig;

uint8_t last_brightness = 50;
int direction = 1;

void fClicked()
{
  LampControl lc;
  lc.state = 1;
  lc.brightness = last_brightness;
  sendData(myConfig.targetMacAddress, (uint8_t*)&lc, sizeof(lc));
  DEBUG("Click");
}

void fDoubleClicked()
{
  DEBUG("DoubleClick");
}

void fLongPressStart()
{
  LampControl lc;
  lc.state = 0;
  lc.brightness = last_brightness;
  sendData(myConfig.targetMacAddress, (uint8_t*)&lc, sizeof(lc));
  DEBUG("LongPressStart");
}

void fLongPressStop()
{
  DEBUG("LongPressStop");
}

void fDuringLongPress()
{
  LampControl lc;
  lc.state = 0;
  if (last_brightness > 245) {
    direction = -1;
  } else if (last_brightness < 15) {
    direction = 1;
  }
  last_brightness = last_brightness + (direction * 5);
  lc.brightness = last_brightness;
  sendData(myConfig.targetMacAddress, (uint8_t*)&lc, sizeof(lc));
  DEBUG("LongPress");
}

void ledShowStatus(int status) {
  #ifdef LED_PIN
  if (status == 1) {
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
  }
  #endif
}


void onTransmitCB(const uint8_t *mac_addr, esp_now_send_status_t status) {
  status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail";
  DEBUG("Last Packet Send Status: " + status);
  ledShowStatus(status == ESP_NOW_SEND_SUCCESS ? 1 : 0);
}


void sleepDevice(){
  DEBUG("Sleeping!!!!");
  esp_deep_sleep_start();
}
hw_timer_t* click_timer;

void setup()
{
  #ifdef EXTERNAL_WAKE
  esp_deep_sleep_enable_gpio_wakeup(BIT(D1), ESP_GPIO_WAKEUP_GPIO_LOW);
  #endif

  delay(WAIT_FOR_SERIAL);
  Serial.begin(115200);

  // Hardware based setup
  #ifdef CAMERA_MODEL_TTGO_T_CAMERA_PLUS
  pinMode(PIN_INPUT, INPUT);  //Note no internal pullup required as this has a hardware pullup
  #endif

  #if defined(ALIEXPRESS_ESP32C3) || defined(SEEED_XIAO_ESP32C3)
    pinMode(PIN_INPUT, INPUT_PULLUP);
  #endif


  // load in per-device config
  if(!loadConfig(myConfig)){
    DEBUG("Failed to load config correctly");
  }

  // setup button functionality
  button = new OneButton();
  button->attachClick(fClicked);
  // button->attachDoubleClick(fDoubleClicked);
  button->setLongPressIntervalMs(500);
  button->attachLongPressStart(fLongPressStart);
  button->attachLongPressStop(fLongPressStop);
  button->attachDuringLongPress(fDuringLongPress);

  // setup esp comms
  setupEspNow();
  registerTransmitCb(onTransmitCB);
  registerPeer(myConfig.targetMacAddress);

  #ifdef ALIEXPRESS_ESP32C3
  pinMode(LED_PIN, OUTPUT);
  #endif

  #ifdef EXTERNAL_WAKE
    click_timer = timerBegin(1, 128, true);

    // Not sure why using the proper timerAttachInterrupt function doesn't work for me, seems to work fine just checking the
    // timer value though??  
    // timerAttachInterrupt(click_timer, sleepDevice, true);
    // timerAlarmWrite(click_timer, MILLI_TILL_SLEEP + WAIT_FOR_SERIAL, false);
    // timerAlarmEnable(click_timer);
  #endif

  DEBUG("Enter loop..");

}

void loop()
{
  bool isPressed = (digitalRead(PIN_INPUT) == LOW);
  
  if(isPressed){
    timerRestart(click_timer);
  }

  button->tick(isPressed);

  #ifdef EXTERNAL_WAKE
  if(timerReadMilis(click_timer) > MILLI_TILL_SLEEP + WAIT_FOR_SERIAL ){
    sleepDevice();
  }
  #endif
}
