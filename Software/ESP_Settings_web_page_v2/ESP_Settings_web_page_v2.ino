#include "ESP_Settings.h"

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
