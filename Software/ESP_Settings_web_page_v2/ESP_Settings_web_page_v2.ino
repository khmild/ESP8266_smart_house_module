#include "ESP_Settings.h"

void setup() {
  
  Serial.begin(9600);
  
  FS_init();
  load_json(settings);
  wifi_start();
  HTTP_init();

  delay(500);
  printFile();
}

void loop() {
  HTTP.handleClient();
  delay(1);
}
