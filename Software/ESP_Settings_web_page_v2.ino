#include "ESP_Settings.h"

unsigned char counter = 0;

const char ap_ssid[] = "ESP_AP";
const char ap_password[] = "12345678";

unsigned int send_period = 1000;

String ssid;
String password;

String mqttUser;
String mqttPassword;
String mqttContrIP;
String mqttContrPort;
String mqttContrSub;
String mqttContrPub;

IPAddress apIP(192, 168, 1, 1);

ESP8266WebServer HTTP(80);
File fsUploadFile;


void setup() {
  
  Serial.begin(9600);
  
  FS_init();
  load_settings();
  wifi_start();
  HTTP_init();

}

void loop() {
  HTTP.handleClient();
  delay(1);
}
