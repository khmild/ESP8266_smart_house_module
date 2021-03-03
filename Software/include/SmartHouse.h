#ifndef FLAG
#define FLAG

#define DEBUGING

//=================== LIBRARIES ===================//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>         //SPIFFS
#include <ArduinoJson.h>
#include <PubSubClient.h>

//=================== VARIABLES ===================//
extern unsigned char counter;

const char ap_ssid[] = "ESP_AP";
const char ap_password[] = "12345678";

extern unsigned int send_period;

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

extern configurations settings;

extern IPAddress apIP;
extern ESP8266WebServer HTTP;

//=================== MQTT SETTINGS ===================//
extern WiFiClient espClient;
extern PubSubClient client;

//=================== PROTOTYPES ===================//
void wifi_start();
void ap_start();
void FS_init(void);
String getContentType(String filename);
bool handleFileRead(String path);
void save_json(const configurations &conf);
void load_json(configurations &conf);
void printFile();
void HTTP_init(void);
void handle_Restart();
void read_wifi_values();
void read_mqtt_values();
void mqtt_setup();
void callback(const char* topic, byte* payload, unsigned int length);
void mqtt_send();
void mqtt_connect();


#endif