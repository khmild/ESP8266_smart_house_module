#include "ESP_Settings.h"

void wifi_start(){
  Serial.println("******** BEGIN ***********");

  Serial.println(settings.ssid);
  Serial.println(settings.password);
  
  Serial.println("*******WIFI_STA***********");
  WiFi.mode(WIFI_STA);

  Serial.println("******CONNECTING**********");
  WiFi.begin(settings.ssid, settings.password);

  while (WiFi.status() != WL_CONNECTED) {
    if(counter > 20){
        Serial.println();
        Serial.println("********* ERROR ***********");
        ap_start();
        return;    
    }
    delay(500);
    Serial.print(".");
    counter++;
  }
  Serial.println();
  Serial.println("******** CONNECTED ***********");
  }

void ap_start(){
  Serial.println("******** STARTING AP ***********");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("******** AP STARTED ***********");
  }
