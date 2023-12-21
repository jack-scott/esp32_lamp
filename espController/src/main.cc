#include <Arduino.h>
#include <OneButton.h>
#include <now_comms.h>
#include <lamp_control_types.h>

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
#define WAIT_FOR_SERIAL 1000

// some dirty global stuff
OneButton *button;
String myMacAddress = "";
String myIpAddress = "";

uint8_t targetMacAddress[] = {0x84, 0x0D, 0x8E, 0x39, 0xD6, 0x8C}; //tenergy
// uint8_t targetMacAddress[] = {0xCC, 0x50, 0xE3, 0xB5, 0xF9, 0x38};

uint8_t last_brightness = 50;
int direction = 1;

void fClicked()
{
  Serial.println("Click");
}

void fDoubleClicked()
{
  LampControl lc;
  lc.state = 1;
  lc.brightness = last_brightness;
  sendData(targetMacAddress, (uint8_t*)&lc, sizeof(lc));
  Serial.println("DoubleClick");
}

void fLongPressStart()
{
  LampControl lc;
  lc.state = 0;
  lc.brightness = last_brightness;
  sendData(targetMacAddress, (uint8_t*)&lc, sizeof(lc));
  Serial.println("LongPressStart");
}

void fLongPressStop()
{
  Serial.println("LongPressStop");
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
  sendData(targetMacAddress, (uint8_t*)&lc, sizeof(lc));
  Serial.println("LongPress");
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
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  ledShowStatus(status == ESP_NOW_SEND_SUCCESS ? 1 : 0);
}


void sleepDevice(){
  Serial.println("Sleeping!!!!");
  delay(1000);
  esp_deep_sleep_start();
}
hw_timer_t* click_timer;

void setup()
{
  #ifdef EXTERNAL_WAKE
  esp_deep_sleep_enable_gpio_wakeup(BIT(D1), ESP_GPIO_WAKEUP_GPIO_LOW);
  #endif

  delay(WAIT_FOR_SERIAL);
  //TODO: Setup a timer and callback to put the device back to sleep 
  Serial.begin(115200);
  Serial.println("One Button Example with custom input.");
  // timerAttachInterrupt(click_timer, sleepDevice, true);

  // setup your own source of input
  #ifdef CAMERA_MODEL_TTGO_T_CAMERA_PLUS
  pinMode(PIN_INPUT, INPUT);  //Note no internal pullup required as this has a hardware pullup
  #endif

  #if defined(ALIEXPRESS_ESP32C3) || defined(SEEED_XIAO_ESP32C3)
    pinMode(PIN_INPUT, INPUT_PULLUP);
  #endif

// create the OneButton instance without a pin.
  button = new OneButton();

  button->attachClick(fClicked);
  button->attachDoubleClick(fDoubleClicked);
  button->setLongPressIntervalMs(500);
  button->attachLongPressStart(fLongPressStart);
  button->attachLongPressStop(fLongPressStop);
  button->attachDuringLongPress(fDuringLongPress);

  setupEspNow();

  registerTransmitCb(onTransmitCB);
  registerPeer(targetMacAddress);

  // digitalWrite(LED_PIN, HIGH);
  #ifndef SEEED_XIAO_ESP32C3
  pinMode(LED_PIN, OUTPUT);
  #endif
  click_timer = timerBegin(1, 128, 1000);
  Serial.println("Enter loop..");

} // setup()

void loop()
{
  // read your own source of input:
  bool isPressed = (digitalRead(PIN_INPUT) == LOW);

  for (int i = 0; i < 20; i++) {
    button->tick(isPressed);
  }
  // call tick frequently with current push-state of the input
  button->tick(isPressed);
  #ifdef EXTERNAL_WAKE
  if(timerReadMilis(click_timer) > MILLI_TILL_SLEEP + WAIT_FOR_SERIAL ){
    sleepDevice();
  }
  #endif
} // loop()
