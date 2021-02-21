void HTTP_init(void) {
  HTTP.on("/restart", handle_Restart);
  HTTP.on("/ssid", read_wifi_values);
  HTTP.on("/mqtt", read_mqtt_values);
  HTTP.begin();
}

void handle_Restart() {
  String restart = HTTP.arg("device");
  if (restart == "ok") ESP.restart();
  HTTP.send(200, "text/plain", "OK");
}

void read_wifi_values(){
  Serial.println("Reading wifi values");
  ssid = HTTP.arg("ssid");
  password = HTTP.arg("password");
  
  save_settings();
  
  HTTP.send(200, "text/plain", "OK");
  }

void read_mqtt_values(){
  Serial.println("Reading mqtt values");
  mqttUser = HTTP.arg("mqttUser");
  mqttPassword = HTTP.arg("mqttPassword");
  mqttContrIP = HTTP.arg("ControllerIP");
  mqttContrPort = HTTP.arg("ControllerPort");
  mqttContrSub = HTTP.arg("ControllerSubscribe");
  mqttContrPub = HTTP.arg("ControllerPublish");

  save_settings();
  
  HTTP.send(200, "text/plain", "OK");
  }
