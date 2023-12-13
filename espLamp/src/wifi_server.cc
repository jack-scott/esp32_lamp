// // Following the instrcutions and boilerplate from this repo https://github.com/KrisKasprzak/ESP32_WebPage/tree/main



// // Load Wi-Fi library
// #include <WiFi.h>
// #include <WebServer.h>  // standard library
// #include "index.h"      // this is the web page you will serve up

// #define USE_INTRANET

// // replace this with your homes intranet connect parameters
// #define LOCAL_SSID "NetComm 4592"
// #define LOCAL_PASS "senahesumf"

// // once  you are read to go live these settings are what you client will connect to
// #define AP_SSID "ESP32-Access-Point"
// #define AP_PASS "123456789"

// IPAddress assigned_ip;

// // just some buffer holder for char operations
// char buf[32];
// // the XML array size needs to be bigger that your maximum expected size. 2048 is way too big for this example
// char XML[2048];
// int brightness = 0;


// // Set web server port number to 80
// WebServer server(80);


// long counter = 0;
// bool SomeOutput = false;


// // code to send the main web page
// // PAGE_MAIN is a large char defined in index.h
// void SendWebsite() {
//   Serial.println("sending web page");
//   // you may have to play with this value, big pages need more porcessing time, and hence
//   // a longer timeout that 200 ms
//   server.send(200, "text/html", PAGE_MAIN);

// }

// // code to send the main web page
// // I avoid string data types at all cost hence all the char mainipulation code
// void SendXML() {

//   // Serial.println("sending xml");

//   strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

//   // // send bitsA0
//   // sprintf(buf, "<B0>%d</B0>\n", BitsA0);
//   // strcat(XML, buf);
//   // // send Volts0
//   // sprintf(buf, "<V0>%d.%d</V0>\n", (int) (VoltsA0), abs((int) (VoltsA0 * 10)  - ((int) (VoltsA0) * 10)));
//   // strcat(XML, buf);

//   // // send bits1
//   // sprintf(buf, "<B1>%d</B1>\n", BitsA1);
//   // strcat(XML, buf);
//   // // send Volts1
//   // sprintf(buf, "<V1>%d.%d</V1>\n", (int) (VoltsA1), abs((int) (VoltsA1 * 10)  - ((int) (VoltsA1) * 10)));
//   // strcat(XML, buf);

//   // // show led0 status
//   // if (LED0) {
//   //   strcat(XML, "<LED>1</LED>\n");
//   // }
//   // else {
//   //   strcat(XML, "<LED>0</LED>\n");
//   // }

//   if (SomeOutput) {
//     strcat(XML, "<SWITCH>1</SWITCH>\n");
//   }
//   else {
//     strcat(XML, "<SWITCH>0</SWITCH>\n");
//   }

//   // send counter
//   sprintf(buf, "<COUNTER>%ld</COUNTER>\n", counter);
//   strcat(XML, buf);

//   strcat(XML, "</Data>\n");
//   // wanna see what the XML code looks like?
//   // actually print it to the serial monitor and use some text editor to get the size
//   // then pad and adjust char XML[2048]; above
//   Serial.println(XML);

//   // you may have to play with this value, big pages need more porcessing time, and hence
//   // a longer timeout that 200 ms
//   server.send(200, "text/xml", XML);


// }

// // function managed by an .on method to handle slider actions on the web page
// // this example will get the passed string called VALUE and conver to a pwm value
// // and control the fan speed
// void UpdateSlider() {

//   // many I hate strings, but wifi lib uses them...
//   String t_state = server.arg("VALUE");

//   // conver the string sent from the web page to an int
//   brightness = t_state.toInt();
//   Serial.print("UpdateSlider"); Serial.println(brightness);


//   // YOU MUST SEND SOMETHING BACK TO THE WEB PAGE--BASICALLY TO KEEP IT LIVE

//   // option 1: send no information back, but at least keep the page live
//   // just send nothing back
//   // server.send(200, "text/plain", ""); //Send web page

//   // option 2: send something back immediately, maybe a pass/fail indication, maybe a measured value
//   // here is how you send data back immediately and NOT through the general XML page update code
//   // my simple example guesses at fan speed--ideally measure it and send back real data
//   // i avoid strings at all caost, hence all the code to start with "" in the buffer and build a
//   // simple piece of data
//   strcpy(buf, "");
//   sprintf(buf, "%d", brightness);
//   sprintf(buf, buf);

//   // now send it back
//   server.send(200, "text/plain", buf); //Send web page

// }


// void setup() {
//     Serial.begin(115200);
    
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

//     // these calls will handle data coming back from your web page
//     // this one is a page request, upon ESP getting / string the web page will be sent
//     server.on("/", SendWebsite);
//     // upon esp getting /XML string, ESP will build and send the XML, this is how we refresh
//     // just parts of the web page
//     server.on("/xml", SendXML);
//     // upon ESP getting /UPDATE_SLIDER string, ESP will execute the UpdateSlider function
//     // same notion for the following .on calls
//     // add as many as you need to process incoming strings from your web page
//     // as you can imagine you will need to code some javascript in your web page to send such strings
//     // this process will be documented in the SuperMon.h web page code
//     server.on("/UPDATE_SLIDER", UpdateSlider);

//     server.begin();
// }

// void loop(){
//   if (counter % 1000 == 0) {
//     SomeOutput = !SomeOutput;
//   }
//   counter++;
//   server.handleClient();
// }

