#include "SmartHouse.h"

unsigned char counter = 0;
unsigned int send_period = 1000;

configurations settings;

IPAddress apIP(192, 168, 1, 1);
ESP8266WebServer HTTP2(80);

void setup() {
  
  Serial.begin(9600);
  
  FS_init();
  load_json(settings);
  wifi_start();
  HTTP_init();

}

void loop() {
  HTTP2.handleClient();
  delay(1);
}