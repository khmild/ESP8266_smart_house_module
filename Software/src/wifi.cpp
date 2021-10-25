#include "SmartHouse.h"


/** 
 * WFI_START FUNCTION
 * This function is used for connecting to WiFi network
 * Uses parameters stored in settings.ssid and settings.password variables
 */
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

/**
 * AP_START FUNCTION
 * Function used for creation WiFi AP when can't connect to the WiFi network
 * Creates WiFi acces point with parameters specified in ap_ssid and ap_password variables
 */
void ap_start(){
  Serial.println("******** STARTING AP ***********");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("******** AP STARTED ***********");
}

/**
 * MQTT_SETUP FUNCTION
 * Sets MQTT connection parameters
 * Converts IP String to IPAdress format
 */
void mqtt_setup(){
  uint8_t Parts_ip[4] = {0,0,0,0};
  uint8_t Part = 0;

  //convertion string to IPAdress format
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
  
  //set server parameters and callback function
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

/**
 * MQTT_CONNECT FUNCTION
 * Connects to MQTT broker using parameters stored in settings.mqtt****
 * converts all the String parameters to the char arrays at the begining
 */
void mqtt_connect(){
  //convert subscribe string
  unsigned int len = settings.mqttContrSub.length() + 1;
  char sub_buf[len];
  settings.mqttContrSub.toCharArray(sub_buf, len);

  //convert MQTT User string
  len = settings.mqttUser.length() + 1;
  char user[len];
  settings.mqttUser.toCharArray(user, len);

  //convert MQTT Password string
  len = settings.mqttPassword.length() + 1;
  char pass[len];
  settings.mqttPassword.toCharArray(pass, len);

  #ifdef DEBUGING
    Serial.println("MQTT connection...");
    Serial.print("Subscribing to: ");
    Serial.println(sub_buf);
    Serial.print("MQTT User: ");
    Serial.println(user);
    Serial.println("MQTT Password: ");
    Serial.println(pass);
  #endif

  String clientId = "Client-";
  clientId += String(random(0xffff), HEX);
  
  if (client.connect(clientId.c_str(),user, pass)) {
    Serial.println("MQTT Connected");
    client.subscribe(sub_buf);
  } 
  else {
    Serial.print("MQTT Connection Failed, rc=");
    Serial.println(client.state());
  }

}

/**
 * CALLBACK FUNCTION
 * !!! DO NOT CHANGE HEADER !!!
 * this function is called when new message in subscribe topic appears
 * stores message in recieved_message variable
 */
void callback(const char* topic, byte* payload, unsigned int length){
  
  received_message = "";

  for (unsigned int i = 0; i < length; i++) {
    received_message += (char)payload[i];
  }

  #ifdef DEBUGING
    Serial.print("Recieved message from [");
    Serial.print(topic);
    Serial.println("]");
    Serial.print("Message: ");
    Serial.println(received_message);
  #endif

}

/**
 * MQTT_SEND FUNCTION (INT TYPE)
 * publishes int value into specified topic
 */
void mqtt_send(int value){
  if (!client.connected()) {
    mqtt_connect();
  }

  char send_buf[MQTT_MSG_SIZE];
  snprintf(send_buf, MQTT_MSG_SIZE, "%d", value);

  unsigned int len = settings.mqttContrPub.length() + 1;
  char pub_buf[len];
  settings.mqttContrPub.toCharArray(pub_buf, len);
  
  #ifdef DEBUGING
    Serial.print("Publishing value:");
    Serial.println(send_buf);
  #endif

  client.publish(pub_buf, send_buf);
}

/**
 * MQTT_SEND FUNCTION (DOUBLE TYPE)
 * publishes double value into specified topic
 */
void mqtt_send(double value){
  if (!client.connected()) {
    mqtt_connect();
  }

  char send_buf[MQTT_MSG_SIZE];
  snprintf(send_buf, MQTT_MSG_SIZE, "%f", value);

  unsigned int len = settings.mqttContrPub.length() + 1;
  char pub_buf[len];
  settings.mqttContrPub.toCharArray(pub_buf, len);
  
  #ifdef DEBUGING
    Serial.print("Publishing value:");
    Serial.println(send_buf);
  #endif

  client.publish(pub_buf, send_buf);
}

/**
 * MQTT_SEND FUNCTION (CHAR TYPE)
 * publishes char value into specified topic
 */
void mqtt_send(const char* value){
  if (!client.connected()) {
    mqtt_connect();
  }

  unsigned int len = settings.mqttContrPub.length() + 1;
  char pub_buf[len];
  settings.mqttContrPub.toCharArray(pub_buf, len);
  
  #ifdef DEBUGING
    Serial.print("Publishing value:");
    Serial.println(value);
  #endif

  client.publish(pub_buf, value);
}

/**
 * MQTT_SEND FUNCTION (CHAR TYPE)
 * publishes float value into topic from web + specified subtopic
 * useful for more values communication
 */
void mqtt_send(float value, String subtopic){
  if (!client.connected()) {
    mqtt_connect();
  }

  /*convert value to string*/
  char send_buf[MQTT_MSG_SIZE];
  snprintf(send_buf, MQTT_MSG_SIZE, "%.1f", value);

  String topic = settings.mqttContrPub + subtopic;
  unsigned int len = topic.length() + 1;
  char pub_buf[len];
  topic.toCharArray(pub_buf, len);
  
  #ifdef DEBUGING
    Serial.print("Publishing value:");
    Serial.println(send_buf);
  #endif

  client.publish(pub_buf, send_buf);
}