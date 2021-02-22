

void FS_init(void) {
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      Serial.println(fileName);
      size_t fileSize = dir.fileSize();
    }
  }
  
  HTTP.onNotFound([]() {
    if (!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "FileNotFound");
  });
}


String getContentType(String filename) {
  if (HTTP.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}


void save_json(const configurations &conf){

  SPIFFS.remove("/config.txt");
  
  File file = SPIFFS.open("/config.txt", "w");
  if (!file) {
    Serial.println("Config file open failed");
  }

  StaticJsonDocument<512> doc;

  doc["ssid"] = conf.ssid;
  doc["password"] = conf.password;
  doc["mqttUser"] = conf.mqttUser;
  doc["mqttPassword"] = conf.mqttPassword;
  doc["mqttContrIP"] = conf.mqttContrIP;
  doc["mqttContrPort"] = conf.mqttContrPort;
  doc["mqttContrSub"] = conf.mqttContrSub;
  doc["mqttContrPub"] = conf.mqttContrPub;

  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
  
}

void printFile() {
  
  File file = SPIFFS.open("/config.txt", "r");
  if (!file) {
    Serial.println("Failed to read file");
    return;
  }

  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  file.close();
}


//================ EEPROM FUNCTIONS (TO BE RETIRED) ================//

void load_settings(){ // to be retired
  
    settings.ssid = read_eeprom(0);
    settings.password = read_eeprom(20);
    settings.mqttUser = read_eeprom(40);
    settings.mqttPassword = read_eeprom(60);
    settings.mqttContrIP = read_eeprom(80);
    settings.mqttContrPort = read_eeprom(100);
    settings.mqttContrSub = read_eeprom(120);
    settings.mqttContrPub = read_eeprom(180);
  
  }

void save_settings(){ // to be retired
  
  Serial.println(settings.ssid);
  Serial.println(settings.password);
  Serial.println(settings.mqttUser);
  Serial.println(settings.mqttPassword);
  Serial.println(settings.mqttContrIP);
  Serial.println(settings.mqttContrPort);
  Serial.println(settings.mqttContrSub);
  Serial.println(settings.mqttContrPub);


  write_eeprom(settings.ssid, 0);
  write_eeprom(settings.password, 20);
  write_eeprom(settings.mqttUser, 40);
  write_eeprom(settings.mqttPassword, 60);
  write_eeprom(settings.mqttContrIP, 80);
  write_eeprom(settings.mqttContrPort, 100);
  write_eeprom(settings.mqttContrSub, 120);
  write_eeprom(settings.mqttContrPub, 180);

  save_json(settings);
  
  }
  
void write_eeprom(String buff, int addr){ // to be retired
  EEPROM.begin(512);
  delay(10);
  unsigned int len = buff.length();
  EEPROM.write(addr, len);
  
  for (int i = 0; i < len; ++i) 
    {
      EEPROM.write(addr + i + 1, buff[i]);
    }
  EEPROM.commit();
  }

String read_eeprom(int addr){ // to be retired
  EEPROM.begin(512); 
  delay(10); 
  unsigned int len = EEPROM.read(addr);
  String buff = "";
  for (int i = addr + 1; i < (addr + len + 1); ++i)
    {
          buff += char(EEPROM.read(i));
    }
  return buff;
  }

//================ EEPROM FUNCTIONS (TO BE RETIRED) ================//
