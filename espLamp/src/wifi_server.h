#pragma once

#include <WiFi.h>
#include <WebServer.h>  // standard library

// replace this with your homes intranet connect parameters
#define LOCAL_SSID ""
#define LOCAL_PASS ""

// once  you are read to go live these settings are what you client will connect to
#define AP_SSID "Lamp-Access-Point"
#define AP_PASS "frostyfruit"

class LampWebPage{
public:
    LampWebPage();
    void SetupServer();
    void SendWebsite();
    void SendXML();
    void UpdateSlider();
    void UpdateTheme();
    int getBrightness(){
        return brightness;
    }
    int getTheme(){
        return theme;
    }
    void printIp();
    void tick();

private:

    IPAddress assigned_ip;

    // just some buffer holder for char operations
    char buf[32];
    // the XML array size needs to be bigger that your maximum expected size. 2048 is way too big for this example
    char XML[2048];

    int brightness = 0;
    
    int theme;

    long counter = 0;
    bool some_output = false;

    // Set web server port number to 80
    WebServer* server;
};