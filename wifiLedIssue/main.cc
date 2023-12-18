#include <Wire.h>
#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>  // standard library




#ifdef CAMERA_MODEL_TTGO_T_CAMERA_PLUS
#define NUM_LEDS 22
#define LED_PIN 23
#define FASTLED_ESP32_I2S
#endif

#ifdef ALIEXPRESS_ESP32C3
#define NUM_LEDS 24
#define LED_PIN 10
#define FASTLED_RMT_MAX_CHANNELS 2
#endif

#ifdef TENERGY_ESP32S3
#define NUM_LEDS 12
#define LED_PIN 23
#define FASTLED_ESP32_I2S
#endif

CRGB leds[NUM_LEDS];

void setupWebServer(){
    #ifdef USE_INTRANET
    WiFi.begin(LOCAL_SSID, LOCAL_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
    assigned_ip = WiFi.localIP();
    #endif

    // if you don't have #define USE_INTRANET, here's where you will creat and access point
    // an intranet with no internet connection. But Clients can connect to your intranet and see
    // the web page you are about to serve up
    #ifndef USE_INTRANET
    // Replace with your network credentials
    #define AP_SSID "ESP32-Access-Point"
    #define AP_PASS "123456789"

    WiFi.softAP(AP_SSID, AP_PASS);
    delay(100);
    // WiFi.softAPConfig(PageIP, gateway, subnet);
    delay(100);
    IPAddress assigned_ip;
    assigned_ip = WiFi.softAPIP();
    Serial.print("IP address: "); Serial.println(assigned_ip);
    #endif
}

void setup() {
    // Serial.begin(115200);
    delay(1000); // give me time to bring up serial monitor
    Serial.println("Starting up");
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    // WiFi.mode(WIFI_OFF);
    setupWebServer();
}

int rotate = 0;
bool indicator = false;
void loop() {
    for(int i = 0; i < NUM_LEDS; i++){
        int mod = (i + 1) *2;
        leds[i] = CRGB(255/mod,rotate/mod,rotate/mod);
        if (indicator) {
            leds[10] = CRGB(0,0,0);
            leds[11] = CRGB(0,0,0);
        }
        FastLED.show();
        Serial.print("dfsDFS");
        Serial.println(rotate);
        Serial.println(mod);
        delay(5);
    }
    rotate ++;
    if (rotate > 100) {
        rotate = 0;
    }
    indicator = !indicator;
}
