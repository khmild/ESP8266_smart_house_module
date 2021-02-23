#ifndef FLAG
#define FLAG

#define DEBUG

//=================== LIBRARIES ===================//
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h> //SPIFFS
#include <ArduinoJson.h>

//=================== VARIABLES ===================//
unsigned char counter = 0;
const char ap_ssid[] = "ESP_AP";
const char ap_password[] = "12345678";

unsigned int send_period = 1000;

struct configurations {
  String ssid;
  String password;
  String mqttUser;
  String mqttPassword;
  String mqttContrIP;
  String mqttContrPort;
  String mqttContrSub;
  String mqttContrPub;
};

configurations settings;

IPAddress apIP(192, 168, 1, 1);
ESP8266WebServer HTTP(80);
File fsUploadFile;


#endif
