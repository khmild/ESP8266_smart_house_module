#include "SmartHouse.h"

void HTTP_init(void) {
  HTTP2.on("/restart", handle_Restart);
  HTTP2.on("/ssid", read_wifi_values);
  HTTP2.on("/mqtt", read_mqtt_values);
  HTTP2.begin();
}

void handle_Restart() {
  String restart = HTTP2.arg("device");
  if (restart == "ok") ESP.restart();
  HTTP2.send(200, "text/plain", "OK");
}

void read_wifi_values(){
  Serial.println("Reading wifi values");
  settings.ssid = HTTP2.arg("ssid");
  settings.password = HTTP2.arg("password");
  
  save_json(settings);
  
  HTTP2.send(200, "text/plain", "OK");
  }

void read_mqtt_values(){
  Serial.println("Reading mqtt values");
  settings.mqttUser = HTTP2.arg("mqttUser");
  settings.mqttPassword = HTTP2.arg("mqttPassword");
  settings.mqttContrIP = HTTP2.arg("ControllerIP");
  settings.mqttContrPort = HTTP2.arg("ControllerPort");
  settings.mqttContrSub = HTTP2.arg("ControllerSubscribe");
  settings.mqttContrPub = HTTP2.arg("ControllerPublish");

  save_json(settings);
  
  HTTP2.send(200, "text/plain", "OK");
  }