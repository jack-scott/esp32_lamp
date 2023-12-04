#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"

#include <esp_now.h>

#define CAMERA_MODEL_TTGO_T_CAMERA_PLUS
#define SOFTAP_MODE       //The comment will be connected to the specified ssid

#define TRANSMITTER

/***************************************
 *  WiFi
 **************************************/
#define WIFI_SSID "YourSSID"
#define WIFI_PASSWD "YourPASSWORD"

#include "select_pins.h"

#if defined(SOFTAP_MODE)
#endif
String macAddress = "";
String ipAddress = "";


#if defined(ENABLE_TFT)
// Depend TFT_eSPI library ,See  https://github.com/Bodmer/TFT_eSPI
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#endif

bool setupDisplay()
{
    #if defined(ENABLE_TFT)
    #if defined(CAMERA_MODEL_TTGO_T_CAMERA_PLUS)
        tft.init();
        tft.setRotation(0);
        tft.fillScreen(TFT_BLACK);
        tft.setTextSize(2);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("TFT_eSPI", tft.width() / 2, tft.height() / 2);
        tft.drawString("LilyGo Camera Plus", tft.width() / 2, tft.height() / 2 + 20);
        pinMode(TFT_BL_PIN, OUTPUT);
        digitalWrite(TFT_BL_PIN, HIGH);
    #endif
    #endif 
    return true;
}
bool deviceProbe(uint8_t addr)
{
    Wire.beginTransmission(addr);
    return Wire.endTransmission() == 0;
}
bool setupPower()
{
    #if defined(ENABLE_IP5306)
    #define IP5306_ADDR 0X75
    #define IP5306_REG_SYS_CTL0 0x00
        if (!deviceProbe(IP5306_ADDR))
            return false;
        bool en = true;
        Wire.beginTransmission(IP5306_ADDR);
        Wire.write(IP5306_REG_SYS_CTL0);
        if (en)
            Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
        else
            Wire.write(0x35); // 0x37 is default reg value
        return Wire.endTransmission() == 0;
    #endif
    return true;
}


void setupNetwork()
{
    WiFi.mode(WIFI_STA);
    macAddress = WiFi.macAddress();
    ipAddress = WiFi.localIP().toString();
    // macAddress = "LilyGo-CAM-";
    // #ifdef SOFTAP_MODE
    //     WiFi.mode(WIFI_AP);
    //     macAddress = WiFi.softAPmacAddress();
    //     WiFi.softAP(macAddress.c_str());
    //     ipAddress = WiFi.softAPIP().toString();
    // #else
    //     WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    //     while (WiFi.status() != WL_CONNECTED) {
    //         delay(500);
    //         Serial.print(".");
    //     }
    //     Serial.println("");
    //     Serial.println("WiFi connected");
    //     ipAddress = WiFi.localIP().toString();
    //     macAddress += WiFi.macAddress().substring(0, 5);
    // #endif
    // #if defined(ENABLE_TFT)
    // #if defined(CAMERA_MODEL_TTGO_T_CAMERA_PLUS)
    //     tft.drawString("ipAddress:", tft.width() / 2, tft.height() / 2 + 50);
    //     tft.drawString(ipAddress, tft.width() / 2, tft.height() / 2 + 72);
    // #endif
    // #endif
}


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  int x;
  int y;
}struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};

// Create peer interface
esp_now_peer_info_t peerInfo;

int i = 0;

void initialiseBoardStructs(){
  for (int i = 0; i < 3; i++){
    boardsStruct[i].id = i+1;
    boardsStruct[i].x = 0;
    boardsStruct[i].y = 0;
  }
    myData.id = 0;
    myData.x = 0;
    myData.y = 0;
}

#ifdef RECIEVER
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}
#endif

#ifdef TRANSMITTER
esp_now_send_status_t last_packet_sent = ESP_NOW_SEND_FAIL;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    last_packet_sent = status;
}
#endif

String lastx1 = "";
String lasty1 = "";
String lastx2 = "";
String lasty2 = "";

int alive = 0;

void loopDisplay(){
    delay(50);
    #if defined(ENABLE_TFT)
    // tft.fillScreen(TFT_BLACK);

    //convert i to string
    // tft.fillRect(0, 100, tft.width(), tft.height()-100, TFT_BLACK);

    tft.setTextColor(TFT_BLACK, TFT_BLACK);
    tft.drawString(lastx1, 130 , 120);
    tft.drawString(lasty1, 200 , 120);
    tft.drawString(lastx2, 130 , 160);
    tft.drawString(lasty2, 200 , 160);
    tft.drawString(String(alive), 180, 180);


    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("x: ", 110 , 120);
    tft.drawString(String(boardsStruct[0].x), 130 , 120);
    lastx1 = String(boardsStruct[0].x);
    tft.drawString("y: ", 180 , 120);
    tft.drawString(String(boardsStruct[0].y), 200 , 120);
    lasty1 = String(boardsStruct[0].y);
    tft.drawString("x: ", 110 , 160);
    tft.drawString(String(boardsStruct[1].x), 130 , 160);
    lastx2 = String(boardsStruct[1].x);
    tft.drawString("y: ", 180 , 160);
    tft.drawString(String(boardsStruct[1].y), 200 , 160);
    lasty2 = String(boardsStruct[1].y);

    tft.drawString("Alive: ", 80, 180);
    #ifdef RECIEVER
    tft.drawString("reciever", tft.width()/2, 200);
    #endif
    #ifdef TRANSMITTER
    if (last_packet_sent == ESP_NOW_SEND_SUCCESS) {
        tft.setTextColor(TFT_WHITE, TFT_GREEN, true);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_ORANGE, false);
    }
    tft.drawString("transmitter", tft.width()/2, 200);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    #endif
    tft.drawString(String(alive), 180, 180);
    #endif
    alive ++;
    // Serial.println("loopDisplay");
}

uint8_t broadcastAddress[] = {0xCC, 0x50, 0xE3, 0xB5, 0xF8, 0x74};


void initEspNow(){
      //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
#ifdef RECIEVER
        esp_now_register_recv_cb(OnDataRecv);
    #endif
  
    #ifdef TRANSMITTER
        // Once ESPNow is successfully Init, we will register for Send CB to
        // get the status of Trasnmitted packet
        esp_now_register_send_cb(OnDataSent);
        
        // Register peer
        memcpy(peerInfo.peer_addr, broadcastAddress, 6);
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;
        
        // Add peer        
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("Failed to add peer");
            return;
        }
    #endif

}

void setup()
{

    Serial.begin(115200);
    #if defined(I2C_SDA) && defined(I2C_SCL)
        Wire.begin(I2C_SDA, I2C_SCL);
    #endif

    bool status;
    status = setupDisplay();
    Serial.print("setupDisplay status ");
    Serial.println(status);

    status = setupPower();
    Serial.print("setupPower status ");
    Serial.println(status);

    setupNetwork();


    Serial.print("Camera Ready! Use 'http://");
    Serial.print(ipAddress);
    Serial.println("' to connect");

    // Initialise the board structures
    initialiseBoardStructs();
    initEspNow();
    tft.fillScreen(TFT_BLACK);
    tft.drawString("MacAddress:", tft.width() / 2, 20);
    tft.drawString(macAddress, tft.width() / 2, 40);
    tft.drawString("ipAddress:", tft.width() / 2, 60);
    tft.drawString(ipAddress, tft.width() / 2, 80);

    tft.drawRect(0, 100, tft.width(), tft.height()-100, TFT_SKYBLUE);
    tft.drawString("DEV 1: ", 60 , 120);
    tft.drawString("DEV 2: ", 60 , 160);
}

#ifdef TRANSMITTER
void sendPacket(){
    // Set values to send
    myData.id = 1;
    myData.x ++;
    if (myData.x > 100) {
        myData.x = 0;
        myData.y ++;
    }
    boardsStruct[0].x = myData.x;
    boardsStruct[0].y = myData.y;
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }
}
#endif


void loop()
{
    loopDisplay();
    #ifdef TRANSMITTER
        sendPacket();
    #endif
}
    
