/**
* Use mqtt_send() to send data to mqtt broker
* Recieved data are stored in a recieved_message variable
* functions.cpp file is created for user functions
*/

#include "SmartHouse.h"

unsigned char counter = 0;
unsigned int send_period = 1000;

configurations settings;

IPAddress apIP(192, 168, 1, 1);
ESP8266WebServer HTTP(80);

WiFiClient espClient;
PubSubClient client(espClient);

String received_message;

AHT10 AHT10_sens(AHT10_ADDRESS_0X38);
float AHT10_temp = 0;
float AHT10_humid = 0;

SH1106 display(0x3c, SDA, SCL);

unsigned long old_time = 0;

void setup() {
  
  Serial.begin(9600);

  FS_init();
  load_json(settings);
  mqtt_setup();
  wifi_start();
  HTTP_init();
  AHT10_setup();
  display_setup();

}

void loop() {
  
  HTTP.handleClient();
  delay(1);

  client.loop();

  if (millis() - old_time > 2000)
  {
    AHT10_read();
    display_values();
    old_time = millis();
    Serial.print("Temperature: ");
    Serial.println(AHT10_temp);
    Serial.print("Humidity: ");
    Serial.println(AHT10_humid);
  }
  
}