#ifndef FLAG
#define FLAG

/** Use DEBUGING flag for showing detailed info in serial port */
#define DEBUGING

//=================== LIBRARIES ===================//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>         //SPIFFS
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <Wire.h>             
#include <AHT10.h>

#include<SH1106.h>
//=================== VARIABLES ===================//
extern unsigned char counter;
extern unsigned int send_period;

/** Default values for ESP8266 AP connection */
const char ap_ssid[] = "ESP_AP";
const char ap_password[] = "12345678";

/** Structure for MQTT and WiFi connection storage*/
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

//=================== USER VARIABLES ===================//
/** Temperature/Humidity sensor AHT10 */
extern AHT10 AHT10_sens;
extern float AHT10_temp;
extern float AHT10_humid;

void AHT10_setup();
void AHT10_read();

/** OLED display */
extern SH1106 display;
void display_setup();
void display_values();


//=================== MQTT SETTINGS ===================//
/** Use MQTT_MSG_SIZE parameter for adjusting maximum MQTT message size */
#define MQTT_MSG_SIZE 50
extern WiFiClient espClient;
extern PubSubClient client;
extern String received_message;

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
void mqtt_send(int value);
void mqtt_send(double value);
void mqtt_send(const char* value);
void mqtt_send(float value, String subtopic);
void mqtt_connect();


#endif