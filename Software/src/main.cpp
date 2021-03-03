#include "SmartHouse.h"

unsigned char counter = 0;
unsigned int send_period = 1000;

configurations settings;

IPAddress apIP(192, 168, 1, 1);
ESP8266WebServer HTTP(80);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long time_last = 0;

String received_message;

void setup() {
  
  Serial.begin(9600);

  FS_init();
  load_json(settings);
  mqtt_setup();
  wifi_start();
  HTTP_init();

}

void loop() {
  
  HTTP.handleClient();
  delay(1);

  client.loop();

  if (millis() - time_last > 5000)
  {
    mqtt_send(11);
    mqtt_send(5.55);
    mqtt_send("999");
    time_last = millis();
  }

}