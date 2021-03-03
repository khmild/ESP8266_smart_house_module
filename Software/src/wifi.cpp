#include "SmartHouse.h"

void wifi_start(){
  Serial.println("******** BEGIN ***********");

  #ifdef DEBUGING
    Serial.println();
    Serial.println("Connecting to:");
    Serial.println(settings.ssid);
    Serial.println(settings.password);
    Serial.println();
  #endif
  
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

void mqtt_setup(){
  uint8_t Parts_ip[4] = {0,0,0,0};
  uint8_t Part = 0;

  for (unsigned int i=0; i < settings.mqttContrIP.length(); i++ )
  {
  	char c = settings.mqttContrIP[i];
	  if ( c == '.' )
  	{
	  	Part++;
	  	continue;
	  }
	Parts_ip[Part] *= 10;
	Parts_ip[Part] += c - '0';
  }

  IPAddress ip( Parts_ip[0], Parts_ip[1], Parts_ip[2], Parts_ip[3] );
  
  client.setServer(ip, settings.mqttContrPort.toInt());
  client.setCallback(callback);
  
  #ifdef DEBUGING
    Serial.println("MQTT sever parameters:");
    Serial.print("IP:");
    Serial.println(ip);
    Serial.print("Port:");
    Serial.println(settings.mqttContrPort.toInt());
  #endif
}

void mqtt_connect(){
  unsigned int len = settings.mqttContrSub.length() + 1;
  char sub_buf[len];
  settings.mqttContrSub.toCharArray(sub_buf, len);

  #ifdef DEBUGING
    Serial.println("MQTT connection...");
    Serial.print("Subscribing to: ");
    Serial.println(sub_buf);
  #endif

  String clientId = "Client-";
  clientId += String(random(0xffff), HEX);
  
  if (client.connect(clientId.c_str())) {
    Serial.println("MQTT Connected");
    client.subscribe(sub_buf);
  } 
  else {
    Serial.print("MQTT Connection Failed, rc=");
    Serial.println(client.state());
  }

}

void callback(const char* topic, byte* payload, unsigned int length){
  
  
  #ifdef DEBUGING
    Serial.print("Recieved message from [");
    Serial.print(topic);
    Serial.println("]");
    Serial.print("Message: ");
    //Serial.println();
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  Serial.println();
  #endif


}

void mqtt_send(){
  if (!client.connected()) {
    mqtt_connect();
  }

  unsigned int len = settings.mqttContrPub.length() + 1;
  char pub_buf[len];
  settings.mqttContrPub.toCharArray(pub_buf, len);
  
  #ifdef DEBUGING
    Serial.println("Publishing value:");
  #endif

  client.publish(pub_buf,"99");
}